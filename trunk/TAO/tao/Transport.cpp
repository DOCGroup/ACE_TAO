// -*- C++ -*-
// $Id$

#include "Transport.h"

#include "Exception.h"
#include "ORB_Core.h"
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
#include "Transport_Cache_Manager.h"
#include "Thread_Lane_Resources.h"
#include "debug.h"
#include "Resume_Handle.h"
#include "Notify_Handler.h"
#include "ace/Message_Block.h"
#include "ace/Reactor.h"

#include "Codeset_Manager.h"
#include "Codeset_Translator_Factory.h"

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

#define DUMP_IOV_PREFIX  "Transport[" ACE_SIZE_T_FORMAT_SPECIFIER "]::%s"
  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) - "
              DUMP_IOV_PREFIX ", "
              "sending %d buffers\n",
              id, location, iovcnt));
  for (int i = 0; i != iovcnt && 0 < current_transfer; ++i)
    {
      size_t iov_len = iov[i].iov_len;

      // Possibly a partially sent iovec entry.
      if (current_transfer < iov_len)
        iov_len = current_transfer;

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - "
                  DUMP_IOV_PREFIX ", "
                  "buffer %d/%d has %d bytes\n",
                  id, location,
                  i, iovcnt,
                  iov_len));

      size_t len;
      for (size_t offset = 0; offset < iov_len; offset += len)
        {
          ACE_TCHAR header[1024];
          ACE_OS::sprintf (header,
                           "TAO - " DUMP_IOV_PREFIX " ("
                           ACE_SIZE_T_FORMAT_SPECIFIER "/"
                           ACE_SIZE_T_FORMAT_SPECIFIER ")\n",
                           id, location, offset, iov_len);

          len = iov_len - offset;
          if (len > 512)
            len = 512;
          ACE_HEX_DUMP ((LM_DEBUG,
                         ACE_static_cast(char*,iov[i].iov_base) + offset,
                         len,
                         header));
        }
      current_transfer -= iov_len;
    }
  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) - "
              DUMP_IOV_PREFIX ", "
              "end of data\n",
              id, location));

  ACE_Log_Msg::instance ()->release ();
}

/*
 * Definitions for methods declared in the transport class
 *
 */

// Constructor.
TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : TAO_Synch_Refcountable (orb_core->resource_factory ()->create_cached_connection_lock (), 1)
  , tag_ (tag)
  , orb_core_ (orb_core)
  , cache_map_entry_ (0)
  , bidirectional_flag_ (-1)
  , head_ (0)
  , tail_ (0)
  , incoming_message_queue_ (orb_core)
  , current_deadline_ (ACE_Time_Value::zero)
  , flush_timer_id_ (-1)
  , transport_timer_ (this)
  , handler_lock_ (orb_core->resource_factory ()->create_cached_connection_lock ())
  , id_ ((size_t) this)
  , purging_order_ (0)
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
  ACE_ASSERT(this->refcount() == 0);

  delete this->ws_;

  delete this->tms_;

  delete this->handler_lock_;

  // By the time the destructor is reached all the connection stuff
  // *must* have been cleaned up
  ACE_ASSERT(this->head_ == 0);
  ACE_ASSERT(this->cache_map_entry_ == 0);
}


/*
 *
 * Public utility methods that are called by other classes.
 *
 */
/*static*/ TAO_Transport*
TAO_Transport::_duplicate (TAO_Transport* transport)
{
  if (transport != 0)
    {
      transport->increment ();
    }
  return transport;
}

/*static*/ void
TAO_Transport::release (TAO_Transport* transport)
{
  if (transport != 0)
    {
      int count = transport->decrement ();

      if (count == 0)
        {
          delete transport;
        }
      else if (count < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - Transport[%d]::release, "
                      "reference count is less than zero: %d\n",
                      transport->id (), count));
          ACE_OS::abort ();
        }
    }
}


void
TAO_Transport::provide_handle (ACE_Handle_Set &reactor_registered,
                               TAO_EventHandlerSet &unregistered)
{
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->handler_lock_));
  ACE_Event_Handler *eh = this->event_handler_i ();

  if (eh != 0)
    {
      if (this->ws_->is_registered ())
        {
          reactor_registered.set_bit (eh->get_handle ());
        }
      else
        {
          unregistered.insert (eh);
        }
    }
}


int
TAO_Transport::register_handler (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));
  if (this->check_event_handler_i ("Transport::register_handler") == -1)
    return -1;

  return this->register_handler_i ();
}


ssize_t
TAO_Transport::send (iovec *iov, int iovcnt,
                     size_t &bytes_transferred,
                     const ACE_Time_Value *timeout)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  if (this->check_event_handler_i ("Transport::send") == -1)
    return -1;

  // now call the template method
  return this->send_i (iov, iovcnt, bytes_transferred, timeout);
}


