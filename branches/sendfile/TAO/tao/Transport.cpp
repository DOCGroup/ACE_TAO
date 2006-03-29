// $Id$

#include "tao/Transport.h"

#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Wait_Strategy.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Stub.h"
#include "tao/Transport_Queueing_Strategies.h"
#include "tao/Connection_Handler.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/Synch_Queued_Message.h"
#include "tao/Asynch_Queued_Message.h"
#include "tao/Flushing_Strategy.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Resume_Handle.h"
#include "tao/Codeset_Manager.h"
#include "tao/Codeset_Translator_Base.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Reactor.h"
#include "ace/os_include/sys/os_uio.h"

/*
 * Specialization hook to add include files from
 * concrete transport implementation.
 */
//@@ TAO_TRANSPORT_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

#if !defined (__ACE_INLINE__)
# include "tao/Transport.inl"
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

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::%s, ")
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
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::%s, ")
                  ACE_TEXT ("buffer %d/%d has %d bytes\n"),
                  id, ACE_TEXT_CHAR_TO_TCHAR(location),
                  i, iovcnt,
                  iov_len));

      size_t len;

      for (size_t offset = 0; offset < iov_len; offset += len)
        {
          ACE_TCHAR header[1024];
          ACE_OS::sprintf (header,
                           ACE_TEXT("TAO - ")
                           ACE_TEXT("Transport[")
                           ACE_SIZE_T_FORMAT_SPECIFIER
                           ACE_TEXT("]::%s")
                           ACE_TEXT(" (")
                           ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT("/")
                           ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT(")"),
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
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::%s, ")
              ACE_TEXT ("end of data\n"),
              id, ACE_TEXT_CHAR_TO_TCHAR(location)));

  ACE_Log_Msg::instance ()->release ();
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  , partial_message_ (0)
{
  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);

  /*
   * Hook to add code that initializes components that
   * belong to the concrete protocol implementation.
   * Further additions to this Transport class will
   * need to add code *before* this hook.
   */
  //@@ TAO_TRANSPORT_SPL_CONSTRUCTOR_ADD_HOOK
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

  // Release the partial message block, however we may
  // have never allocated one.
  ACE_Message_Block::release (this->partial_message_);

  // By the time the destructor is reached here all the connection stuff
  // *must* have been cleaned up.

  /*
   * Hook to add code that cleans up components
   * belong to the concrete protocol implementation.
   * Further additions to this Transport class will
   * need to add code *before* this hook.
   */
  //@@ TAO_TRANSPORT_SPL_DESTRUCTOR_ADD_HOOK
}

void
TAO_Transport::provide_handler (TAO::Connection_Handler_Set &handlers)
{
  (void) this->add_reference ();

  handlers.insert (this->connection_handler_i ());
}

bool
TAO_Transport::provide_blockable_handler (TAO::Connection_Handler_Set &h)
{
  if (this->ws_->non_blocking () ||
      this->opening_connection_role_ == TAO::TAO_SERVER_ROLE)
    return false;

  (void) this->add_reference ();

  h.insert (this->connection_handler_i ());

  return true;
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

/*
 * A concrete transport class specializes this
 * method. This hook allows commenting this function
 * when TAO's transport is specialized. Note: All
 * functions that have an implementation that does
 * nothing should be added within this hook to
 * enable specialization.
 */
//@@ TAO_TRANSPORT_SPL_COMMENT_HOOK_START

int
TAO_Transport::tear_listen_point_list (TAO_InputCDR &)
{
  ACE_NOTSUP_RETURN (-1);
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

//@@ TAO_TRANSPORT_SPL_COMMENT_HOOK_END

bool
TAO_Transport::post_connect_hook (void)
{
  return true;
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
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::register_handler\n"),
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

#ifdef ACE_HAS_SENDFILE
ssize_t
TAO_Transport::sendfile (ACE_Message_Block * /* data */,
                         size_t & /* bytes_transferred */,
                         ACE_Time_Value const * /* timeout */)
{
  return 0;
}
#endif  /* ACE_HAS_SENDFILE */

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
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::generate_locate_request, ")
                      ACE_TEXT ("error while marshalling the LocateRequest header\n"),
                      this->id ()));
        }

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
  if (this->first_request_)
    {
      TAO_Codeset_Manager *csm = this->orb_core ()->codeset_manager ();
      if (csm)
        csm->generate_service_context (opdetails,*this);
    }

  if (this->messaging_object ()->generate_request_header (opdetails,
                                                          spec,
                                                          output) == -1)
    {
      if (TAO_debug_level > 0)
        {
        ACE_DEBUG ((LM_DEBUG,
                   ACE_TEXT ("(%P|%t) - Transport[%d]::generate_request_header, ")
                   ACE_TEXT ("error while marshalling the Request header\n"),
                      this->id()));
        }

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
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::make_idle\n"),
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
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_output\n"),
                  this->id ()));
    }

  // The flushing strategy (potentially via the Reactor) wants to send
  // more data, first check if there is a current message that needs
  // more sending...
  int retval = this->drain_queue ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_output, ")
                  ACE_TEXT ("drain_queue returns %d/%d\n"),
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
  TAO_Synch_Queued_Message synch_message (mb,
                                          this->orb_core_);

  synch_message.push_back (this->head_, this->tail_);

  int n = this->drain_queue_i ();

  if (n == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      return -1; // Error while sending...
    }
  else if (n == 1)
    {
      bytes_transferred = total_length;
      return 1;  // Empty queue, message was sent..
    }

  // Remove the temporary message from the queue...
  synch_message.remove_from_list (this->head_, this->tail_);

  bytes_transferred =
    total_length - synch_message.message_length ();

  return 0;
}

