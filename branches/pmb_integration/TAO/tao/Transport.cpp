// $Id$

#include "Transport.h"

#include "LF_Follower.h"
#include "Leader_Follower.h"
#include "Client_Strategy_Factory.h"
#include "Wait_Strategy.h"
#include "Transport_Mux_Strategy.h"
#include "Stub.h"
#include "Sync_Strategies.h"
#include "Connection_Handler.h"
#include "Pluggable_Messaging.h"
#include "Synch_Queued_Message.h"
#include "Asynch_Queued_Message.h"
#include "Flushing_Strategy.h"
#include "Thread_Lane_Resources.h"
#include "Resume_Handle.h"
#include "Codeset_Manager.h"
#include "Codeset_Translator_Factory.h"
#include "GIOP_Message_State.h"
#include "debug.h"

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Reactor.h"
#include "ace/os_include/sys/os_uio.h"


#if !defined (__ACE_INLINE__)
# include "Transport.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Transport,
           "$Id$")

/*
 * Static function in file scope
 */
static void
dump_iov (iovec *iov, int iovcnt, size_t id,
          size_t current_transfer,
          const char *location)
{
  ACE_Log_Msg::instance ()->acquire ();

#define DUMP_IOV_PREFIX  ACE_TEXT("Transport[") ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT("]::%s")
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - ")
              DUMP_IOV_PREFIX ACE_TEXT (", ")
              ACE_TEXT ("sending %d buffers\n"),
              id, ACE_TEXT_CHAR_TO_TCHAR (location), iovcnt));

  for (int i = 0; i != iovcnt && 0 < current_transfer; ++i)
    {
      size_t iov_len = iov[i].iov_len;

      // Possibly a partially sent iovec entry.
      if (current_transfer < iov_len)
        {
          iov_len = current_transfer;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  DUMP_IOV_PREFIX ACE_TEXT (", ")
                  ACE_TEXT ("buffer %d/%d has %d bytes\n"),
                  id, ACE_TEXT_CHAR_TO_TCHAR(location),
                  i, iovcnt,
                  iov_len));

      size_t len;

      for (size_t offset = 0; offset < iov_len; offset += len)
        {
          ACE_TCHAR header[1024];
          ACE_OS::sprintf (header,
                           ACE_TEXT("TAO - ") DUMP_IOV_PREFIX ACE_TEXT(" (")
                           ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT("/")
                           ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT(")\n"),
                           id, location, offset, iov_len);

          len = iov_len - offset;

          if (len > 512)
            {
              len = 512;
            }

          ACE_HEX_DUMP ((LM_DEBUG,
                         static_cast<char*> (iov[i].iov_base) + offset,
                         len,
                         header));
        }
      current_transfer -= iov_len;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - ")
              DUMP_IOV_PREFIX ACE_TEXT (", ")
              ACE_TEXT ("end of data\n"),
              id, ACE_TEXT_CHAR_TO_TCHAR(location)));

  ACE_Log_Msg::instance ()->release ();
}

TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : tag_ (tag)
  , orb_core_ (orb_core)
  , cache_map_entry_ (0)
  , bidirectional_flag_ (-1)
  , opening_connection_role_ (TAO::TAO_UNSPECIFIED_ROLE)
  , head_ (0)
  , tail_ (0)
  , incoming_message_queue_ (orb_core)
  , current_message_ (0)
  , current_deadline_ (ACE_Time_Value::zero)
  , flush_timer_id_ (-1)
  , transport_timer_ (this)
  , handler_lock_ (orb_core->resource_factory ()->create_cached_connection_lock ())
  , id_ ((size_t) this)
  , purging_order_ (0)
  , recv_buffer_size_ (0)
  , sent_byte_count_ (0)
  , is_connected_ (false)
  , char_translator_ (0)
  , wchar_translator_ (0)
  , tcs_set_ (0)
  , first_request_ (1)
{
  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);
}

TAO_Transport::~TAO_Transport (void)
{
  delete this->ws_;

  delete this->tms_;

  delete this->handler_lock_;

  if (!this->is_connected_)
    {
      // When we have a not connected transport we could have buffered
      // messages on this transport which we have to cleanup now.
      this->cleanup_queue_i();

      // Cleanup our cache entry
      this->purge_entry();
    }

  // By the time the destructor is reached here all the connection stuff
  // *must* have been cleaned up.
  ACE_ASSERT (this->head_ == 0);
  ACE_ASSERT (this->cache_map_entry_ == 0);
}

void
TAO_Transport::provide_handler (TAO_Connection_Handler_Set &handlers)
{
  this->add_reference ();
  handlers.insert (this->connection_handler_i ());
}

bool
TAO_Transport::idle_after_send (void)
{
  return this->tms ()->idle_after_send ();
}

bool
TAO_Transport::idle_after_reply (void)
{
  return this->tms ()->idle_after_reply ();
}

int
TAO_Transport::tear_listen_point_list (TAO_InputCDR &)
{
  ACE_NOTSUP_RETURN (-1);
}

void
TAO_Transport::close_connection (void)
{
  this->connection_handler_i ()->close_connection ();
}

int
TAO_Transport::register_handler (void)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::register_handler\n"),
                  this->id ()));
    }

  ACE_Reactor *r = this->orb_core_->reactor ();

  // @@note: This should be okay since the register handler call will
  // not make a nested call into the transport.
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->handler_lock_,
                    false);

  if (r == this->event_handler_i ()->reactor ())
    {
      return 0;
    }

  // Set the flag in the Connection Handler and in the Wait Strategy
  // @@Maybe we should set these flags after registering with the
  // reactor. What if the  registration fails???
  this->ws_->is_registered (1);

  // Register the handler with the reactor
  return r->register_handler (this->event_handler_i (),
                              ACE_Event_Handler::READ_MASK);
}

int
TAO_Transport::generate_locate_request (
    TAO_Target_Specification &spec,
    TAO_Operation_Details &opdetails,
    TAO_OutputCDR &output)
{
  if (this->messaging_object ()->generate_locate_request_header (opdetails,
                                                                 spec,
                                                                 output)
       == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Transport[%d]::generate_locate_request, ")
                    ACE_TEXT("error while marshalling the LocateRequest header\n"),
                    this->id ()));

      return -1;
    }

  return 0;
}

int
TAO_Transport::generate_request_header (
    TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &output)
{
  // codeset service context is only supposed to be sent in the first request
  // on a particular connection.
  TAO_Codeset_Manager *csm = this->orb_core()->codeset_manager();
  if (csm && this->first_request_)
    csm->generate_service_context( opdetails, *this );

  if (this->messaging_object ()->generate_request_header (opdetails,
                                                          spec,
                                                          output) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) - Transport[%d]::generate_request_header, ")
                    ACE_TEXT("error while marshalling the Request header\n"),
                    this->id()));

      return -1;
    }

  return 0;
}