ssize_t
TAO_Transport::recv (char *buffer,
                     size_t len,
                     const ACE_Time_Value *timeout)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  if (this->check_event_handler_i ("Transport::recv") == -1)
    return -1;

  // now call the template method
  return this->recv_i (buffer, len, timeout);
}


int
TAO_Transport::idle_after_send (void)
{
  return this->tms ()->idle_after_send ();
}

int
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
  TAO_Connection_Handler * eh = this->invalidate_event_handler ();
  this->close_connection_shared (1, eh);
}

int
TAO_Transport::generate_locate_request (
    TAO_Target_Specification &spec,
    TAO_Operation_Details &opdetails,
    TAO_OutputCDR &output)
{
  if (this->messaging_object ()->generate_locate_request_header (opdetails,
                                                                 spec,
                                                                 output) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport[%d]::generate_locate_request, "
                    "error while marshalling the LocateRequest header\n",
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
  if (this->first_request_)
    this->orb_core()->codeset_manager()->
      generate_service_context( opdetails, *this );

  if (this->messaging_object ()->generate_request_header (opdetails,
                                                          spec,
                                                          output) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) - Transport[%d]::generate_request_header, "
                    "error while marshalling the Request header\n",
                    this->id()));

      return -1;
    }

  return 0;
}


/*
 * NOTE: Some of these calls looks like ideal fodder for
 * inlining. But, please note that the calls made within the method
 * are not inlined.  This would increase closure cost on the
 * compiler.
 */
void
TAO_Transport::connection_handler_closing (void)
{
  // The connection has closed, we must invalidate the handler to
  // ensure that any attempt to use this transport results in an
  // error.  Basically all the other methods in the Transport
  // cooperate via check_event_handler_i()

  TAO_Connection_Handler * eh = this->invalidate_event_handler ();
  this->send_connection_closed_notifications ();

  if (eh != 0)
    {
      // REFCNT: Matches incr_refcnt in XXX_Transport::XXX_Transport
      // REFCNT: Only one of this or close_connection_shared() run
      eh->decr_refcount();
    }
}

// @@TODO: Ideally the following should be inline.
int
TAO_Transport::recache_transport (TAO_Transport_Descriptor_Interface *desc)
{
  // First purge our entry
  this->transport_cache_manager ().purge_entry (this->cache_map_entry_);

  // Then add ourselves to the cache
  return this->transport_cache_manager ().cache_transport (desc,
                                                           this);
}

void
TAO_Transport::purge_entry (void)
{
  (void) this->transport_cache_manager ().purge_entry (this->cache_map_entry_);
}

int
TAO_Transport::make_idle (void)
{
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
                  "TAO (%P|%t) - Transport[%d]::handle_output\n",
                  this->id ()));
    }

  // The flushing strategy (potentially via the Reactor) wants to send
  // more data, first check if there is a current message that needs
  // more sending...
  int retval = this->drain_queue ();

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::handle_output, "
                  "drain_queue returns %d/%d\n",
                  this->id (),
                  retval, errno));
    }

  // Any errors are returned directly to the Reactor
  return retval;
}


int
TAO_Transport::send_message_block_chain (const ACE_Message_Block *mb,
                                         size_t &bytes_transferred,
                                         ACE_Time_Value *max_wait_time)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

  if (this->check_event_handler_i ("Transport::send_message_block_chain") == -1)
    return -1;

  return this->send_message_block_chain_i (mb,
                                           bytes_transferred,
                                           max_wait_time);
}

int
TAO_Transport::send_message_block_chain_i (const ACE_Message_Block *mb,
                                           size_t &bytes_transferred,
                                           ACE_Time_Value *)
{
  size_t total_length = mb->total_length ();

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
  int r;
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

    if (this->check_event_handler_i ("Transport::send_message_shared") == -1)
      return -1;

    r = this->send_message_shared_i (stub, message_semantics,
                                     message_block, max_wait_time);
  }
  if (r == -1)
  {
    this->close_connection ();
  }

  return r;
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

  if (n == -1 ||
      n == 1)
    return n;

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
                      "TAO (%P|%t) - Transport[%d]::send_synchronous_message_i, "
                      "error while flushing message %p\n",
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

  if (n == -1 ||
      n == 1)
    return n;

  ACE_ASSERT (n == 0);

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::send_reply_message_i, "
                  "preparing to add to queue before leaving \n",
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


void
TAO_Transport::close_connection_i (void)
{
  TAO_Connection_Handler * eh = this->invalidate_event_handler_i ();
  this->close_connection_shared (1, eh);
}

void
TAO_Transport::close_connection_no_purge (void)
{
  TAO_Connection_Handler * eh = this->invalidate_event_handler ();

  this->close_connection_shared (0,
                                 eh);
}