int
TAO_Transport::send_synchronous_message_i (const ACE_Message_Block *mb,
                                           ACE_Time_Value *max_wait_time)
{
  // We are going to block, so there is no need to clone
  // the message block.
  TAO_Synch_Queued_Message synch_message (mb, this->orb_core_);

  // Push synch_message on to the back of the queue.
  synch_message.push_back (this->head_, this->tail_);

  int n =
    this->send_synch_message_helper_i (synch_message,
                                       max_wait_time);

  if (n == -1 || n == 1)
    {
      return n;
    }

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
                                  this->orb_core_,
                                  0,
                                  1),
                              -1);
              queued_message->push_front (this->head_, this->tail_);
            }
        }

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_synchronous_message_i, ")
             ACE_TEXT ("error while flushing message - %m\n"),
             this->id ()));
        }

      return -1;
    }

  return 1;
}


int
TAO_Transport::send_reply_message_i (const ACE_Message_Block *mb,
                                     ACE_Time_Value *max_wait_time)
{
  // Dont clone now.. We could be sent in one shot!
  TAO_Synch_Queued_Message synch_message (mb, this->orb_core_);

  synch_message.push_back (this->head_,
                           this->tail_);

  int n =
    this->send_synch_message_helper_i (synch_message,
                                       max_wait_time);

  if (n == -1 || n == 1)
    {
      return n;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_reply_message_i, ")
         ACE_TEXT ("preparing to add to queue before leaving \n"),
         this->id ()));
    }

  // Till this point we shouldn't have any copying and that is the
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
      return -1; // Error while sending...
    }
  else if (n == 1)
    {
      return 1;  // Empty queue, message was sent..
    }

  if (synch_message.all_data_sent ())
    {
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::schedule_output_i\n"),
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::cancel_output_i\n"),
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
         ACE_TEXT ("TAO (%P|%t) - TAO_Transport[%d]::handle_timeout, ")
         ACE_TEXT ("timer expired\n"),
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
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
             ACE_TEXT ("send() returns 0\n"),
             this->id ()));
        }
      return -1;
    }
  else if (retval == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
             ACE_TEXT ("error during %p\n"),
             this->id (), ACE_TEXT ("send()")));
        }

      if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
          return 0;
        }

      return -1;
    }

  // ... start over, how do we guarantee progress?  Because if
  // no bytes are sent send() can only return 0 or -1

  // Total no. of bytes sent for a send call
  this->sent_byte_count_ += byte_count;

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
         ACE_TEXT ("byte_count = %d, head_is_empty = %d\n"),
         this->id(), byte_count, (this->head_ == 0)));
    }

  return 1;
}