/// @todo Ideally the following should be inline.
/// @todo purge_entry has a return value, use it
int
TAO_Transport::recache_transport (TAO_Transport_Descriptor_Interface *desc)
{
  // First purge our entry
  this->purge_entry ();

  // Then add ourselves to the cache
  return this->transport_cache_manager ().cache_transport (desc,
                                                           this);
}

int
TAO_Transport::purge_entry (void)
{
  return this->transport_cache_manager ().purge_entry (this->cache_map_entry_);
}

int
TAO_Transport::make_idle (void)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::make_idle\n",
                  this->id ()));
    }

  return this->transport_cache_manager ().make_idle (this->cache_map_entry_);
}

int
TAO_Transport::update_transport (void)
{
  return this->transport_cache_manager ().update_entry (this->cache_map_entry_);
}

/*
 *
 *  Methods called and used in the output path of the ORB.
 *
 */
int
TAO_Transport::handle_output (void)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_output\n"),
                  this->id ()));
    }

  // The flushing strategy (potentially via the Reactor) wants to send
  // more data, first check if there is a current message that needs
  // more sending...
  int retval = this->drain_queue ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_output, ")
                  ACE_TEXT("drain_queue returns %d/%d\n"),
                  this->id (),
                  retval, errno));
    }

  // Any errors are returned directly to the Reactor
  return retval;
}

int
TAO_Transport::format_queue_message (TAO_OutputCDR &stream)
{
  if (this->messaging_object ()->format_message (stream) != 0)
    return -1;

  return this->queue_message_i (stream.begin());
}

int
TAO_Transport::send_message_block_chain (const ACE_Message_Block *mb,
                                         size_t &bytes_transferred,
                                         ACE_Time_Value *max_wait_time)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

  return this->send_message_block_chain_i (mb,
                                           bytes_transferred,
                                           max_wait_time);
}

int
TAO_Transport::send_message_block_chain_i (const ACE_Message_Block *mb,
                                           size_t &bytes_transferred,
                                           ACE_Time_Value *)
{
  const size_t total_length = mb->total_length ();

  // We are going to block, so there is no need to clone
  // the message block.
  TAO_Synch_Queued_Message synch_message (mb);

  synch_message.push_back (this->head_, this->tail_);

  int n = this->drain_queue_i ();

  if (n == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      return -1; // Error while sending...
    }
  else if (n == 1)
    {
      ACE_ASSERT (synch_message.all_data_sent ());
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      bytes_transferred = total_length;
      return 1;  // Empty queue, message was sent..
    }

  ACE_ASSERT (n == 0); // Some data sent, but data remains.

  // Remove the temporary message from the queue...
  synch_message.remove_from_list (this->head_, this->tail_);

  bytes_transferred =
    total_length - synch_message.message_length ();

  return 0;
}

int
TAO_Transport::send_message_shared (TAO_Stub *stub,
                                    int message_semantics,
                                    const ACE_Message_Block *message_block,
                                    ACE_Time_Value *max_wait_time)
{
  int result;

  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

    result =
      this->send_message_shared_i (stub, message_semantics,
                                   message_block, max_wait_time);
  }

  if (result == -1)
    {
      this->close_connection ();
    }

  return result;
}

int
TAO_Transport::send_synchronous_message_i (const ACE_Message_Block *mb,
                                           ACE_Time_Value *max_wait_time)
{
  // We are going to block, so there is no need to clone
  // the message block.
  TAO_Synch_Queued_Message synch_message (mb);

  synch_message.push_back (this->head_, this->tail_);

  int n =
    this->send_synch_message_helper_i (synch_message,
                                       max_wait_time);

  if (n == -1 || n == 1)
    {
      return n;
    }

  ACE_ASSERT (n == 0);

  // @todo: Check for timeouts!
  // if (max_wait_time != 0 && errno == ETIME) return -1;
  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();
  (void) flushing_strategy->schedule_output (this);

  // Release the mutex, other threads may modify the queue as we
  // block for a long time writing out data.
  int result;
  {
    typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
    TAO_REVERSE_LOCK reverse (*this->handler_lock_);
    ACE_GUARD_RETURN (TAO_REVERSE_LOCK,
                      ace_mon,
                      reverse,
                      -1);

    result = flushing_strategy->flush_message (this,
                                               &synch_message,
                                               max_wait_time);
  }
  if (result == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);

      if (errno == ETIME)
        {
          if (this->head_ == &synch_message)
            {
              // This is a timeout, there is only one nasty case: the
              // message has been partially sent!  We simply cannot take
              // the message out of the queue, because that would corrupt
              // the connection.
              //
              // What we do is replace the queued message with an
              // asynchronous message, that contains only what remains of
              // the timed out request.  If you think about sending
              // CancelRequests in this case: there is no much point in
              // doing that: the receiving ORB would probably ignore it,
              // and figuring out the request ID would be a bit of a
              // nightmare.
              //

              synch_message.remove_from_list (this->head_, this->tail_);
              TAO_Queued_Message *queued_message = 0;
              ACE_NEW_RETURN (queued_message,
                              TAO_Asynch_Queued_Message (
                                  synch_message.current_block (),
                                  0,
                                  1),
                              -1);
              queued_message->push_front (this->head_, this->tail_);
            }
        }

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_synchronous_message_i, ")
                      ACE_TEXT("error while flushing message %p\n"),
                      this->id (), ""));
        }

      return -1;
    }

  else
    {
      ACE_ASSERT (synch_message.all_data_sent () != 0);
    }

  ACE_ASSERT (synch_message.next () == 0);
  ACE_ASSERT (synch_message.prev () == 0);
  return 1;
}


int
TAO_Transport::send_reply_message_i (const ACE_Message_Block *mb,
                                     ACE_Time_Value *max_wait_time)
{
  // Dont clone now.. We could be sent in one shot!
  TAO_Synch_Queued_Message synch_message (mb);

  synch_message.push_back (this->head_,
                           this->tail_);

  int n =
    this->send_synch_message_helper_i (synch_message,
                                       max_wait_time);

  if (n == -1 || n == 1)
    {
      return n;
    }

  ACE_ASSERT (n == 0);

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_reply_message_i, ")
                  ACE_TEXT("preparing to add to queue before leaving \n"),
                  this->id ()));
    }

  // Till this point we shouldnt have any copying and that is the
  // point anyway. Now, remove the node from the list
  synch_message.remove_from_list (this->head_,
                                  this->tail_);

  // Clone the node that we have.
  TAO_Queued_Message *msg =
    synch_message.clone (this->orb_core_->transport_message_buffer_allocator ());

  // Stick it in the queue
  msg->push_back (this->head_,
                  this->tail_);

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();

  (void) flushing_strategy->schedule_output (this);

  return 1;
}