void
TAO_Transport::close_connection_shared (int purge,
                                        TAO_Connection_Handler * eh)
{
  // Purge the entry
  if (purge)
    {
      this->transport_cache_manager ().purge_entry (this->cache_map_entry_);
    }

  if (eh == 0)
    {
      // The connection was already closed
      return;
    }

  // Set the event handler in the connection close wait state.
  (void) eh->connection_close_wait ();

  // NOTE: If the wait strategy is in blocking mode, then there is no
  // chance that it could be inside the reactor. We can safely skip
  // driving the LF. If <purge> is 0, then we are cleaned up by the
  // cache. So no point in driving the LF either.
  if (this->ws_->non_blocking () && purge)
    {
      // NOTE: This is a work around for BUG 1020. We drive the leader
      // follower for a predetermined amount of time. Ideally this
      // needs to be an ORB option. But this is just the first
      // cut. Doing that will be a todo..

      ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT, 0);
      this->orb_core_->leader_follower ().wait_for_event (eh,
                                                          this,
                                                          &tv);

    }

  // We need to explicitly shut it down to avoid memory leaks.
  if (!eh->successful () ||
      !this->ws_->non_blocking ())
    {
      eh->close_connection ();
    }

  this->send_connection_closed_notifications ();

  // REFCNT: Matches incr_refcnt in XXX_Transport::XXX_Transport
  // REFCNT: Only one of this or connection_handler_closing() run
  eh->decr_refcount ();
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
  if (eh == 0)
    return -1;

  ACE_Reactor *reactor = eh->reactor ();
  if (reactor == 0)
    return -1;

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::schedule_output\n",
                  this->id ()));
    }

  return reactor->schedule_wakeup (eh, ACE_Event_Handler::WRITE_MASK);
}

int
TAO_Transport::cancel_output_i (void)
{
  ACE_Event_Handler *eh = this->event_handler_i ();
  if (eh == 0)
    return -1;

  ACE_Reactor *reactor = eh->reactor ();
  if (reactor == 0)
    return -1;

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::cancel_output\n",
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
                  "TAO (%P|%t) - TAO_Transport[%d]::handle_timeout, "
                  "timer expired\n",
                  this->id ()));
    }

  /// This is the only legal ACT in the current configuration....
  if (act != &this->current_deadline_)
    return -1;

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
  if (this->check_event_handler_i ("Transport::drain_queue_helper") == -1)
    return -1;

  size_t byte_count = 0;

  // ... send the message ...
  ssize_t retval =
    this->send_i (iov, iovcnt, byte_count);

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
                      "TAO (%P|%t) - Transport[%d]::drain_queue_helper, "
                      "send() returns 0",
                      this->id ()));
        }
      return -1;
    }
  else if (retval == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::drain_queue_helper, "
                      "error during %p\n",
                      this->id (), "send_i()"));
        }
      if (errno == EWOULDBLOCK)
        return 0;
      return -1;
    }

  // ... start over, how do we guarantee progress?  Because if
  // no bytes are sent send() can only return 0 or -1
  ACE_ASSERT (byte_count != 0);

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::drain_queue_helper, "
                  "byte_count = %d, head_is_empty = %d\n",
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
                          "TAO (%P|%t) - Transport[%d]::drain_queue_i, "
                          "helper retval = %d\n",
                          this->id (), retval));
            }
          if (retval != 1)
            return retval;

          i = this->head_;
          continue;
        }
      // ... notice that this line is only reached if there is still
      // room in the iovector ...
      i = i->next ();
    }


  if (iovcnt != 0)
    {
      int retval =
        this->drain_queue_helper (iovcnt, iov);

          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) - Transport[%d]::drain_queue_i, "
                          "helper retval = %d\n",
                          this->id (), retval));
            }
      if (retval != 1)
        return retval;
    }

  if (this->head_ == 0)
    {
      if (this->flush_timer_pending ())
        {
          ACE_Event_Handler *eh = this->event_handler_i ();
          if (eh != 0)
            {
              ACE_Reactor *reactor = eh->reactor ();
              if (reactor != 0)
                {
                  (void) reactor->cancel_timer (this->flush_timer_id_);
                }
            }
          this->reset_flush_timer ();
        }
      return 1;
    }

  return 0;
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
                      "TAO (%P|%t) - Transport[%d]::cleanup_queue, "
                      "byte_count = %d\n",
                      this->id (), byte_count));
        }

      // Update the state of the first message
      i->bytes_transferred (byte_count);

      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::cleanup_queue, "
                      "after transfer, bc = %d, all_sent = %d, ml = %d\n",
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
      if (eh != 0)
        {
          ACE_Reactor *reactor = eh->reactor ();
          if (reactor != 0)
            {
              this->current_deadline_ = new_deadline;
              ACE_Time_Value delay =
                new_deadline - ACE_OS::gettimeofday ();

              if (this->flush_timer_pending ())
                {
                  (void) reactor->cancel_timer (this->flush_timer_id_);
                }
              this->flush_timer_id_ =
                reactor->schedule_timer (&this->transport_timer_,
                                         &this->current_deadline_,
                                         delay);
            }
        }
    }

  return constraints_reached;
}