int
TAO_Transport::drain_queue_i (ACE_Message_Block * raw_data)
{
#ifdef ACE_HAS_SENDFILE
  size_t bytes_transferred = 0;

  // If we've been given raw data and the queue is empty, perform a
  // zero-copy write.
  if (raw_data
      && this->queue_is_empty_i ()
      && this->sendfile (raw_data, bytes_transferred) == -1)
    {
      return -1;
    }
  this->sent_byte_count_ += bytes_transferred;
#else
  ACE_UNUSED_ARG (raw_data);
#endif  /* ACE_HAS_SENDFILE */

  // This is the vector used to send data, it must be declared outside
  // the loop because after the loop there may still be data to be
  // sent
  int iovcnt = 0;
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  iovec iov[ACE_IOV_MAX] = { 0 , 0 };
#else
  iovec iov[ACE_IOV_MAX];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

  // We loop over all the elements in the queue ...
  TAO_Queued_Message *i = this->head_;

  // Reset the value so that the counting is done for each new send
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
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
                 ACE_TEXT ("helper retval = %d\n"),
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
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
                 ACE_TEXT ("helper retval = %d\n"),
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::cleanup_queue_i, ")
         ACE_TEXT ("cleaning up complete queue\n"),
         this->id ()));
    }

  // Cleanup all messages
  while (this->head_ != 0)
    {
      TAO_Queued_Message *i = this->head_;

       // @@ This is a good point to insert a flag to indicate that a
       //    CloseConnection message was successfully received.
      i->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                        this->orb_core_->leader_follower ());

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
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::cleanup_queue, ")
             ACE_TEXT ("byte_count = %d\n"),
             this->id (), byte_count));
        }

      // Update the state of the first message
      i->bytes_transferred (byte_count);

      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::cleanup_queue, ")
             ACE_TEXT ("after transfer, bc = %d, all_sent = %d, ml = %d\n"),
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
                                              bool &must_flush)
{
  // First let's compute the size of the queue:
  size_t msg_count = 0;
  size_t total_bytes = 0;

  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      msg_count++;
      total_bytes += i->message_length ();
    }

  bool set_timer;
  ACE_Time_Value new_deadline;

  bool constraints_reached =
    stub->transport_queueing_strategy ().
      buffering_constraints_reached (stub,
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::report_invalid_event_handler")
         ACE_TEXT ("(%s) no longer associated with handler [tag=%d]\n"),
         this->id (), ACE_TEXT_CHAR_TO_TCHAR (caller), this->tag_));
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
  switch (message_semantics)
    {
      case TAO_Transport::TAO_TWOWAY_REQUEST:
        return this->send_synchronous_message_i (message_block,
                                                 max_wait_time);
      case TAO_Transport::TAO_REPLY:
        return this->send_reply_message_i (message_block,
                                           max_wait_time);
      case TAO_Transport::TAO_ONEWAY_REQUEST:
        return this->send_asynchronous_message_i (stub,
                                                  message_block,
                                                  max_wait_time);
    }

  return -1;
}

int
TAO_Transport::send_asynchronous_message_i (TAO_Stub *stub,
                                            const ACE_Message_Block *message_block,
                                            ACE_Time_Value *max_wait_time)
{
  // Let's figure out if the message should be queued without trying
  // to send first:
  bool try_sending_first = 1;

  const bool queue_empty = (this->head_ == 0);

  if (!queue_empty)
    {
      try_sending_first = false;
    }
  else if (stub->transport_queueing_strategy ().must_queue (queue_empty))
    {
      try_sending_first = false;
    }

  if (try_sending_first)
    {
      ssize_t n = 0;
      size_t byte_count = 0;
      // ... in this case we must try to send the message first ...

      const size_t total_length = message_block->total_length ();

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
             ACE_TEXT ("trying to send the message (ml = %d)\n"),
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
                  ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
                     ACE_TEXT ("fatal error in ")
                     ACE_TEXT ("send_message_block_chain_i - %m\n"),
                     this->id ()));
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
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
             ACE_TEXT ("partial send %d / %d bytes\n"),
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
    }

  // ... either the message must be queued or we need to queue it
  // because it was not completely sent out ...

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
         ACE_TEXT ("message is queued\n"),
         this->id ()));
    }

  if (this->queue_message_i(message_block) == -1)
  {
    ACE_DEBUG ((LM_DEBUG,
       ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
       ACE_TEXT ("cannot queue message for ")
       ACE_TEXT (" - %m\n"),
       this->id ()));
    return -1;
  }

  // ... if the queue is full we need to activate the output on the
  // queue ...
  bool must_flush = false;
  const bool constraints_reached =
    this->check_buffering_constraints_i (stub,
                                         must_flush);

  // ... but we also want to activate it if the message was partially
  // sent.... Plus, when we use the blocking flushing strategy the
  // queue is flushed as a side-effect of 'schedule_output()'

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();

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
                                             this->orb_core_,
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
                             int /* block */ /* deprecated parameter */ )
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input\n"),
         this->id ()));
    }

  // First try to process messages of the head of the incoming queue.
  int retval = this->process_queue_head (rh);

  if (retval <= 0)
    {
      if (retval == -1)
        {
          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input, ")
                 ACE_TEXT ("error while parsing the head of the queue\n"),
                 this->id()));

            }
          return -1;
        }
      else
        {
          // retval == 0

          // Processed a message in queue successfully. This
          // thread must return to thread-pool now.
          return 0;
        }
    }

  TAO_Queued_Data *q_data = 0;

  if (this->incoming_message_stack_.top (q_data) != -1
      && q_data->missing_data_ != TAO_MISSING_DATA_UNDEFINED)
    {
      /* PRE: q_data->missing_data_ > 0 as all QD on stack must be incomplete  */
      if (this->handle_input_missing_data (rh, max_wait_time, q_data) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input, ")
                 ACE_TEXT ("error consolidating incoming message\n"),
                 this->id ()));
            }
          return -1;
        }
    }
  else
    {
      if (this->handle_input_parse_data (rh, max_wait_time) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input, ")
                 ACE_TEXT ("error parsing incoming message\n"),
                 this->id ()));
            }
          return -1;
        }
    }

  return 0;
}