int
TAO_Transport::send_synch_message_helper_i (TAO_Synch_Queued_Message &synch_message,
                                            ACE_Time_Value * /*max_wait_time*/)
{
  // @@todo: Need to send timeouts for writing..
  int n = this->drain_queue_i ();

  if (n == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      return -1; // Error while sending...
    }
  else if (n == 1)
    {
      ACE_ASSERT (synch_message.all_data_sent ());
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      return 1;  // Empty queue, message was sent..
    }

  ACE_ASSERT (n == 0); // Some data sent, but data remains.

  if (synch_message.all_data_sent ())
    {
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      return 1;
    }

  return 0;
}

int
TAO_Transport::queue_is_empty_i (void)
{
  return (this->head_ == 0);
}


int
TAO_Transport::schedule_output_i (void)
{
  ACE_Event_Handler *eh = this->event_handler_i ();
  ACE_Reactor *reactor = eh->reactor ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::schedule_output\n"),
                  this->id ()));
    }

  return reactor->schedule_wakeup (eh, ACE_Event_Handler::WRITE_MASK);
}

int
TAO_Transport::cancel_output_i (void)
{
  ACE_Event_Handler *eh = this->event_handler_i ();
  ACE_Reactor *reactor = eh->reactor ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::cancel_output\n"),
                  this->id ()));
    }

  return reactor->cancel_wakeup (eh, ACE_Event_Handler::WRITE_MASK);
}

int
TAO_Transport::handle_timeout (const ACE_Time_Value & /* current_time */,
                               const void *act)
{
  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - TAO_Transport[%d]::handle_timeout, ")
                  ACE_TEXT("timer expired\n"),
                  this->id ()));
    }

  /// This is the only legal ACT in the current configuration....
  if (act != &this->current_deadline_)
    {
      return -1;
    }

  if (this->flush_timer_pending ())
    {
      // The timer is always a oneshot timer, so mark is as not
      // pending.
      this->reset_flush_timer ();

      TAO_Flushing_Strategy *flushing_strategy =
        this->orb_core ()->flushing_strategy ();
      (void) flushing_strategy->schedule_output (this);
    }

  return 0;
}

int
TAO_Transport::drain_queue (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

  int retval = this->drain_queue_i ();

  if (retval == 1)
    {
      // ... there is no current message or it was completely
      // sent, cancel output...
      TAO_Flushing_Strategy *flushing_strategy =
        this->orb_core ()->flushing_strategy ();

      flushing_strategy->cancel_output (this);

      return 0;
    }

  return retval;
}

int
TAO_Transport::drain_queue_helper (int &iovcnt, iovec iov[])
{
  size_t byte_count = 0;

  // ... send the message ...
  ssize_t retval =
    this->send (iov, iovcnt, byte_count);

  if (TAO_debug_level == 5)
    {
      dump_iov (iov, iovcnt, this->id (),
                byte_count, "drain_queue_helper");
    }

  // ... now we need to update the queue, removing elements
  // that have been sent, and updating the last element if it
  // was only partially sent ...
  this->cleanup_queue (byte_count);
  iovcnt = 0;

  if (retval == 0)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
                      ACE_TEXT("send() returns 0"),
                      this->id ()));
        }
      return -1;
    }
  else if (retval == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
                      ACE_TEXT("error during %p\n"),
                      this->id (), "send()"));
        }

      if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
          return 0;
        }

      return -1;
    }

  // ... start over, how do we guarantee progress?  Because if
  // no bytes are sent send() can only return 0 or -1
  ACE_ASSERT (byte_count != 0);

  // Total no. of bytes sent for a send call
  this->sent_byte_count_ += byte_count;

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
                  ACE_TEXT("byte_count = %d, head_is_empty = %d\n"),
                  this->id(), byte_count, (this->head_ == 0)));
    }

  return 1;
}

int
TAO_Transport::drain_queue_i (void)
{
  // This is the vector used to send data, it must be declared outside
  // the loop because after the loop there may still be data to be
  // sent
  int iovcnt = 0;
  iovec iov[ACE_IOV_MAX];

  // We loop over all the elements in the queue ...
  TAO_Queued_Message *i = this->head_;

  // reset the value so that the counting is done for each new send
  // call.
  this->sent_byte_count_ = 0;

  while (i != 0)
    {
      // ... each element fills the iovector ...
      i->fill_iov (ACE_IOV_MAX, iovcnt, iov);

      // ... the vector is full, no choice but to send some data out.
      // We need to loop because a single message can span multiple
      // IOV_MAX elements ...
      if (iovcnt == ACE_IOV_MAX)
        {
          int retval =
            this->drain_queue_helper (iovcnt, iov);

          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
                          ACE_TEXT("helper retval = %d\n"),
                          this->id (), retval));
            }

          if (retval != 1)
            {
              return retval;
            }

          i = this->head_;
          continue;
        }
      // ... notice that this line is only reached if there is still
      // room in the iovector ...
      i = i->next ();
    }


  if (iovcnt != 0)
    {
      int retval = this->drain_queue_helper (iovcnt, iov);

          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
                          ACE_TEXT("helper retval = %d\n"),
                          this->id (), retval));
            }

      if (retval != 1)
        {
          return retval;
        }
    }

  if (this->head_ == 0)
    {
      if (this->flush_timer_pending ())
        {
          ACE_Event_Handler *eh = this->event_handler_i ();
          ACE_Reactor *reactor = eh->reactor ();
          reactor->cancel_timer (this->flush_timer_id_);
          this->reset_flush_timer ();
        }

      return 1;
    }

  return 0;
}

void
TAO_Transport::cleanup_queue_i ()
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::cleanup_queue_i, "
                  "cleaning up complete queue\n",
                  this->id ()));
    }

  // Cleanup all messages
  while (this->head_ != 0)
    {
      TAO_Queued_Message *i = this->head_;

       // @@ This is a good point to insert a flag to indicate that a
       //    CloseConnection message was successfully received.
      i->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);

      i->remove_from_list (this->head_, this->tail_);

      i->destroy ();
   }
}

void
TAO_Transport::cleanup_queue (size_t byte_count)
{
  while (this->head_ != 0 && byte_count > 0)
    {
      TAO_Queued_Message *i = this->head_;

      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::cleanup_queue, ")
                      ACE_TEXT("byte_count = %d\n"),
                      this->id (), byte_count));
        }

      // Update the state of the first message
      i->bytes_transferred (byte_count);

      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::cleanup_queue, ")
                      ACE_TEXT("after transfer, bc = %d, all_sent = %d, ml = %d\n"),
                      this->id (), byte_count, i->all_data_sent (),
                      i->message_length ()));
        }

      // ... if all the data was sent the message must be removed from
      // the queue...
      if (i->all_data_sent ())
        {
          i->remove_from_list (this->head_, this->tail_);
          i->destroy ();
        }
    }
}