void
TAO_Transport::report_invalid_event_handler (const char *caller)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::report_invalid_event_handler"
                  "(%s) no longer associated with handler [tag=%d]\n",
                  this->id (), caller, this->tag_));
    }
}

TAO_Connection_Handler *
TAO_Transport::invalidate_event_handler (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->handler_lock_, 0));

  return this->invalidate_event_handler_i ();
}

void
TAO_Transport::send_connection_closed_notifications (void)
{
  ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->handler_lock_));

  this->send_connection_closed_notifications_i ();
}

void
TAO_Transport::send_connection_closed_notifications_i (void)
{
  while (this->head_ != 0)
    {
      TAO_Queued_Message *i = this->head_;

      // @@ This is a good point to insert a flag to indicate that a
      //    CloseConnection message was successfully received.
      i->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);

      this->head_ = i->next ();

      i->destroy ();
    }

  this->tms ()->connection_closed ();
  this->messaging_object ()->reset ();
}

int
TAO_Transport::send_message_shared_i (TAO_Stub *stub,
                                      int message_semantics,
                                      const ACE_Message_Block *message_block,
                                      ACE_Time_Value *max_wait_time)
{
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


  // Let's figure out if the message should be queued without trying
  // to send first:
  int try_sending_first = 1;

  int queue_empty = (this->head_ == 0);

  if (!queue_empty)
    try_sending_first = 0;
  else if (stub->sync_strategy ().must_queue (queue_empty))
    try_sending_first = 0;

  ssize_t n;

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();

  if (try_sending_first)
    {
      size_t byte_count = 0;
      // ... in this case we must try to send the message first ...

      size_t total_length = message_block->total_length ();
      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::send_message_i, "
                      "trying to send the message (ml = %d)\n",
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
                              "TAO (%P|%t) - Transport[%d]::send_message_i, "
                              "fatal error in "
                              "send_message_block_chain_i %p\n",
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
                      "TAO (%P|%t) - Transport[%d]::send_message_i, "
                      "partial send %d / %d bytes\n",
                      this->id (), byte_count, total_length));
        }

      // ... part of the data was sent, need to figure out what piece
      // of the message block chain must be queued ...
      while (message_block != 0 && message_block->length () == 0)
        message_block = message_block->cont ();

      // ... at least some portion of the message block chain should
      // remain ...
      ACE_ASSERT (message_block != 0);
    }

  // ... either the message must be queued or we need to queue it
  // because it was not completely sent out ...

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::send_message_i, "
                  "message is queued\n",
                  this->id ()));
    }

  TAO_Queued_Message *queued_message = 0;
  ACE_NEW_RETURN (queued_message,
                  TAO_Asynch_Queued_Message (message_block,
                                             0,
                                             1),
                  -1);
  queued_message->push_back (this->head_, this->tail_);

  // ... if the queue is full we need to activate the output on the
  // queue ...
  int must_flush = 0;
  int constraints_reached =
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



/*
 *
 * All the methods relevant to the incoming data path of the ORB are
 * defined below
 *
 */
int
TAO_Transport::handle_input_i (TAO_Resume_Handle &rh,
                               ACE_Time_Value * max_wait_time,
                               int /*block*/)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::handle_input_i\n",
                  this->id ()));
    }

  // First try to process messages of the head of the incoming queue.
  int retval = this->process_queue_head (rh);

  if (retval <= 0)
    {
      if (retval == -1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Transport[%d]::handle_input_i, "
                        "error while parsing the head of the queue\n",
                        this->id()));

          this->send_connection_closed_notifications ();
        }
      return retval;
    }

  // If there are no messages then we can go ahead to read from the
  // handle for further reading..

  // The buffer on the stack which will be used to hold the input
  // messages
  char buf [TAO_MAXBUFSIZE];

#if defined (ACE_HAS_PURIFY)
  (void) ACE_OS::memset (buf,
                         '\0',
                         sizeof buf);