int
TAO_Transport::consolidate_process_message (TAO_Queued_Data *q_data,
                                            TAO_Resume_Handle &rh)
{
  // paranoid check
  if (q_data->missing_data_ != 0)
    {
      if (TAO_debug_level > 0)
        {
           ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::consolidate_process_message, ")
              ACE_TEXT ("missing data\n"),
              this->id ()));
        }
       return -1;
    }

  if (q_data->more_fragments_ ||
      q_data->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT)
    {
      // consolidate message on top of stack, only for fragmented messages
      TAO_Queued_Data *new_q_data = 0;

      switch (this->messaging_object()->consolidate_fragmented_message (q_data, new_q_data))
        {
        case -1: // error
          return -1;

        case 0:  // returning consolidated message in q_data
          if (!new_q_data)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::consolidate_process_message, ")
                     ACE_TEXT ("error, consolidated message is NULL\n"),
                     this->id ()));
                }
              return -1;
            }


          if (this->process_parsed_messages (new_q_data, rh) == -1)
            {
              TAO_Queued_Data::release (new_q_data);

              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::consolidate_process_message, ")
                     ACE_TEXT ("error processing consolidated message\n"),
                     this->id ()));
                }
              return -1;
            }

          TAO_Queued_Data::release (new_q_data);

          break;

        case 1:  // fragment has been stored in messaging_oject()
          break;
        }
    }
  else
    {
      if (this->process_parsed_messages (q_data, rh) == -1)
        {
          TAO_Queued_Data::release (q_data);

          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::consolidate_process_message, ")
                 ACE_TEXT ("error processing message\n"),
                 this->id ()));
            }
          return -1;
        }

      TAO_Queued_Data::release (q_data);

    }

  return 0;
}

int
TAO_Transport::consolidate_enqueue_message (TAO_Queued_Data *q_data)
{
  // consolidate message on top of stack, only for fragmented messages

  // paranoid check
  if (q_data->missing_data_ != 0)
    {
       return -1;
    }

  if (q_data->more_fragments_ ||
      q_data->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT)
    {
      TAO_Queued_Data *new_q_data = 0;

      switch (this->messaging_object()->consolidate_fragmented_message (q_data, new_q_data))
        {
        case -1: // error
          return -1;

        case 0:  // returning consolidated message in new_q_data
          if (!new_q_data)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::consolidate_enqueue_message, ")
                     ACE_TEXT ("error, consolidated message is NULL\n"),
                     this->id ()));
                }
              return -1;
            }

          if (this->incoming_message_queue_.enqueue_tail (new_q_data) != 0)
            {
              TAO_Queued_Data::release (new_q_data);
              return -1;
            }
          break;

        case 1:  // fragment has been stored in messaging_oject()
          break;
        }
    }
  else
    {
      if (this->incoming_message_queue_.enqueue_tail (q_data) != 0)
        {
          TAO_Queued_Data::release (q_data);
          return -1;
        }
    }

  return 0; // success
}