int
TAO_Transport::check_buffering_constraints_i (TAO_Stub *stub,
                                              int &must_flush)
{
  // First let's compute the size of the queue:
  size_t msg_count = 0;
  size_t total_bytes = 0;

  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      msg_count++;
      total_bytes += i->message_length ();
    }

  int set_timer;
  ACE_Time_Value new_deadline;

  int constraints_reached =
    stub->sync_strategy ().buffering_constraints_reached (stub,
                                                          msg_count,
                                                          total_bytes,
                                                          must_flush,
                                                          this->current_deadline_,
                                                          set_timer,
                                                          new_deadline);

  // ... set the new timer, also cancel any previous timers ...
  if (set_timer)
    {
      ACE_Event_Handler *eh = this->event_handler_i ();
      ACE_Reactor *reactor = eh->reactor ();
      this->current_deadline_ = new_deadline;
      ACE_Time_Value delay =
        new_deadline - ACE_OS::gettimeofday ();

      if (this->flush_timer_pending ())
        {
          reactor->cancel_timer (this->flush_timer_id_);
        }

      this->flush_timer_id_ =
        reactor->schedule_timer (&this->transport_timer_,
                                 &this->current_deadline_,
                                 delay);
    }

  return constraints_reached;
}

void
TAO_Transport::report_invalid_event_handler (const char *caller)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::report_invalid_event_handler")
                  ACE_TEXT("(%s) no longer associated with handler [tag=%d]\n"),
                  this->id (), caller, this->tag_));
    }
}

void
TAO_Transport::send_connection_closed_notifications (void)
{
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->handler_lock_));

    this->send_connection_closed_notifications_i ();
  }

  this->tms ()->connection_closed ();
}

void
TAO_Transport::send_connection_closed_notifications_i (void)
{
  this->cleanup_queue_i ();

  this->messaging_object ()->reset ();
}

int
TAO_Transport::send_message_shared_i (TAO_Stub *stub,
                                      int message_semantics,
                                      const ACE_Message_Block *message_block,
                                      ACE_Time_Value *max_wait_time)
{

// @todo Bala mentioned that this has to go out here
// {
  if (message_semantics == TAO_Transport::TAO_TWOWAY_REQUEST)
    {
      return this->send_synchronous_message_i (message_block,
                                               max_wait_time);
    }
  else if (message_semantics == TAO_Transport::TAO_REPLY)
    {
      return this->send_reply_message_i (message_block,
                                         max_wait_time);
    }
 // }

  // Let's figure out if the message should be queued without trying
  // to send first:
  int try_sending_first = 1;

  const int queue_empty = (this->head_ == 0);

  if (!queue_empty)
    {
      try_sending_first = 0;
    }
  else if (stub->sync_strategy ().must_queue (queue_empty))
    {
      try_sending_first = 0;
    }

  ssize_t n;

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();

  if (try_sending_first)
    {
      size_t byte_count = 0;
      // ... in this case we must try to send the message first ...

      const size_t total_length = message_block->total_length ();

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_message_i, ")
                      ACE_TEXT("trying to send the message (ml = %d)\n"),
                      this->id (), total_length));
        }

      // @@ I don't think we want to hold the mutex here, however if
      // we release it we need to recheck the status of the transport
      // after we return... once I understand the final form for this
      // code I will re-visit this decision
      n = this->send_message_block_chain_i (message_block,
                                            byte_count,
                                            max_wait_time);
      if (n == -1)
        {
          // ... if this is just an EWOULDBLOCK we must schedule the
          // message for later, if it is ETIME we still have to send
          // the complete message, because cutting off the message at
          // this point will destroy the synchronization with the
          // server ...
          if (errno != EWOULDBLOCK && errno != ETIME)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_message_i, ")
                              ACE_TEXT("fatal error in ")
                              ACE_TEXT("send_message_block_chain_i %p\n"),
                              this->id (), ""));
                }
              return -1;
            }
        }

      // ... let's figure out if the complete message was sent ...
      if (total_length == byte_count)
        {
          // Done, just return.  Notice that there are no allocations
          // or copies up to this point (though some fancy calling
          // back and forth).
          // This is the common case for the critical path, it should
          // be fast.
          return 0;
        }

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_message_i, ")
                      ACE_TEXT("partial send %d / %d bytes\n"),
                      this->id (), byte_count, total_length));
        }

      // ... part of the data was sent, need to figure out what piece
      // of the message block chain must be queued ...
      while (message_block != 0 && message_block->length () == 0)
        {
          message_block = message_block->cont ();
        }

      // ... at least some portion of the message block chain should
      // remain ...
      ACE_ASSERT (message_block != 0);
    }

  // ... either the message must be queued or we need to queue it
  // because it was not completely sent out ...

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::send_message_i, ")
                  ACE_TEXT("message is queued\n"),
                  this->id ()));
    }

  if (this->queue_message_i(message_block) == -1)
  {
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - Transport[%d]::send_message_shared_i, "
                "cannot queue message for "
                " - %m\n",
                this->id ()));
    return -1;
  }

  // ... if the queue is full we need to activate the output on the
  // queue ...
  int must_flush = 0;
  const int constraints_reached =
    this->check_buffering_constraints_i (stub,
                                         must_flush);

  // ... but we also want to activate it if the message was partially
  // sent.... Plus, when we use the blocking flushing strategy the
  // queue is flushed as a side-effect of 'schedule_output()'

  if (constraints_reached || try_sending_first)
    {
      (void) flushing_strategy->schedule_output (this);
    }

  if (must_flush)
    {
      typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
      TAO_REVERSE_LOCK reverse (*this->handler_lock_);
      ACE_GUARD_RETURN (TAO_REVERSE_LOCK, ace_mon, reverse, -1);

      (void) flushing_strategy->flush_transport (this);
    }

  return 0;
}

int
TAO_Transport::queue_message_i(const ACE_Message_Block *message_block)
{
  TAO_Queued_Message *queued_message = 0;
  ACE_NEW_RETURN (queued_message,
                  TAO_Asynch_Queued_Message (message_block,
                                             0,
                                             1),
                  -1);
  queued_message->push_back (this->head_, this->tail_);

  return 0;
}

/*
 *
 * All the methods relevant to the incoming data path of the ORB are
 * defined below
 *
 */