#endif /* ACE_HAS_PURIFY */

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

  size_t recv_size = 0;
  if (this->orb_core_->orb_params ()->single_read_optimization ())
    {
      recv_size =
        message_block.space ();
    }
  else
    {
      recv_size =
        this->messaging_object ()->header_length ();
    }

  // Read the message into the  message block that we have created on
  // the stack.
  ssize_t n = this->recv (message_block.rd_ptr (),
                          recv_size,
                          max_wait_time);

  // If there is an error return to the reactor..
  if (n <= 0)
    {
      if (n == -1)
        {
          this->send_connection_closed_notifications ();
        }

      return n;
    }

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::handle_input_i, "
                  "read %d bytes\n",
                  this->id (), n));
    }

  // Set the write pointer in the stack buffer
  message_block.wr_ptr (n);

  // Parse the message and try consolidating the message if
  // needed.
  retval = this->parse_consolidate_messages (message_block,
                                             rh,
                                             max_wait_time);

  if (retval <= 0)
    {
      if (retval == -1 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::handle_input_i, "
                      "error while parsing and consolidating\n",
                      this->id ()));
        }
      return retval;
    }

  // Make a node of the message block..
  TAO_Queued_Data qd (&message_block,
                      this->orb_core_->transport_message_buffer_allocator ());

  // Extract the data for the node..
  this->messaging_object ()->get_message_data (&qd);

  // Check whether the message was fragmented..
  if (qd.more_fragments_ ||
      (qd.msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT))
    {
      // Duplicate the node that we have as the node is on stack..
      TAO_Queued_Data *nqd =
        TAO_Queued_Data::duplicate (qd);

      return this->consolidate_fragments (nqd, rh);
    }

  // Process the message
  return this->process_parsed_messages (&qd,
                                        rh);
}

int
TAO_Transport::parse_consolidate_messages (ACE_Message_Block &block,
                                           TAO_Resume_Handle &rh,
                                           ACE_Time_Value *max_wait_time)
{
  // Parse the incoming message for validity. The check needs to be
  // performed by the messaging objects.
  if (this->parse_incoming_messages (block) == -1)
    return -1;

  // Check whether we have a complete message for processing
  ssize_t missing_data = this->missing_data (block);


  if (missing_data < 0)
    {
      // If we have more than one message
      return this->consolidate_extra_messages (block,
                                               rh);
    }
  else if (missing_data > 0)
    {
      // If we have missing data then try doing a read or try queueing
      // them.
      return this->consolidate_message (block,
                                        missing_data,
                                        rh,
                                        max_wait_time);
    }

  return 1;
}

int
TAO_Transport::parse_incoming_messages (ACE_Message_Block &block)
{
  // If we have a queue and if the last message is not complete a
  // complete one, then this read will get us the remaining data. So
  // do not try to parse the header if we have an incomplete message
  // in the queue.
  if (this->incoming_message_queue_.is_tail_complete () != 0)
    {
      //  As it looks like a new message has been read, process the
      //  message. Call the messaging object to do the parsing..
      int retval =
        this->messaging_object ()->parse_incoming_messages (block);

      if (retval == -1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Transport[%d]::parse_incoming_messages, "
                        "error in incoming message\n",
                        this->id ()));

          this->send_connection_closed_notifications ();
          return -1;
        }
    }

  return 0;
}


size_t
TAO_Transport::missing_data (ACE_Message_Block &incoming)
{
  // If we have a incomplete message in the queue then find out how
  // much of data is required to get a complete message.
  if (this->incoming_message_queue_.is_tail_complete () == 0)
    {
      return this->incoming_message_queue_.missing_data_tail ();
    }

  return this->messaging_object ()->missing_data (incoming);
}


int
TAO_Transport::consolidate_message (ACE_Message_Block &incoming,
                                    ssize_t missing_data,
                                    TAO_Resume_Handle &rh,
                                    ACE_Time_Value *max_wait_time)
{
  // Check whether the last message in the queue is complete..
  if (this->incoming_message_queue_.is_tail_complete () == 0)
    return this->consolidate_message_queue (incoming,
                                            missing_data,
                                            rh,
                                            max_wait_time);

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_message\n",
                  this->id ()));
    }

  // Calculate the actual length of the load that we are supposed to
  // read which is equal to the <missing_data> + length of the buffer
  // that we have..
  size_t payload = missing_data + incoming.size ();

  // Grow the buffer to the size of the message
  ACE_CDR::grow (&incoming,
                 payload);

  ssize_t n = 0;

  // As this used for transports where things are available in one
  // shot this looping should not create any problems.
  for (ssize_t bytes = missing_data;
       bytes != 0;
       bytes -= n)
    {
      // .. do a read on the socket again.
      n = this->recv (incoming.wr_ptr (),
                      bytes,
                      max_wait_time);

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::consolidate_message, "
                      "read %d bytes on attempt\n",
                      this->id(), n));
        }

      if (n == 0 || n == -1)
        {
          break;
        }

      incoming.wr_ptr (n);
      missing_data -= n;
    }

  // If we got an error..
  if (n == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Trasport[%d]::consolidate_message, "
                      "error while trying to consolidate\n",
                      this->id ()));
        }
      this->send_connection_closed_notifications ();
      return -1;
    }

  // If we had gotten a EWOULDBLOCK n would be equal to zero. But we
  // have to put the message in the queue anyway. So let us proceed
  // to do that and return...

  // Check to see if we have messages in queue or if we have missing
  // data . AT this point we cannot have have semi-complete messages
  // in the queue as they would have been taken care before. Put
  // ourselves in the queue and  then try processing one of the
  // messages..
  if ((missing_data > 0
       ||this->incoming_message_queue_.queue_length ())
      && this->incoming_message_queue_.is_tail_fragmented () == 0)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::consolidate_message, "
                      "queueing up the message\n",
                      this->id ()));
        }

      // Get a queued data
      TAO_Queued_Data *qd =
        this->make_queued_data (incoming);

      // Add the missing data to the queue
      qd->missing_data_ = missing_data;

      // Get the rest of the messaging data
      this->messaging_object ()->get_message_data (qd);

      // Add it to the tail of the queue..
      this->incoming_message_queue_.enqueue_tail (qd);

      if (this->incoming_message_queue_.is_head_complete ())
        return this->process_queue_head (rh);

      return 0;
    }

  // We dont have any missing data. Just make a queued_data node with
  // the existing message block and send it to the higher layers of
  // the ORB.
  TAO_Queued_Data pqd (&incoming,
                       this->orb_core_->transport_message_buffer_allocator ());
  pqd.missing_data_ = missing_data;
  this->messaging_object ()->get_message_data (&pqd);

  // Check whether the message was fragmented and try to consolidate
  // the fragments..
  if (pqd.more_fragments_ ||
      (pqd.msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT))
    {
      // Duplicate the queued data as it is on stack..
      TAO_Queued_Data *nqd = TAO_Queued_Data::duplicate (pqd);

      return this->consolidate_fragments (nqd, rh);
    }

  // Now we have a full message in our buffer. Just go ahead and
  // process that
  return this->process_parsed_messages (&pqd,
                                        rh);
}