int
TAO_Transport::handle_input_missing_data (TAO_Resume_Handle &rh,
                                          ACE_Time_Value * max_wait_time,
                                          TAO_Queued_Data *q_data)
{
  // paranoid check
  if (q_data == 0)
    {
      return -1;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_missing_data_message, ")
         ACE_TEXT ("enter (missing data == %d)\n"),
         this->id (), q_data->missing_data_));
    }

  const size_t recv_size = q_data->missing_data_;

  // make sure the message_block has enough space
  const size_t message_size =  recv_size
                               + q_data->msg_block_->length();

  if (q_data->msg_block_->space() < recv_size)
    {
      if (ACE_CDR::grow (q_data->msg_block_, message_size) == -1)
        {
          return -1;
        }
    }

  // Saving the size of the received buffer in case any one needs to
  // get the size of the message thats received in the
  // context. Obviously the value will be changed for each recv call
  // and the user is supposed to invoke the accessor only in the
  // invocation context to get meaningful information.
  this->recv_buffer_size_ = recv_size;

  // Read the message into the existing message block on heap
  const ssize_t n = this->recv (q_data->msg_block_->wr_ptr(),
                                recv_size,
                                max_wait_time);


  if (n <= 0)
    {
      return n;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_missing_data_message, ")
         ACE_TEXT ("read bytes %d\n"),
         this->id (), n));
    }

  q_data->msg_block_->wr_ptr(n);
  q_data->missing_data_ -= n;

  if (q_data->missing_data_ == 0)
    {
      // paranoid check
      if (this->incoming_message_stack_.pop (q_data) == -1)
        {
          return -1;
        }

      if (this->consolidate_process_message (q_data, rh) == -1)
        {
          return -1;
        }
    }

  return 0;
}


int
TAO_Transport::handle_input_parse_extra_messages (ACE_Message_Block &message_block)
{

  // store buffer status of last extraction: -1 parse error, 0
  // incomplete message header in buffer, 1 complete messages header
  // parsed
  int buf_status = 0;

  TAO_Queued_Data *q_data = 0;     // init

  // parse buffer until all messages have been extracted, consolidate
  // and enqueue complete messages, if the last message being parsed
  // has missin data, it is stays on top of incoming_message_stack.
  while (message_block.length () > 0 &&
         (buf_status = this->messaging_object ()->extract_next_message
          (message_block, q_data)) != -1 &&
         q_data != 0) // paranoid check
    {
      if (q_data->missing_data_ == 0)
        {
          if (this->consolidate_enqueue_message (q_data) == -1)
            {
              return -1;
            }
        }
      else  // incomplete message read, probably the last message in buffer
        {
          // can not fail
          this->incoming_message_stack_.push (q_data);
        }

      q_data = 0; // reset
    } // while

  if (buf_status == -1)
    {
      return -1;
    }

  return 0;
}

int
TAO_Transport::handle_input_parse_data  (TAO_Resume_Handle &rh,
                                         ACE_Time_Value * max_wait_time)
{

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_parse_data, ")
         ACE_TEXT ("enter\n"),
         this->id ()));
    }


  // The buffer on the stack which will be used to hold the input
  // messages, ACE_CDR::MAX_ALIGNMENT compensates the
  // memory-alignment. This improves performance with SUN-Java-ORB-1.4
  // and higher that sends fragmented requests of size 1024 bytes.
  char buf [TAO_MAXBUFSIZE + ACE_CDR::MAX_ALIGNMENT];