int
TAO_Transport::handle_input (TAO_Resume_Handle &rh,
                             ACE_Time_Value * max_wait_time,
                             int block)
{
  ACE_UNUSED_ARG (block);

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                  ACE_TEXT("the size of the queue is [%d]\n"),
                  this->id (),
                  this->incoming_message_queue_.queue_length()));
    }

  // First try to process messages off the head of the incoming queue.
  int retval = this->process_queue_head (rh);
  if (retval <= 0)
    {
      if (retval == -1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                        ACE_TEXT("error while parsing the head of the queue\n"),
                        this->id()));
        }

      return retval;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                  ACE_TEXT("nothing to process on queue, read from handle.\n"),
                  this->id ()));
    }

  //
  // Loop while reads are incomplete and retries remain.
  //
  int  rereads = TAO_MAX_TRANSPORT_REREAD_ATTEMPTS ;
                                  // Attempts to complete a read.
  bool partial = true ;           // Incomplete read.
  bool disconnect = false ;       // Problem encountered.
  bool message_enqueued = false ; // A message has been queued.
  while( partial == true && rereads-- >= 0 && disconnect == false) {
    //
    // Create the input buffer.  The actual buffer and the data
    // block that manages it are created on the heap and orphaned to
    // the processing code.  The message block that we use to walk
    // through the entire set of data read from the handle is on the
    // stack and reinitialized each pass through the loop here.  As it
    // goes out of scope at the end of loop processing, it release()es
    // the contained data block and buffer.  If there were problems
    // and no other references to the data was created during the loop,
    // this then frees those resources.  Cool, huh.
    //
    ACE_Data_Block* data = 0 ;
    ACE_NEW_MALLOC_RETURN (
      data,
      (ACE_Data_Block*) this->orb_core_
                            ->input_cdr_dblock_allocator()
                            ->malloc( sizeof(ACE_Data_Block)),
      ACE_Data_Block(
        4*TAO_MAXBUFSIZE,             // S/B tunable
        ACE_Message_Block::MB_DATA,
        0,                            // Create new buffer on heap
        this->orb_core_->input_cdr_buffer_allocator (),
        this->orb_core_->locking_strategy (),
        0,                            // flags -- on the heap and fully owned!
        this->orb_core_->input_cdr_dblock_allocator()
      ),
      -1
    ) ;


    //
    // Pass in the allocator as a courtesy to any duplicates on the
    // heap.
    //
    ACE_Message_Block whole_block(
      data,// ->duplicate(), // From above.
      0,    // flags -- on the heap and fully owned!
      this->orb_core_->input_cdr_msgblock_allocator ()
    ) ;
    ACE_CDR::mb_align( &whole_block) ;

    //
    // Greedy read - always read as much as possible.
    //
    ssize_t data_left_in_buffer
            = this->recv(
                whole_block.wr_ptr(),
                whole_block.space(),
                max_wait_time
              ) ;
    if( data_left_in_buffer > 0) {
      whole_block.wr_ptr( data_left_in_buffer) ;

      if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                    ACE_TEXT("read %d bytes from handle.\n"),
                    this->id (), data_left_in_buffer));
      }

      if (TAO_debug_level >= 10)
      {
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char *) whole_block.rd_ptr(),
                       data_left_in_buffer,
                       ACE_TEXT ("handle data")));
      }

    } else {
      if( errno != EWOULDBLOCK && errno != EAGAIN) {
        //
        // Error during read operation.
        //
        disconnect = true  ;

// #define NO_RETRY_ON_EMPTY_HANDLE
#ifdef NO_RETRY_ON_EMPTY_HANDLE
      } else {
        //
        // @todo: Determine the desired action here -- ask Chris.
        //
        partial = false ;
#endif // NO_RETRY_ON_EMPTY_HANDLE

      }

        //
        // Try again or give up.
        //
        continue ;
    }

    //
    // Process the entire buffer.
    //
    while( data_left_in_buffer > 0) {
      //
      // Create a message block to reference the current portion of
      // the header that we are reading.
      //
      ACE_Message_Block* current_block ;
      ACE_NEW_MALLOC_RETURN (
        current_block,
        (ACE_Message_Block*) this->orb_core_
                                 ->input_cdr_msgblock_allocator()
                                 ->malloc( sizeof(ACE_Message_Block)),
        ACE_Message_Block(
          data->duplicate(), // From above.
          0,    // flags -- on the heap and fully owned!
          this->orb_core_->input_cdr_msgblock_allocator ()
        ),
        -1
      ) ;
      current_block->rd_ptr( whole_block.rd_ptr()) ;

      //
      // Check current state of header processing.
      //
      ssize_t wanted_size
              = this->messaging_object()->header_length() ;
      if( this->current_message_ != 0) {
        //
        // Only need the remainder.  If we have a full header already,
        // this will allow us to bypass further header processing.
        // Let the value go under 0 if we are receiving data, since
        // the header read is guarded by checking for over 0 bytes to
        // go in the header.
        //
        wanted_size
          -= this->current_message_->msg_block_->total_length() ;
              }

              //
      // Only process header information if we need to.
      //
      if( 0 < wanted_size) {
        if( data_left_in_buffer < wanted_size) {
          //
          // A partial header was read.  Adjust the current read pointer
          // in the buffer as well as the header block write pointer.
          //
          partial = true ;
          whole_block.rd_ptr( data_left_in_buffer) ;
          current_block->wr_ptr( whole_block.rd_ptr()) ;
          data_left_in_buffer = 0 ;

        } else {
          //
          // A complete header was read.  Make sure that the header
          // chain is correct.
          //
          partial = false ;
          whole_block.rd_ptr( wanted_size) ;
          current_block->wr_ptr( whole_block.rd_ptr()) ;
          data_left_in_buffer -= wanted_size ;
        }

        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
              ACE_TEXT("processed %d header bytes, header is %s.\n"),
              this->id (), current_block->length(),
              (partial == true) ? ACE_TEXT("incomplete") : ACE_TEXT("complete")
            ));
          }

        if (TAO_debug_level >= 10)
          {
            ACE_HEX_DUMP ((LM_DEBUG,
                          (const char *) current_block->rd_ptr(),
                          current_block->length(),
                          ACE_TEXT ("handle data")));
          }

        //
        // Install the new block appropriately.
        //
        if( this->current_message_ == 0) {
          //
          // This is the start of a new message, create the structures
          // to handle the message and load 'em up.  We pass ownership
          // of the current ACE_Message_Block to the TAO_Queued_Data
          // object, since we will only refer to it via that object
          // and it will persist beyond the current scope.
          //
          this->current_message_
            = TAO_Queued_Data::make_minimal_message( current_block) ;
          if( this->current_message_ == 0) {
            //
            // Problem creating data structures, panic.
            //
            disconnect = true ;
            break ; // Leave the inner loop and hit the outer loop
                    // conditional directly.
          }

        } else {
          //
          // This is another block in a header chain.
          //
          ACE_Message_Block* location = this->current_message_->msg_block_ ;
          while( location->cont() != 0) {
            location = location->cont() ;
            }
          if( current_block->rd_ptr() == location->wr_ptr()) {
            //
            // Current data follows the previous in the same buffer,
            // so we just need to adjust the existing block and release
            // the new one.
            //
            location->wr_ptr( current_block->length()) ;
            current_block->release() ;

          } else {
            //
            // Data is in a different buffer, need to chain this on.
            //
            location->cont( current_block) ;
          }
        }

        //
        // Process the new header data.
        //
        if( partial == true) {
          //
          // We read a partial header, so make sure that the queued
          // data state reflects this.
          //
          this->current_message_->current_state_
            = TAO_Queued_Data::WAITING_TO_COMPLETE_HEADER ;

        } else {
          //
          // We just finished a header, go ahead and parse it to
          // extract the message length.
          //

          //
          // Ensure that we have a contiguous aligned header to parse.
          //
          if( this->current_message_->msg_block_->cont() != 0) {
            //
            // This is inefficient but the entire set of code for
            // handling the header (and indeed the entire message)
            // assumes that we have a single, aligned, contiguous
            // buffer.  Yuk.
            //
            current_block = this->current_message_->msg_block_ ;
            ACE_NEW_MALLOC_RETURN (
              this->current_message_->msg_block_,
              (ACE_Message_Block*) this->orb_core_
                                       ->input_cdr_msgblock_allocator()
                                       ->malloc( sizeof(ACE_Message_Block)),
              ACE_Message_Block(
                this->orb_core_->input_cdr_msgblock_allocator ()
              ),
              -1
            ) ;
            ACE_CDR::consolidate(
              this->current_message_->msg_block_,
              current_block
            ) ;
            current_block->release() ;
          }

          //
          // We have a complete, aligned and contiguous, header at
          // this point, so go ahead and process it.
          //
          // This delegates the actual parsing actions to the protocol
          // objects.
          //
          if( 0 == this->messaging_object()->check_for_valid_header(
                     *this->current_message_->msg_block_
                   )) {
            //
            // Invalid header, panic.
            //
            disconnect = true ;
            break ; // Leave the inner loop and hit the outer loop
                    // conditional directly.
            }

          //
          // This is copied from the original
          // TAO_Queued_Data::make_{in}complete_message() factory
          // methods.
          //
          this->messaging_object()->set_queued_data_from_message_header(
            this->current_message_, *this->current_message_->msg_block_
          ) ;
          if( this->current_message_->current_state_
              == TAO_Queued_Data::INVALID) {
            //
            // Could not understand header, panic.
            //
            disconnect = true ;
            break ; // Leave the inner loop and hit the outer loop
                    // conditional directly.
            }

          //
          // Update queued data state.  We have not extracted any
          // payload bytes from the input buffer yet, so we do not
          // have to adjust any of the internal values.
          //
          this->current_message_->current_state_
            = TAO_Queued_Data::WAITING_TO_COMPLETE_PAYLOAD ;
        }

        //
        // At this point, we have already consumed the new
        // ACE_Message_Block pointing into the data that we created
        // at the top of the loop, so we return to the top to initialize
        // another one.
        //
        continue ;
    }

      //
      // We have a valid message and parsed header!  This means
      // that we are just receiving a message at this point.
      // The TAO_Queued_Data::missing_data_bytes_ member is initialized
      // to the GIOP message payload size.  We adjust it to show the
      // remaining data to read for the current message since we do
      // not want to change the semantics of its use, although it
      // would be just as easy to leave the payload size alone.
      //

      //
      // Update the message to end at the end of the current segment
      // of the message.
      //
      wanted_size = this->current_message_->missing_data_bytes_ ;
      if( data_left_in_buffer < wanted_size) {
        //
        // This is a partial read of message data, store it, chain it,
        // and move on.
        //
        partial = true ;
        whole_block.rd_ptr( data_left_in_buffer) ;
        current_block->wr_ptr( whole_block.rd_ptr()) ;
        this->current_message_->missing_data_bytes_ -= data_left_in_buffer ;
        data_left_in_buffer = 0 ;

      } else {
        //
        // We completed a message, pass it on.
        //
        partial = false ;
        whole_block.rd_ptr( wanted_size) ;
        current_block->wr_ptr( whole_block.rd_ptr()) ;
        data_left_in_buffer -= wanted_size ;
        this->current_message_->missing_data_bytes_ = 0 ;
        this->current_message_->current_state_ = TAO_Queued_Data::COMPLETED ;
      }

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                ACE_TEXT("grabbed another %d data bytes into current message.  ")
                ACE_TEXT("%d bytes left in buffer for next message.  ")
                ACE_TEXT("%d bytes left to read to complete message.\n"),
                this->id (), current_block->length(), data_left_in_buffer,
                this->current_message_->missing_data_bytes_
                     ));
        }

      if (TAO_debug_level >= 10)
        {
          ACE_HEX_DUMP ((LM_DEBUG,
                        (const char *) current_block->rd_ptr(),
                        current_block->length(),
                        ACE_TEXT ("handle data")));
        }

      //
      // Install the current block into the message.
      //
      ACE_Message_Block* location = this->current_message_->msg_block_ ;
      while( location->cont() != 0) {
        location = location->cont() ;
      }
      if( current_block->rd_ptr() == location->wr_ptr()
          && location != this->current_message_->msg_block_
        ) {
        //
        // Current data follows the previous in the same buffer,
        // so we just need to adjust the existing block and release
        // the new one.
        //
        // Note that we do _not_ coallesce the first message data block
        // into the message header block.  This allows us to easily
        // discard the header once we have received the entire message.
        // The test works since we know that we have consolidated the
        // header into a single message block in order to parse it.
        //
        location->wr_ptr( current_block->length()) ;
        current_block->release() ;

      } else {
        //
        // Data is in a different buffer, need to chain this on.
        //
        location->cont( current_block) ;
      }

      //
      // Enqueue any completed message.
      //
      if( this->current_message_->current_state_
          == TAO_Queued_Data::COMPLETED
        ) {
        //
        // Remove and release the header from the queued message.
        // This works since we are assured that the header has been
        // consolidated in order to have been parsed earlier.
        //
        current_block = this->current_message_->msg_block_ ;
        this->current_message_->msg_block_
          = this->current_message_->msg_block_->cont() ;
        current_block->cont( 0) ;
        current_block->release() ;

        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
                ACE_TEXT("enqueueing message for processing.\n"),
                this->id ()));
            ACE_DEBUG ((LM_DEBUG,"---Data block ref count: %d\n",
                       this->current_message_
                           ->msg_block_
                           ->data_block()
                           ->reference_count()
                      )) ;
          }

        if (TAO_debug_level >= 10)
          {
            this->current_message_->dump_msg( "enqueueing") ;
          }

        //
        // Enqueue the current message.
        //
        this->enqueue_incoming_message( this->current_message_) ;
        this->current_message_ = 0 ;
        message_enqueued = true ;
      }
    }
  }

  //
  // Post read processing - manage message processing, resume handle
  //                        and disconnecting.
  //
  if( disconnect) {
    this->enqueue_incoming_message (
      TAO_Queued_Data::make_close_connection ()
    ) ;
    message_enqueued = true ; // ??? or not ???

  } else if( partial) {
      rh.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
  }

  //
  // @todo: Need to determine whether to process the queue head if we
  //        are left with a partial message.  If we have a partial
  //        message, we will process the messages on the queue, then
  //        wait for more input.  This is regardless of wheter we have a
  //        partial or not -- not sure if the calling code needs to know
  //        that we have a partial message waiting to complete or not.
  //
  int processing_results = 0 ;
  if( (message_enqueued == true) ){// && (partial == false)) {
                                    // because of return val?
    processing_results = this->process_queue_head( rh) ;
  }

  if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("TAO (%P|%t) - Transport[%d]::handle_input, ")
            ACE_TEXT("returning with disconnect == %d and partial == %d and processing_results == %d\n"),
            this->id (), disconnect, partial, processing_results));
        }

  //
  // Return an appropriate value.
  //
  return disconnect? -1: partial? 1: processing_results ;

}