int
TAO_Transport::consolidate_fragments (TAO_Queued_Data *qd,
                                      TAO_Resume_Handle &rh)
{
  // If we have received a fragment message then we have to
  // consolidate <qd> with the last message in queue
  // @@todo: this piece of logic follows GIOP a bit... Need to revisit
  // if we have protocols other than GIOP

  // @@todo: Fragments now have too much copying overhead. Need to get
  // them out if we want to have some reasonable performance metrics
  // in future.. Post 1.2 seems a nice time..
  if (qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT)
    {
      TAO_Queued_Data *tqd =
        this->incoming_message_queue_.dequeue_tail ();

      tqd->more_fragments_ = qd->more_fragments_;
      tqd->missing_data_ = qd->missing_data_;

      if (this->messaging_object ()->consolidate_fragments (tqd,
                                                            qd) == -1)
        return -1;


      TAO_Queued_Data::release (qd);

      this->incoming_message_queue_.enqueue_tail (tqd);

      this->process_queue_head (rh);
    }
  else
    {
      // if we dont have a fragment already in the queue just add it in
      // the queue
      this->incoming_message_queue_.enqueue_tail (qd);
    }

  return 0;
}

int
TAO_Transport::consolidate_message_queue (ACE_Message_Block &incoming,
                                          ssize_t missing_data,
                                          TAO_Resume_Handle &rh,
                                          ACE_Time_Value *max_wait_time)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_message_queue\n",
                  this->id ()));
    }

  // If the queue did not have a complete message put this piece of
  // message in the queue. We know it did not have a complete
  // message. That is why we are here.
  size_t n =
    this->incoming_message_queue_.copy_tail (incoming);

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_message_queue, "
                  "copied [%d] bytes to the tail\n",
                  this->id (),
                  n));
    }

  // Update the missing data...
  missing_data =
    this->incoming_message_queue_.missing_data_tail ();

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_message_queue, "
                  "missing [%d] bytes in the tail message\n",
                  this->id (),
                  missing_data));
    }

  // Move the read pointer of the <incoming> message block to the end
  // of the copied message and process the remaining portion...
  incoming.rd_ptr (n);

  // If we have some more information left in the message block..
  if (incoming.length ())
    {
      // We may have to parse & consolidate. This part of the message
      // doesn't seem to be part of the last message in the queue (as
      // the copy () hasn't taken away this message).
      int retval = this->parse_consolidate_messages (incoming,
                                                     rh,
                                                     max_wait_time);

      // If there is an error return
      if (retval == -1)
        {
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) - Transport[%d]::consolidate_message_queue, "
                          "error while consolidating, part of the read message\n",
                          this->id ()));
            }
          return retval;
        }
      else if (retval == 1)
        {
          // If the message in the <incoming> message block has only
          // one message left we need to process that seperately.

          // Get a queued data
          TAO_Queued_Data *qd = this->make_queued_data (incoming);

          // Get the rest of the message data
          this->messaging_object ()->get_message_data (qd);

          // Add the missing data to the queue
          qd->missing_data_ = 0;

          // Check whether the message was fragmented and try to consolidate
          // the fragments..
          if (qd->more_fragments_ ||
              (qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT))
            {
              return this->consolidate_fragments (qd, rh);
            }

          // Add it to the tail of the queue..
          this->incoming_message_queue_.enqueue_tail (qd);

          // We should surely have a message in queue now. So just
          // process that.
          return this->process_queue_head (rh);
        }

      // parse_consolidate_messages () would have processed one of the
      // messages, so we better return as we dont want to starve other
      // threads.
      return 0;
    }

  // If we still have some missing data..
  if (missing_data > 0)
    {
      // Get the last message from the Queue
      TAO_Queued_Data *qd =
        this->incoming_message_queue_.dequeue_tail ();

      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport[%d]::consolidate_message_queue, "
                    "trying recv, again\n",
                    this->id ()));

      // Try to do a read again. If we have some luck it would be
      // great..
      ssize_t n = this->recv (qd->msg_block_->wr_ptr (),
                              missing_data,
                              max_wait_time);

      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport[%d]::consolidate_message_queue, "
                    "recv retval [%d]\n",
                    this->id (),
                    n));
      // Error...
      if (n < 0)
        return n;

      // If we get a EWOULDBLOCK ie. n==0, we should anyway put the
      //  message in queue before returning..
      // Move the write pointer
      qd->msg_block_->wr_ptr (n);

      // Decrement the missing data
      qd->missing_data_ -= n;

      // Now put the TAO_Queued_Data back in the queue
      this->incoming_message_queue_.enqueue_tail (qd);

      // Any way as we have come this far and are about to return,
      // just try to process a message if it is there in the queue.
      if (this->incoming_message_queue_.is_head_complete ())
        return this->process_queue_head (rh);

      return 0;
    }

  // Process a message in the head of the queue if we have one..
  return this->process_queue_head (rh);
}