#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  (void) ACE_OS::memset (buf,
                         '\0',
                         sizeof buf);
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

  // Create a data block
  ACE_Data_Block db (sizeof (buf),
                     ACE_Message_Block::MB_DATA,
                     buf,
                     this->orb_core_->input_cdr_buffer_allocator (),
                     this->orb_core_->locking_strategy (),
                     ACE_Message_Block::DONT_DELETE,
                     this->orb_core_->input_cdr_dblock_allocator ());

  // Create a message block
  ACE_Message_Block message_block (&db,
                                   ACE_Message_Block::DONT_DELETE,
                                   this->orb_core_->input_cdr_msgblock_allocator ());


  // Align the message block
  ACE_CDR::mb_align (&message_block);

  size_t recv_size = 0; // Note: unsigned integer

  // Pointer to newly parsed message
  TAO_Queued_Data *q_data = 0;

  // optimizing access of constants
  const size_t header_length =
            this->messaging_object ()->header_length ();

  // paranoid check
  if (header_length > message_block.space ())
    {
      return -1;
    }

  if (this->orb_core_->orb_params ()->single_read_optimization ())
    {
      recv_size =
        message_block.space ();
    }
  else
    {
      // Single read optimization has been de-activated. That means
      // that we need to read from transport the GIOP header first
      // before the payload. This codes first checks the incoming
      // stack for partial messages which needs to be
      // consolidated. Otherwise we are in new cycle, reading complete
      // GIOP header of new incoming message.
      if (this->incoming_message_stack_.top (q_data) != -1
           && q_data->missing_data_ == TAO_MISSING_DATA_UNDEFINED)
        {
          // There is a partial message on incoming_message_stack_
          // whose length is unknown so far. We need to consolidate
          // the GIOP header to get to know the payload size,
          recv_size = header_length - q_data->msg_block_->length ();
        }
      else
        {
          // Read amount of data forming GIOP header of new incoming
          // message.
          recv_size = header_length;
        }
      // POST: 0 <= recv_size <= header_length
    }
  // POST: 0 <= recv_size <= message_block->space ()

  // If we have a partial message, copy it into our message block and
  // clear out the partial message.
  if (this->partial_message_ != 0 && this->partial_message_->length () > 0)
    {
      // (*) Copy back the partial message into current read-buffer,
      // verify that the read-strategy of "recv_size" bytes is not
      // exceeded. The latter check guarantees that recv_size does not
      // roll-over and keeps in range
      // 0<=recv_size<=message_block->space()
      if (this->partial_message_->length () <= recv_size &&
          message_block.copy (this->partial_message_->rd_ptr (),
                              this->partial_message_->length ()) == 0)
        {

          recv_size -= this->partial_message_->length ();
          this->partial_message_->reset ();
        }
      else
        {
          return -1;
        }
    }
  // POST: 0 <= recv_size <= buffer_space

  if (0 >= recv_size) // paranoid: the check above (*) guarantees recv_size>=0
    {
      // This event would cause endless looping, trying frequently to
      // read zero bytes from stream.  This might happen, if TAOs
      // protocol implementation is not correct and tries to read data
      // beyond header without "single_read_optimazation" being
      // activated.
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_parse_data, ")
             ACE_TEXT ("Error - endless loop detection, closing connection"),
             this->id ()));
        }
      return -1;
    }

  // Saving the size of the received buffer in case any one needs to
  // get the size of the message thats received in the
  // context. Obviously the value will be changed for each recv call
  // and the user is supposed to invoke the accessor only in the
  // invocation context to get meaningful information.
  this->recv_buffer_size_ = recv_size;

  // Read the message into the  message block that we have created on
  // the stack.
  const ssize_t n = this->recv (message_block.wr_ptr (),
                                recv_size,
                                max_wait_time);

  // If there is an error return to the reactor..
  if (n <= 0)
    {
      return n;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_parse_data, ")
         ACE_TEXT ("read %d bytes\n"),
         this->id (), n));
    }

  // Set the write pointer in the stack buffer
  message_block.wr_ptr (n);

  //
  // STACK PROCESSING OR MESSAGE CONSOLIDATION
  //

  // PRE: data in buffer is aligned && message_block.length() > 0

  if (this->incoming_message_stack_.top (q_data) != -1
      && q_data->missing_data_ == TAO_MISSING_DATA_UNDEFINED)
    {
      //
      // MESSAGE CONSOLIDATION
      //

      // Partial message on incoming_message_stack_ needs to be
      // consolidated.  The message header could not be parsed so far
      // and therefor the message size is unknown yet. Consolidating
      // the message destroys the memory alignment of succeeding
      // messages sharing the buffer, for that reason consolidation
      // and stack based processing are mutial exclusive.
      if (this->messaging_object ()->consolidate_node (q_data,
                                                       message_block) == -1)
        {
           if (TAO_debug_level > 0)
            {
                ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_parse_data, ")
                   ACE_TEXT ("error consolidating message from input buffer\n"),
                   this->id () ));
             }
           return -1;
        }

      // Complete message are to be enqueued and later processed
      if (q_data->missing_data_ == 0)
        {
          if (this->incoming_message_stack_.pop (q_data) == -1)
            {
              return -1;
            }

          if (this->consolidate_enqueue_message (q_data) == -1)
            {
              return -1;
            }
        }

      if (message_block.length () > 0
          && this->handle_input_parse_extra_messages (message_block) == -1)
        {
          return -1;
        }

      // In any case try to process the enqueued messages
      if (this->process_queue_head (rh) == -1)
        {
          return -1;
        }
    }
  else
    {
      //
      // STACK PROCESSING (critical path)
      //

      // Process the first message in buffer on stack

      // (PRE: first message resides in aligned memory) Make a node of
      // the message-block..

      TAO_Queued_Data qd (&message_block,
                          this->orb_core_->transport_message_buffer_allocator ());

      size_t mesg_length  = 0;

      if (this->messaging_object ()->parse_next_message (message_block,
                                                         qd,
                                                         mesg_length) == -1
          || (qd.missing_data_ == 0
              && mesg_length > message_block.length ()) )
        {
          // extracting message failed
          return -1;
        }
      // POST: qd.missing_data_ == 0 --> mesg_length <= message_block.length()
      // This prevents seeking rd_ptr behind the wr_ptr

      if (qd.missing_data_ != 0 ||
          qd.more_fragments_   ||
          qd.msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT)
        {
          if (qd.missing_data_ == 0)
            {
              // Dealing with a fragment
              TAO_Queued_Data *nqd =
                TAO_Queued_Data::duplicate (qd);

              if (nqd == 0)
                {
                  return -1;
                }

              // mark the end of message in new buffer
              char* end_mark = nqd->msg_block_->rd_ptr ()
                             + mesg_length;
              nqd->msg_block_->wr_ptr (end_mark);

              // move the read pointer forward in old buffer
              message_block.rd_ptr (mesg_length);

              // enqueue the message
              if (this->consolidate_enqueue_message (nqd) == -1)
                {
                  return -1;
                }

              if (message_block.length () > 0
                  && this->handle_input_parse_extra_messages (message_block) == -1)
                {
                  return -1;
                }

              // In any case try to process the enqueued messages
              if (this->process_queue_head (rh) == -1)
                {
                  return -1;
                }
            }
          else if (qd.missing_data_ != TAO_MISSING_DATA_UNDEFINED)
            {
              // Incomplete message, must be the last one in buffer

              if (qd.missing_data_ != TAO_MISSING_DATA_UNDEFINED &&
                  qd.missing_data_ > message_block.space ())
                {
                  // Re-Allocate correct size on heap
                  if (ACE_CDR::grow (qd.msg_block_,
                                     message_block.length ()
                                     + qd.missing_data_) == -1)
                    {
                      return -1;
                    }
                }

              TAO_Queued_Data *nqd =
                TAO_Queued_Data::duplicate (qd);

              if (nqd == 0)
                {
                  return -1;
                }

              // move read-pointer to end of buffer
              message_block.rd_ptr (message_block.length());

              this->incoming_message_stack_.push (nqd);
            }
        }
      else
        {
          //
          // critical path
          //

          // We cant process the message on stack right now. First we
          // have got to parse extra messages from message_block,
          // putting them into queue.  When this is done we can return
          // to process this message, and notifying other threads to
          // process the messages in queue.

          char * end_marker = message_block.rd_ptr ()
                            + mesg_length;

          if (message_block.length () > mesg_length)
            {
              // There are more message in data stream to be parsed.
              // Safe the rd_ptr to restore later.
              char *rd_ptr_stack_mesg = message_block.rd_ptr ();

              // Skip parsed message, jump to next message in buffer
              // PRE: mesg_length <= message_block.length ()
              message_block.rd_ptr (mesg_length);

              // Extract remaining messages and enqueue them for later
              // heap processing
              if (this->handle_input_parse_extra_messages (message_block) == -1)
                {
                  return -1;
                }

              // correct the end_marker
              end_marker = message_block.rd_ptr ();

              // Restore rd_ptr
              message_block.rd_ptr (rd_ptr_stack_mesg);
            }

          // The following if-else has been copied from
          // process_queue_head().  While process_queue_head()
          // processes message on heap, here we will process a message
          // on stack.

          // Now that we have one message on stack to be processed,
          // check whether we have one more message in the queue...
          if (this->incoming_message_queue_.queue_length () > 0)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input_parse_data, ")
                     ACE_TEXT ("notify reactor\n"),
                     this->id ()));

                }

              const int retval = this->notify_reactor ();

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
              // As there are no further messages in queue just resume
              // the handle. Set the flag incase someone had reset the flag..
              rh.set_flag (TAO_Resume_Handle::TAO_HANDLE_RESUMABLE);
            }

          // PRE: incoming_message_queue is empty
          if (this->process_parsed_messages (&qd,
                                             rh) == -1)
            {
              return -1;
            }

          // move the rd_ptr tp position of end_marker
          message_block.rd_ptr (end_marker);
        }
    }

  // Now that all cases have been processed, there might be kept some data
  // in buffer that needs to be safed for next "handle_input" invocations.
   if (message_block.length () > 0)
     {
       if (this->partial_message_ == 0)
         {
           this->allocate_partial_message_block ();
         }

       if (this->partial_message_ != 0 &&
           this->partial_message_->copy (message_block.rd_ptr (),
                                         message_block.length ()) == 0)
         {
           message_block.rd_ptr (message_block.length ());
         }
       else
         {
           return -1;
         }
     }

   return 0;
}