int
TAO_Transport::enqueue_incoming_message (TAO_Queued_Data *queueable_message)
{
  // Get the GIOP version
  CORBA::Octet major  = queueable_message->major_version_;
  CORBA::Octet minor  = queueable_message->minor_version_;
  CORBA::UShort whole = major << 8 | minor;

  // Set up a couple of pointers that are shared by the code
  // for the different GIOP versions.
  ACE_Message_Block *mb = 0;
  TAO_Queued_Data *fragment_message = 0;

  //
  // Peek at the request ID since we may need that to find any
  // previous fragments.
  //
  this->messaging_object()->set_request_id_from_peek(
                              queueable_message,
                              queueable_message->msg_block_
                            );

  switch(whole)
    {
    case 0x0100:  // GIOP 1.0
      if (!queueable_message->more_fragments_)
        return this->incoming_message_queue_.enqueue_tail (
                                                  queueable_message);

      // Fragments aren't supported in 1.0.  This is an error and
      // we should reject it somehow.  What do we do here?  Do we throw
      // an exception to the receiving side?  Do we throw an exception
      // to the sending side?
      //
      // At the very least, we need to log the fact that we received
      // nonsense.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("TAO (%P|%t) - ")
                         ACE_TEXT("TAO_Transport::enqueue_incoming_message ")
                         ACE_TEXT("detected a fragmented GIOP 1.0 message\n")),
                        -1);
      break;
    case 0x0101:  // GIOP 1.1
      // In 1.1, fragments kinda suck because they don't have they're
      // own message-specific header.  Therefore, we have to do the
      // following:
      fragment_message =
          this->incoming_message_queue_.find_fragment (major, minor);

      // No fragment was found
      if (fragment_message == 0)
        return this->incoming_message_queue_.enqueue_tail (
                                                queueable_message);

      if (queueable_message->more_fragments_)
        {
          // Find the last message block in the continuation
          mb = fragment_message->msg_block_;
          while (mb->cont () != 0)
            mb = mb->cont ();

          // Add the current message block to the end of the chain
          mb->cont (queueable_message->msg_block_);

          // Get rid of the queuable message but save the message block
          queueable_message->msg_block_ = 0;
          queueable_message->release ();

          // One note is that TAO_Queued_Data contains version numbers,
          // but doesn't indicate the actual protocol to which those
          // version numbers refer.  That's not a problem, though, because
          // instances of TAO_Queued_Data live in a queue, and that queue
          // lives in a particular instance of a Transport, and the
          // transport instance has an association with a particular
          // messaging_object.  The concrete messaging object embodies a
          // messaging protocol, and must cover all versions of that
          // protocol.  Therefore, we just need to cover the bases of all
          // versions of that one protocol.
        }
      else
        {
          // There is a complete chain of fragments
          // Go ahead and enqueue this message
          return this->incoming_message_queue_.enqueue_tail (
                                                    queueable_message);
        }
      break;
    case 0x0102:  // GIOP 1.2
      // In 1.2, we get a little more context.  There's a
      // FRAGMENT message-specific header, and inside that is the
      // request id with which the fragment is associated.
      fragment_message =
          this->incoming_message_queue_.find_fragment (
                                queueable_message->request_id_);

      // No fragment was found
      if (fragment_message == 0)
        return this->incoming_message_queue_.enqueue_tail (
                                                queueable_message);

      if (fragment_message->major_version_ != major ||
          fragment_message->minor_version_ != minor)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("TAO (%P|%t) - ")
                           ACE_TEXT("TAO_Transport::enqueue_incoming_message ")
                           ACE_TEXT("GIOP versions do not match ")
                           ACE_TEXT("(%d.%d != %d.%d\n"),
                           fragment_message->major_version_,
                           fragment_message->minor_version_,
                           major, minor),
                          -1);

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG, "FRAGMENTED GIOP v1.2 MESSAGE BEING PROCESSED!!!\n")) ;
        }

      // Find the last message block in the continuation
      mb = fragment_message->msg_block_;
      while (mb->cont () != 0)
        mb = mb->cont ();

      // Add the current message block to the end of the chain
      mb->cont (queueable_message->msg_block_);

      // Remove our reference to the message block.  At this point
      // the message block of the fragment head owns it as part of a
      // chain
      queueable_message->msg_block_ = 0;

      // Get rid of the queuable message
      queueable_message->release ();
      break;
    default:
      if (!queueable_message->more_fragments_)
        return this->incoming_message_queue_.enqueue_tail (
                                                  queueable_message);
      // This is an unknown GIOP version
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("TAO (%P|%t) - ")
                         ACE_TEXT("TAO_Transport::enqueue_incoming_message ")
                         ACE_TEXT("can not handle a fragmented GIOP %d.%d ")
                         ACE_TEXT("message\n"), major, minor),
                        -1);
    }

  return 0;
}