int
TAO_Transport::consolidate_extra_messages (ACE_Message_Block
                                           &incoming,
                                           TAO_Resume_Handle &rh)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_extra_messages\n",
                  this->id ()));
    }

  // Pick the tail of the queue
  TAO_Queued_Data *tail =
    this->incoming_message_queue_.dequeue_tail ();

  if (tail)
    {
      // If we have a node in the tail, checek to see whether it needs
      // consolidation. If so, just consolidate it.
      if (this->messaging_object ()->consolidate_node (tail,
                                                       incoming) == -1)
        return -1;

      // .. put the tail back in queue..
      this->incoming_message_queue_.enqueue_tail (tail);
    }

  int retval = 1;

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::consolidate_extra_messages, "
                  "extracting extra messages\n",
                  this->id ()));
    }

  // Extract messages..
  while (retval == 1)
    {
      TAO_Queued_Data *q_data = 0;

      retval =
        this->messaging_object ()->extract_next_message (incoming,
                                                         q_data);
      if (q_data)
        {
          // If we have read a framented message then...
          if (q_data->more_fragments_ ||
              q_data->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT)
            {
              this->consolidate_fragments (q_data, rh);
            }
          else
            {
              this->incoming_message_queue_.enqueue_tail (q_data);
            }
        }
    }

  // In case of error return..
  if (retval == -1)
    {
      return retval;
    }

  return this->process_queue_head (rh);
}

int
TAO_Transport::process_parsed_messages (TAO_Queued_Data *qd,
                                        TAO_Resume_Handle &rh)
{
  // Get the <message_type> that we have received
  TAO_Pluggable_Message_Type t =  qd->msg_type_;

  int result = 0;
  if (t == TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport[%d]::process_parsed_messages, "
                    "received CloseConnection message %p\n",
                    this->id(), ""));

      this->send_connection_closed_notifications ();

      // Return a "-1" so that the next stage can take care of
      // closing connection and the necessary memory management.
      return -1;
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REQUEST ||
           t == TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST)
    {
      // Ready to process a request. Increment the refcount of <this
      // transport>. Theoretically, after handler resumption another
      // thread can access this very same transport and can even close
      // this. To have a valid Transport object for further processing
      // we should increment the refcount. Please see Bug 1382 for
      // more details.
      // REFCNT: Matched by the release before returning.

      // This generic class takes care of everything.
      TAO_Transport_Refcount_Guard rg (this);

      // Let us resume the handle before we go ahead to process the
      // request. This will open up the handle for other threads.
      rh.resume_handle ();

      if (this->messaging_object ()->process_request_message (
            this,
            qd) == -1)
        {
          this->send_connection_closed_notifications ();


          // Return a "-1" so that the next stage can take care of
          // closing connection and the necessary memory management.
          return -1;
        }
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REPLY ||
           t == TAO_PLUGGABLE_MESSAGE_LOCATEREPLY)
    {
      // Please see ..else if (XXX_REQUEST) for whys and whats..
      TAO_Transport_Refcount_Guard rg (this);
      rh.resume_handle ();

      // @@todo: Maybe the input_cdr can be constructed from the
      // message_block
      TAO_Pluggable_Reply_Params params (this->orb_core ());


      if (this->messaging_object ()->process_reply_message (params,
                                                            qd) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Transport[%d]::process_parsed_messages, "
                        "error in process_reply_message %p\n",
                        this->id (), ""));

          this->send_connection_closed_notifications ();
          return -1;
        }

      result = this->tms ()->dispatch_reply (params);

      if (result == -1)
        {
          // Something really critical happened, we will forget about
          // every reply on this connection.
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - Transport[%d]::process_parsed_messages, "
                        "dispatch reply failed\n",
                        this->id ()));

          this->send_connection_closed_notifications ();
          return -1;
        }

    }
  else if (t == TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
    {
      if (TAO_debug_level)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "TAO (%P|%t) - Transport[%d]::process_parsed_messages, "
                             "received MessageError, closing connection\n",
                             this->id ()),
                            -1);
        }
    }

  // If not, just return back..
  return 0;
}