int
TAO_Transport::process_parsed_messages (TAO_Queued_Data *qd,
                                        TAO_Resume_Handle &rh)
{
  if (TAO_debug_level > 7)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
         ACE_TEXT ("entering (missing data == %d)\n"),
         this->id(), qd->missing_data_));
    }

  // Get the <message_type> that we have received
  const TAO_Pluggable_Message_Type t = qd->msg_type_;

  // int result = 0;

  if (t == TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
           ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
           ACE_TEXT ("received CloseConnection message - %m\n"),
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

      TAO_Pluggable_Reply_Params params (this);

      if (this->messaging_object ()->process_reply_message (params,
                                                            qd) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
               ACE_TEXT ("error in process_reply_message - %m\n"),
               this->id ()));

          return -1;
        }

    }
  else if (t == TAO_PLUGGABLE_MESSAGE_CANCELREQUEST)
    {
      // The associated request might be incomplpete residing
      // fragmented in messaging object. We must make sure the
      // resources allocated by fragments are released.

      if (this->messaging_object ()->discard_fragmented_message (qd) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                 ACE_TEXT ("error processing CancelRequest\n"),
                 this->id ()));
            }
        }

      // We are not able to cancel requests being processed already;
      // this is declared as optional feature by CORBA, and TAO does
      // not support this currently.

      // Just continue processing, CancelRequest does not mean to cut
      // off the connection.
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
             ACE_TEXT ("received MessageError, closing connection\n"),
             this->id ()));
        }
      return -1;
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_queue_head, %d enqueued\n"),
         this->id (), this->incoming_message_queue_.queue_length () ));
    }

  // See if  message in queue ...
  if (this->incoming_message_queue_.queue_length () > 0)
    {
      // Get the message on the head of the queue..
      TAO_Queued_Data *qd =
        this->incoming_message_queue_.dequeue_head ();

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
             ACE_TEXT ("the size of the queue is [%d]\n"),
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
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_queue_head, ")
                 ACE_TEXT ("notify reactor\n"),
                 this->id ()));

            }

          const int retval = this->notify_reactor ();

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
      if (this->process_parsed_messages (qd, rh) == -1)
        {
          return -1;
        }

      // Delete the Queued_Data..
      TAO_Queued_Data::release (qd);

      return 0;
    }

  return 1;
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::notify_reactor, ")
         ACE_TEXT ("notify to Reactor\n"),
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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::notify_reactor, ")
         ACE_TEXT ("notify to the reactor failed..\n"),
         this->id ()));
    }

  return 1;
}