int
TAO_Transport::process_parsed_messages (TAO_Queued_Data *qd,
                                        TAO_Resume_Handle &rh)
{
  // Get the <message_type> that we have received
  TAO_Pluggable_Message_Type t =  qd->msg_type_;

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
            ACE_TEXT("going to protocol processing.  ")
            ACE_TEXT("---Data block ref count (if any): %d\n"),
            this->id (),
            (qd&&qd->msg_block_)?
            qd->msg_block_->data_block()->reference_count():-1)) ;
    }
  if (TAO_debug_level >= 10 && qd && qd->msg_block_)
    {
      qd->dump_msg( "dispatching") ;
    }

  if (t == TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                    ACE_TEXT("received CloseConnection message\n"),
                    this->id()));

      // Return a "-1" so that the next stage can take care of
      // closing connection and the necessary memory management.
      return -1;
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REQUEST ||
           t == TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST)
    {
      // Let us resume the handle before we go ahead to process the
      // request. This will open up the handle for other threads.
      rh.resume_handle ();

      if (this->messaging_object ()->process_request_message (
            this,
            qd) == -1)
        {
          // Return a "-1" so that the next stage can take care of
          // closing connection and the necessary memory management.
          return -1;
        }
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REPLY ||
           t == TAO_PLUGGABLE_MESSAGE_LOCATEREPLY)
    {
      rh.resume_handle ();

      // @@todo: Maybe the input_cdr can be constructed from the
      // message_block
      TAO_Pluggable_Reply_Params params (this);


      if (this->messaging_object ()->process_reply_message (params,
                                                            qd) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                        ACE_TEXT("error in process_reply_message %p\n"),
                        this->id (), ""));
          return -1;
        }

      int result = this->tms ()->dispatch_reply (params);

      if (result == -1)
        {
          // Something really critical happened, we will forget about
          // every reply on this connection.
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                        ACE_TEXT("dispatch reply failed\n"),
                        this->id ()));

          return -1;
        }
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
    {
      if (TAO_debug_level)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                             ACE_TEXT("received MessageError, closing connection\n"),
                             this->id ()),
                            -1);
        }
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
            ACE_TEXT("---Data block ref count (if any) after protocol: %d\n"),
            this->id (),
            (qd&&qd->msg_block_)?
            qd->msg_block_->data_block()->reference_count():-1)) ;
    }

  // If not, just return back..
  return 0;
}