TAO_Queued_Data *
TAO_Transport::make_queued_data (ACE_Message_Block &incoming)
{
  // Get an instance of TAO_Queued_Data
  TAO_Queued_Data *qd =
    TAO_Queued_Data::get_queued_data (
      this->orb_core_->transport_message_buffer_allocator ());

  // Get the flag for the details of the data block...
  ACE_Message_Block::Message_Flags flg =
    incoming.self_flags ();

  if (ACE_BIT_DISABLED (flg,
                        ACE_Message_Block::DONT_DELETE))
    {
      // Duplicate the data block before putting it in the queue.
      qd->msg_block_ = ACE_Message_Block::duplicate (&incoming);
    }
  else
    {
      // As we are in CORBA mode, all the data blocks would be aligned
      // on an 8 byte boundary. Hence create a data block for more
      // than the actual length
      ACE_Data_Block *db =
        this->orb_core_->create_input_cdr_data_block (incoming.length ()+
                                                      ACE_CDR::MAX_ALIGNMENT);

      // Get the allocator..
      ACE_Allocator *alloc =
        this->orb_core_->input_cdr_msgblock_allocator ();

      // Make message block..
      ACE_Message_Block mb (db,
                            0,
                            alloc);

      // Duplicate the block..
      qd->msg_block_ = mb.duplicate ();

      // Align the message block
      ACE_CDR::mb_align (qd->msg_block_);

      // Copy the data..
      qd->msg_block_->copy (incoming.rd_ptr (),
                            incoming.length ());
    }


  return qd;
}

int
TAO_Transport::process_queue_head (TAO_Resume_Handle &rh)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::process_queue_head\n",
                  this->id ()));
    }

  // See if the message in the head of the queue is complete...
  if (this->incoming_message_queue_.is_head_complete () > 0)
    {
      // Get the message on the head of the queue..
      TAO_Queued_Data *qd =
        this->incoming_message_queue_.dequeue_head ();

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::process_queue_head, "
                      "the size of the queue is [%d]\n",
                      this->id (),
                      this->incoming_message_queue_.queue_length()));
        }
      // Now that we have pulled out out one message out of the queue,
      // check whether we have one more message in the queue...
      if (this->incoming_message_queue_.is_head_complete () > 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) - Transport[%d]::process_queue_head, "
                          "notify reactor\n",
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
      if (this->process_parsed_messages (qd,
                                         rh) == -1)
        return -1;

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
    return 0;

  ACE_Event_Handler *eh =
    this->event_handler_i ();

  if (eh == 0)
    return -1;

  // Get the reactor associated with the event handler
  ACE_Reactor *reactor =
    this->orb_core ()->reactor ();

  if (reactor == 0)
    return -1;

  // NOTE: Instead of creating the handler seperately, it would be
  // awesome if we  could create the handler when we create the
  // TAO_Queued_Data. That would save us an allocation.
  TAO_Notify_Handler *nh =
    TAO_Notify_Handler::create_handler (
        this,
        eh->get_handle (),
        this->orb_core ()->transport_message_buffer_allocator ());

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::notify_reactor, "
                  "notify to Reactor\n",
                  this->id ()));
    }


  // Send a notification to the reactor...
  int retval = reactor->notify (nh,
                                ACE_Event_Handler::READ_MASK);

  if (retval < 0 && TAO_debug_level > 2)
    {
      // @@todo: need to think about what is the action that
      // we can take when we get here.
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::process_queue_head, "
                  "notify to the reactor failed..\n",
                  this->id ()));
    }

  return 1;
}

TAO_Transport_Cache_Manager &
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



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Reverse_Lock<ACE_Lock>;
template class ACE_Guard<ACE_Reverse_Lock<ACE_Lock> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Reverse_Lock<ACE_Lock>
#pragma instantiate ACE_Guard<ACE_Reverse_Lock<ACE_Lock> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