TAO::Transport_Cache_Manager &
TAO_Transport::transport_cache_manager (void)
{
  return this->orb_core_->lane_resources ().transport_cache ();
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

void
TAO_Transport::clear_translators (TAO_InputCDR *inp, TAO_OutputCDR *outp)
{
  if (inp)
    {
      inp->char_translator (0);
      inp->wchar_translator (0);
    }
  if (outp)
    {
      outp->char_translator (0);
      outp->wchar_translator (0);
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
           ACE_TEXT ("TAO (%P|%t) - Transport[%d]::post_connect , ")
           ACE_TEXT ("could not register the transport ")
           ACE_TEXT ("in the reactor.\n"),
           this->id ()));

      return false;
    }

  return true;
}

void
TAO_Transport::allocate_partial_message_block (void)
{
  if (this->partial_message_ == 0)
    {
      // This value must be at least large enough to hold a GIOP message
      // header plus a GIOP fragment header
      const size_t partial_message_size =
        this->messaging_object ()->header_length ();
       // + this->messaging_object ()->fragment_header_length ();
       // deprecated, conflicts with not-single_read_opt.

      ACE_NEW (this->partial_message_,
               ACE_Message_Block (partial_message_size));
    }
}

/*
 * Hook to add concrete implementations from the derived class onto
 * TAO's transport.
 */

//@@ TAO_TRANSPORT_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