int
TAO_Transport::process_queue_head (TAO_Resume_Handle &rh)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_queue_head\n"),
                  this->id ()));
    }

  if (this->incoming_message_queue_.is_head_complete () != 1)
    return 1;

  // Get the message on the head of the queue..
  TAO_Queued_Data *qd =
    this->incoming_message_queue_.dequeue_head ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
            ACE_TEXT("dequeued message to process.  ")
            ACE_TEXT("---Data block ref count (if any): %d\n"),
            this->id (),
            (qd&&qd->msg_block_)?
            qd->msg_block_->data_block()->reference_count():-1)) ;
    }
  if (TAO_debug_level >= 10 && qd && qd->msg_block_)
    {
      qd->dump_msg( "dequeued") ;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
                  ACE_TEXT("the size of the queue is [%d]\n"),
                  this->id (),
                  this->incoming_message_queue_.queue_length()));
    }
  // Now that we have pulled out out one message out of the queue,
  // check whether we have one more message in the queue...
  if (this->incoming_message_queue_.queue_length () > 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
                      ACE_TEXT("notify reactor\n"),
                      this->id ()));

        }
      int retval =
        this->notify_reactor ();

      if (retval == 1)
        {
          // Let the class know that it doesn't need to resume  the
          // handle..
          rh.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
        }
      else if (retval < 0)
        return -1;
    }
  else
    {
      // As we are ready to process the last message just resume
      // the handle. Set the flag incase someone had reset the flag..
      rh.set_flag (TAO_Resume_Handle::TAO_HANDLE_RESUMABLE);
    }

  // Process the message...
  int retval = this->process_parsed_messages (qd, rh);

  // Delete the Queued_Data..
  TAO_Queued_Data::release (qd);

  return (retval == -1) ? -1 : 0;
}

int
TAO_Transport::notify_reactor (void)
{
  if (!this->ws_->is_registered ())
    {
      return 0;
    }

  ACE_Event_Handler *eh = this->event_handler_i ();

  // Get the reactor associated with the event handler
  ACE_Reactor *reactor = this->orb_core ()->reactor ();

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::notify_reactor, ")
                  ACE_TEXT("notify to Reactor\n"),
                  this->id ()));
    }


  // Send a notification to the reactor...
  const int retval = reactor->notify (eh,
                                      ACE_Event_Handler::READ_MASK);

  if (retval < 0 && TAO_debug_level > 2)
    {
      // @@todo: need to think about what is the action that
      // we can take when we get here.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
                  ACE_TEXT("notify to the reactor failed..\n"),
                  this->id ()));
    }

  return 1;
}

TAO_Transport_Cache_Manager &
TAO_Transport::transport_cache_manager (void)
{
  return this->orb_core_->lane_resources ().transport_cache ();
}

size_t
TAO_Transport::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

size_t
TAO_Transport::sent_byte_count (void)
{
  return this->sent_byte_count_;
}

void
TAO_Transport::assign_translators (TAO_InputCDR *inp, TAO_OutputCDR *outp)
{
  if (this->char_translator_)
    {
      this->char_translator_->assign (inp);
      this->char_translator_->assign (outp);
    }
  if (this->wchar_translator_)
    {
      this->wchar_translator_->assign (inp);
      this->wchar_translator_->assign (outp);
    }
}

ACE_Event_Handler::Reference_Count
TAO_Transport::add_reference (void)
{
  return this->event_handler_i ()->add_reference ();
}

ACE_Event_Handler::Reference_Count
TAO_Transport::remove_reference (void)
{
  return this->event_handler_i ()->remove_reference ();
}

TAO_OutputCDR &
TAO_Transport::out_stream (void)
{
  return this->messaging_object ()->out_stream ();
}

bool
TAO_Transport::post_open (size_t id)
{
  this->id_ = id;

  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->handler_lock_,
                      false);

    this->is_connected_ = true;
  }

  // When we have data in our outgoing queue schedule ourselves
  // for output
  if (this->queue_is_empty_i ())
    return true;

  // If the wait strategy wants us to be registered with the reactor
  // then we do so. If registeration is required and it succeeds,
  // #REFCOUNT# becomes two.
  if (this->wait_strategy ()->register_handler () == 0)
    {
      TAO_Flushing_Strategy *flushing_strategy =
        this->orb_core ()->flushing_strategy ();
      (void) flushing_strategy->schedule_output (this);
    }
  else
    {
      // Registration failures.

      // Purge from the connection cache, if we are not in the cache, this
      // just does nothing.
      (void) this->purge_entry ();

      // Close the handler.
      (void) this->close_connection ();

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport[%d]::post_connect , "
                    "could not register the transport "
                    "in the reactor.\n",
                    this->id ()));

      return false;
    }

  return true;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Reverse_Lock<ACE_Lock>;
template class ACE_Guard<ACE_Reverse_Lock<ACE_Lock> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Reverse_Lock<ACE_Lock>
#pragma instantiate ACE_Guard<ACE_Reverse_Lock<ACE_Lock> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
