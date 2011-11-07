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
#include "tao/GIOP_Message_Base.h"
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
#include "tao/MMAP_Allocator.h"
#include "tao/SystemException.h"
#include "tao/operation_details.h"
#include "tao/Transport_Descriptor_Interface.h"

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Reactor.h"
#include "ace/os_include/sys/os_uio.h"
#include "ace/High_Res_Timer.h"
#include "ace/Countdown_Time.h"
#include "ace/CORBA_macros.h"

/*
 * Specialization hook to add include files from
 * concrete transport implementation.
 */
//@@ TAO_TRANSPORT_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

#if !defined (__ACE_INLINE__)
# include "tao/Transport.inl"
#endif /* __ACE_INLINE__ */

/*
 * Static function in file scope
 */
static void
dump_iov (iovec *iov, int iovcnt, size_t id,
          size_t current_transfer,
          const ACE_TCHAR *location)
{
  ACE_GUARD (ACE_Log_Msg, ace_mon, *ACE_Log_Msg::instance ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::%s, ")
              ACE_TEXT ("sending %d buffers\n"),
              id, location, iovcnt));

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
                  id, location,
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
              id, location));
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if TAO_HAS_TRANSPORT_CURRENT == 1
TAO::Transport::Stats::~Stats ()
{
  // no-op
}
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core,
                              size_t input_cdr_size)
  : tag_ (tag)
  , orb_core_ (orb_core)
  , cache_map_entry_ (0)
  , tms_ (0)
  , ws_ (0)
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
  , messaging_object_ (0)
  , char_translator_ (0)
  , wchar_translator_ (0)
  , tcs_set_ (0)
  , first_request_ (true)
  , partial_message_ (0)
#if TAO_HAS_SENDFILE == 1
    // The ORB has been configured to use the MMAP allocator, meaning
    // we could/should use sendfile() to send data.  Cast once rather
    // here rather than during each send.  This assumes that all
    // TAO_OutputCDR instances are using the same TAO_MMAP_Allocator
    // instance as the underlying output CDR buffer allocator.
  , mmap_allocator_ (
      dynamic_cast<TAO_MMAP_Allocator *> (
        orb_core->output_cdr_buffer_allocator ()))
#endif  /* TAO_HAS_SENDFILE==1 */
#if TAO_HAS_TRANSPORT_CURRENT == 1
  , stats_ (0)
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */
  , flush_in_post_open_ (false)
{
  ACE_NEW (this->messaging_object_,
            TAO_GIOP_Message_Base (orb_core,
                                   this,
                                   input_cdr_size));

  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);

#if TAO_HAS_TRANSPORT_CURRENT == 1
  // Allocate stats
  ACE_NEW_THROW_EX (this->stats_,
                    TAO::Transport::Stats,
                    CORBA::NO_MEMORY ());
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

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
  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport[%d]::~Transport\n"),
                  this->id_
                  ));
    }

  delete this->messaging_object_;

  delete this->ws_;

  delete this->tms_;

  delete this->handler_lock_;

  if (!this->is_connected_)
    {
      // When we have a not connected transport we could have buffered
      // messages on this transport which we have to cleanup now.
      this->cleanup_queue_i();
    }

  // Release the partial message block, however we may
  // have never allocated one.
  ACE_Message_Block::release (this->partial_message_);

  // By the time the destructor is reached here all the connection stuff
  // *must* have been cleaned up.

  // The following assert is needed for the test "Bug_2494_Regression".
  // See the bugzilla bug #2494 for details.
  ACE_ASSERT (this->queue_is_empty_i ());
  ACE_ASSERT (this->cache_map_entry_ == 0);

#if TAO_HAS_TRANSPORT_CURRENT == 1
  delete this->stats_;
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

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
                                    TAO_Message_Semantics message_semantics,
                                    const ACE_Message_Block *message_block,
                                    ACE_Time_Value *max_wait_time)
{
  int result = 0;

  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

    result =
      this->send_message_shared_i (stub, message_semantics,
                                   message_block, max_wait_time);
  }

  if (result == -1)
    {
      // The connection needs to be closed here.
      // In the case of a partially written message this is the only way to cleanup
      //  the physical connection as well as the Transport. An EOF on the remote end
      //  will cancel the partially received message.
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

bool
TAO_Transport::register_if_necessary (void)
{
  if (this->is_connected_ &&
      this->wait_strategy ()->register_handler () == -1)
    {
      // Registration failures.
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::register_if_necessary, ")
                      ACE_TEXT ("could not register the transport ")
                      ACE_TEXT ("in the reactor.\n"),
                      this->id ()));
        }

      // Purge from the connection cache, if we are not in the cache, this
      // just does nothing.
      (void) this->purge_entry ();

      // Close the handler.
      (void) this->close_connection ();

      return false;
    }
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

  ACE_Reactor * const r = this->orb_core_->reactor ();

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
  this->ws_->is_registered (true);

  // Register the handler with the reactor
  return r->register_handler (this->event_handler_i (),
                              ACE_Event_Handler::READ_MASK);
}

#if TAO_HAS_SENDFILE == 1
ssize_t
TAO_Transport::sendfile (TAO_MMAP_Allocator * /* allocator */,
                         iovec * iov,
                         int iovcnt,
                         size_t &bytes_transferred,
                         TAO::Transport::Drain_Constraints const & dc)
{
  // Concrete pluggable transport doesn't implement sendfile().
  // Fallback on TAO_Transport::send().

  // @@ We can probably refactor the TAO_IIOP_Transport::sendfile()
  //    implementation to this base class method, and leave any TCP
  //    specific configuration out of this base class method.
  //      -Ossama
  return this->send (iov, iovcnt, bytes_transferred,
                     this->io_timeout (dc));
}
#endif  /* TAO_HAS_SENDFILE==1 */

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
        {
          ACE_ERROR ((LM_ERROR,
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
  if (this->messaging_object ()->generate_request_header (opdetails,
                                                          spec,
                                                          output) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::generate_request_header, ")
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
  return this->transport_cache_manager ().cache_transport (desc, this);
}

int
TAO_Transport::purge_entry (void)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::purge_entry, ")
                  ACE_TEXT ("entry is %@\n"),
                  this->id (), this->cache_map_entry_));
    }

  return this->transport_cache_manager ().purge_entry (this->cache_map_entry_);
}

bool
TAO_Transport::can_be_purged (void)
{
  return !this->tms_->has_request ();
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

/**
 *  Methods called and used in the output path of the ORB.
 */
TAO_Transport::Drain_Result
TAO_Transport::handle_output (TAO::Transport::Drain_Constraints const & dc)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_output")
                  ACE_TEXT (" - block_on_io=%d, timeout=%d.%06d\n"),
                  this->id (),
                  dc.block_on_io(),
                  dc.timeout() ? dc.timeout()->sec() : static_cast<time_t> (-1),
                  dc.timeout() ? dc.timeout()->usec() : -1 ));
    }

  // The flushing strategy (potentially via the Reactor) wants to send
  // more data, first check if there is a current message that needs
  // more sending...
  Drain_Result const retval = this->drain_queue (dc);

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_output, ")
                  ACE_TEXT ("drain_queue returns %d/%d\n"),
                  this->id (),
                  static_cast<int> (retval.dre_), ACE_ERRNO_GET));
    }

  // Any errors are returned directly to the Reactor
  return retval;
}

int
TAO_Transport::format_queue_message (TAO_OutputCDR &stream,
                                     ACE_Time_Value *max_wait_time,
                                     TAO_Stub* stub)
{
  if (this->messaging_object ()->format_message (stream, stub) != 0)
    return -1;

  return this->queue_message_i (stream.begin (), max_wait_time);
}

int
TAO_Transport::send_message_block_chain (const ACE_Message_Block *mb,
                                         size_t &bytes_transferred,
                                         ACE_Time_Value *max_wait_time)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

  TAO::Transport::Drain_Constraints dc(
      max_wait_time, true);

  return this->send_message_block_chain_i (mb,
                                           bytes_transferred,
                                           dc);
}

int
TAO_Transport::send_message_block_chain_i (const ACE_Message_Block *mb,
                                           size_t &bytes_transferred,
                                           TAO::Transport::Drain_Constraints const & dc)
{
  size_t const total_length = mb->total_length ();

  // We are going to block, so there is no need to clone
  // the message block.
  TAO_Synch_Queued_Message synch_message (mb, this->orb_core_);

  synch_message.push_back (this->head_, this->tail_);

  Drain_Result const n = this->drain_queue_i (dc);

  if (n == DR_ERROR)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      return -1; // Error while sending...
    }
  else if (n == DR_QUEUE_EMPTY)
    {
      bytes_transferred = total_length;
      return 1;  // Empty queue, message was sent..
    }

  // Remove the temporary message from the queue...
  synch_message.remove_from_list (this->head_, this->tail_);

  bytes_transferred = total_length - synch_message.message_length ();

  return 0;
}

int
TAO_Transport::send_synchronous_message_i (const ACE_Message_Block *mb,
                                           ACE_Time_Value *max_wait_time)
{
  // We are going to block, so there is no need to clone
  // the message block.
  size_t const total_length = mb->total_length ();
  TAO_Synch_Queued_Message synch_message (mb, this->orb_core_);

  synch_message.push_back (this->head_, this->tail_);

  int const result = this->send_synch_message_helper_i (synch_message,
                                                        max_wait_time);
  if (result == -1 && errno == ETIME)
    {
      if (total_length == synch_message.message_length ()) //none was sent
        {
          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("Transport[%d]::send_synchronous_message_i, ")
                          ACE_TEXT ("timeout encountered before any bytes sent\n"),
                          this->id ()));
            }
          throw ::CORBA::TIMEOUT (
            CORBA::SystemException::_tao_minor_code (
              TAO_TIMEOUT_SEND_MINOR_CODE,
              ETIME),
            CORBA::COMPLETED_NO);
        }
      else
        {
          return -1;
        }
    }
  else if(result == -1 || result == 1)
    {
      return result;
    }

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();
  if (flushing_strategy->schedule_output (this) == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::")
                      ACE_TEXT ("send_synchronous_message_i, ")
                      ACE_TEXT ("error while scheduling flush - %m\n"),
                      this->id ()));
        }
      return -1;
    }

  // No need to check for result == TAO_Flushing_Strategy::MUST_FLUSH,
  // because we're always going to flush anyway.

  // Release the mutex, other threads may modify the queue as we
  // block for a long time writing out data.
  int flush_result;
  {
    typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
    TAO_REVERSE_LOCK reverse (*this->handler_lock_);
    ACE_GUARD_RETURN (TAO_REVERSE_LOCK, ace_mon, reverse, -1);

    flush_result = flushing_strategy->flush_message (this,
                                                     &synch_message,
                                                     max_wait_time);
  }

  if (flush_result == -1)
    {
      synch_message.remove_from_list (this->head_, this->tail_);

      // We don't need to do anything special for the timeout case.
      // The connection is going to get closed and the Transport destroyed.
      // The only thing to do maybe is to empty the queue.

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_synchronous_message_i, ")
             ACE_TEXT ("error while sending message - %m\n"),
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
  // Don't clone now.. We could be sent in one shot!
  TAO_Synch_Queued_Message synch_message (mb, this->orb_core_);

  synch_message.push_back (this->head_, this->tail_);

  int const n =
    this->send_synch_message_helper_i (synch_message, max_wait_time);

  // What about partially sent messages.
  if (n == -1 || n == 1)
    {
      return n;
    }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_reply_message_i, ")
         ACE_TEXT ("preparing to add to queue before leaving\n"),
         this->id ()));
    }

  // Till this point we shouldn't have any copying and that is the
  // point anyway. Now, remove the node from the list
  synch_message.remove_from_list (this->head_, this->tail_);

  // Clone the node that we have.
  TAO_Queued_Message *msg =
    synch_message.clone (this->orb_core_->transport_message_buffer_allocator ());

  // Stick it in the queue
  msg->push_back (this->head_, this->tail_);

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();

  int const result = flushing_strategy->schedule_output (this);

  if (result == -1)
    {
      if (TAO_debug_level > 5)
        {
          ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - Transport[%d]::send_reply_"
                      "message_i, dequeuing msg due to schedule_output "
                      "failure\n", this->id ()));
        }
      msg->remove_from_list (this->head_, this->tail_);
      msg->destroy ();
    }
  else if (result == TAO_Flushing_Strategy::MUST_FLUSH)
    {
      typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
      TAO_REVERSE_LOCK reverse (*this->handler_lock_);
      ACE_GUARD_RETURN (TAO_REVERSE_LOCK, ace_mon, reverse, -1);
      (void) flushing_strategy->flush_transport (this, 0);
    }

  return 1;
}

int
TAO_Transport::send_synch_message_helper_i (TAO_Synch_Queued_Message &synch_message,
                                            ACE_Time_Value * max_wait_time)
{
  TAO::Transport::Drain_Constraints dc(
      max_wait_time, this->using_blocking_io_for_synch_messages());

  Drain_Result const n = this->drain_queue_i (dc);

  if (n == DR_ERROR)
    {
      synch_message.remove_from_list (this->head_, this->tail_);
      return -1; // Error while sending...
    }
  else if (n == DR_QUEUE_EMPTY)
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
TAO_Transport::schedule_output_i (void)
{
  ACE_Event_Handler * const eh = this->event_handler_i ();
  ACE_Reactor * const reactor = eh->reactor ();

  if (reactor == 0)
    {
      if (TAO_debug_level > 1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ")
                      ACE_TEXT ("Transport[%d]::schedule_output_i, ")
                      ACE_TEXT ("no reactor,")
                      ACE_TEXT ("returning -1\n"),
                      this->id ()));
        }
      return -1;
    }

  // Check to see if our event handler is still registered with the
  // reactor.  It's possible for another thread to have run close_connection()
  // since we last used the event handler.
  ACE_Event_Handler * const found = reactor->find_handler (eh->get_handle ());
  if (found)
    {
      found->remove_reference ();

      if (found != eh)
        {
          if (TAO_debug_level > 3)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("Transport[%d]::schedule_output_i ")
                          ACE_TEXT ("event handler not found in reactor,")
                          ACE_TEXT ("returning -1\n"),
                          this->id ()));
            }

          return -1;
        }
    }

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
  ACE_Event_Handler * const eh = this->event_handler_i ();
  ACE_Reactor *const reactor = eh->reactor ();

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
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_timeout, ")
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
      int const result = flushing_strategy->schedule_output (this);
      if (result == TAO_Flushing_Strategy::MUST_FLUSH)
        {
          typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
          TAO_REVERSE_LOCK reverse (*this->handler_lock_);
          ACE_GUARD_RETURN (TAO_REVERSE_LOCK, ace_mon, reverse, -1);
          if (flushing_strategy->flush_transport (this, 0) == -1) {
            return -1;
          }
        }
    }

  return 0;
}

TAO_Transport::Drain_Result
TAO_Transport::drain_queue (TAO::Transport::Drain_Constraints const & dc)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, DR_ERROR);
  Drain_Result const retval = this->drain_queue_i (dc);

  if (retval == DR_QUEUE_EMPTY)
    {
      // ... there is no current message or it was completely
      // sent, cancel output...
      TAO_Flushing_Strategy *flushing_strategy =
        this->orb_core ()->flushing_strategy ();

      flushing_strategy->cancel_output (this);

      return DR_OK;
    }

  return retval;
}

TAO_Transport::Drain_Result
TAO_Transport::drain_queue_helper (int &iovcnt, iovec iov[],
    TAO::Transport::Drain_Constraints const & dc)
{
  // As a side-effect, this decrements the timeout() pointed-to value by
  // the time used in this function.  That might be important as there are
  // potentially long running system calls invoked from here.
  ACE_Countdown_Time countdown(dc.timeout());

  size_t byte_count = 0;

  // ... send the message ...
  ssize_t retval = -1;

#if TAO_HAS_SENDFILE == 1
  if (this->mmap_allocator_)
    retval = this->sendfile (this->mmap_allocator_,
                             iov,
                             iovcnt,
                             byte_count,
                             dc);
  else
#endif  /* TAO_HAS_SENDFILE==1 */
    retval = this->send (iov, iovcnt, byte_count,
                         this->io_timeout (dc));

  if (TAO_debug_level > 9)
    {
      dump_iov (iov, iovcnt, this->id (),
                byte_count, ACE_TEXT("drain_queue_helper"));
    }

  if (retval == 0)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
             ACE_TEXT ("send() returns 0\n"),
             this->id ()));
        }
      return DR_ERROR;
    }
  else if (retval == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
             ACE_TEXT ("error during send() (errno: %d) - %m\n"),
             this->id (), ACE_ERRNO_GET));
        }

      if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
          return DR_WOULDBLOCK;
        }

      return DR_ERROR;
    }

  // ... now we need to update the queue, removing elements
  // that have been sent, and updating the last element if it
  // was only partially sent ...
  this->cleanup_queue (byte_count);
  iovcnt = 0;

  // ... start over, how do we guarantee progress?  Because if
  // no bytes are sent send() can only return 0 or -1

  // Total no. of bytes sent for a send call
  this->sent_byte_count_ += byte_count;

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_helper, ")
         ACE_TEXT ("byte_count = %d, head_is_empty = %d\n"),
         this->id(), byte_count, this->queue_is_empty_i ()));
    }

  return DR_QUEUE_EMPTY;
  // drain_queue_i will check if the queue is actually empty
}

TAO_Transport::Drain_Result
TAO_Transport::drain_queue_i (TAO::Transport::Drain_Constraints const & dc)
{
  // This is the vector used to send data, it must be declared outside
  // the loop because after the loop there may still be data to be
  // sent
  int iovcnt = 0;
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  iovec iov[ACE_IOV_MAX] = { { 0 , 0 } };
#else
  iovec iov[ACE_IOV_MAX];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

  // We loop over all the elements in the queue ...
  TAO_Queued_Message *i = this->head_;

  // Reset the value so that the counting is done for each new send
  // call.
  this->sent_byte_count_ = 0;

  // Avoid calling this expensive function each time through the loop. Instead
  // we'll assume that the time is unlikely to change much during the loop.
  // If we are forced to send in the loop then we'll recompute the time.
  ACE_Time_Value now = ACE_High_Res_Timer::gettimeofday_hr ();

  while (i != 0)
    {
      if (i->is_expired (now))
        {
          if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
              ACE_TEXT ("Discarding expired queued message.\n"),
              this->id ()));
          }
          TAO_Queued_Message *next = i->next ();
          i->state_changed (TAO_LF_Event::LFS_TIMEOUT,
                            this->orb_core_->leader_follower ());
          i->remove_from_list (this->head_, this->tail_);
          i->destroy ();
          i = next;
          continue;
        }
      // ... each element fills the iovector ...
      i->fill_iov (ACE_IOV_MAX, iovcnt, iov);

      // ... the vector is full, no choice but to send some data out.
      // We need to loop because a single message can span multiple
      // IOV_MAX elements ...
      if (iovcnt == ACE_IOV_MAX)
        {
          Drain_Result const retval =
            this->drain_queue_helper (iovcnt, iov, dc);

          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
                 ACE_TEXT ("helper retval = %d\n"),
                 this->id (), static_cast<int> (retval.dre_)));
            }

          if (retval != DR_QUEUE_EMPTY)
            {
              return retval;
            }

          now = ACE_High_Res_Timer::gettimeofday_hr ();

          i = this->head_;
          continue;
        }
      // ... notice that this line is only reached if there is still
      // room in the iovector ...
      i = i->next ();
    }

  if (iovcnt != 0)
    {
      Drain_Result const retval = this->drain_queue_helper (iovcnt, iov, dc);

      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport[%d]::drain_queue_i, ")
              ACE_TEXT ("helper retval = %d\n"),
              this->id (), static_cast<int> (retval.dre_)));
        }

      if (retval != DR_QUEUE_EMPTY)
        {
          return retval;
        }
    }

  if (this->queue_is_empty_i ())
    {
      if (this->flush_timer_pending ())
        {
          ACE_Event_Handler *eh = this->event_handler_i ();
          ACE_Reactor * const reactor = eh->reactor ();
          reactor->cancel_timer (this->flush_timer_id_);
          this->reset_flush_timer ();
        }

      return DR_QUEUE_EMPTY;
    }

  return DR_OK;
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

  size_t byte_count = 0;
  int msg_count = 0;

  // Cleanup all messages
  while (!this->queue_is_empty_i ())
    {
      TAO_Queued_Message *i = this->head_;

      if (TAO_debug_level > 4)
        {
          byte_count += i->message_length();
          ++msg_count;
        }
       // @@ This is a good point to insert a flag to indicate that a
       //    CloseConnection message was successfully received.
      i->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                        this->orb_core_->leader_follower ());

      i->remove_from_list (this->head_, this->tail_);

      i->destroy ();
    }

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Transport[%d]::cleanup_queue_i, ")
                  ACE_TEXT ("discarded %d messages, %u bytes.\n"),
                  this->id (), msg_count, byte_count));
    }
}

void
TAO_Transport::cleanup_queue (size_t byte_count)
{
  while (!this->queue_is_empty_i () && byte_count > 0)
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
      else if (byte_count == 0)
        {
          // If we have sent out a full message block, but we are not
          // finished with this message, we need to do something with the
          // message block chain held by our output stream.  If we don't,
          // another thread can attempt to service this transport and end
          // up resetting the output stream which will release the
          // message that we haven't finished sending.
          i->copy_if_necessary (this->out_stream ().begin ());
        }
    }
}

int
TAO_Transport::check_buffering_constraints_i (TAO_Stub *stub, bool &must_flush)
{
  // First let's compute the size of the queue:
  size_t msg_count = 0;
  size_t total_bytes = 0;

  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      ++msg_count;
      total_bytes += i->message_length ();
    }

  bool set_timer = false;
  ACE_Time_Value new_deadline;

  TAO::Transport_Queueing_Strategy *queue_strategy =
    stub->transport_queueing_strategy ();

  bool constraints_reached = true;

  if (queue_strategy)
    {
      constraints_reached =
        queue_strategy->buffering_constraints_reached (stub,
                                                       msg_count,
                                                       total_bytes,
                                                       must_flush,
                                                       this->current_deadline_,
                                                       set_timer,
                                                       new_deadline);
    }
  else
    {
      must_flush = false;
    }

  // ... set the new timer, also cancel any previous timers ...
  if (set_timer)
    {
      ACE_Event_Handler *eh = this->event_handler_i ();
      ACE_Reactor * const reactor = eh->reactor ();
      this->current_deadline_ = new_deadline;
      ACE_Time_Value delay = new_deadline - ACE_OS::gettimeofday ();

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
         ACE_TEXT ("(%C) no longer associated with handler [tag=%d]\n"),
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
}

int
TAO_Transport::send_message_shared_i (TAO_Stub *stub,
                                      TAO_Message_Semantics message_semantics,
                                      const ACE_Message_Block *message_block,
                                      ACE_Time_Value *max_wait_time)
{
  int ret = 0;

#if TAO_HAS_TRANSPORT_CURRENT == 1
  size_t const message_length = message_block->length ();
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

  switch (message_semantics)
    {
      case TAO_TWOWAY_REQUEST:
        ret = this->send_synchronous_message_i (message_block, max_wait_time);
        break;

      case TAO_REPLY:
        ret = this->send_reply_message_i (message_block, max_wait_time);
        break;

      case TAO_ONEWAY_REQUEST:
        ret = this->send_asynchronous_message_i (stub,
                                                 message_block,
                                                 max_wait_time);
        break;
    }

#if TAO_HAS_TRANSPORT_CURRENT == 1
  // "Count" the message, only if no error was encountered.
  if (ret != -1 && this->stats_ != 0)
    this->stats_->messages_sent (message_length);
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

  return ret;
}

int
TAO_Transport::send_asynchronous_message_i (TAO_Stub *stub,
                                            const ACE_Message_Block *message_block,
                                            ACE_Time_Value *max_wait_time)
{
  // Let's figure out if the message should be queued without trying
  // to send first:
  bool try_sending_first = true;

  bool const queue_empty = this->queue_is_empty_i ();

  TAO::Transport_Queueing_Strategy *queue_strategy =
    stub->transport_queueing_strategy ();

  if (!queue_empty)
    {
      try_sending_first = false;
    }
  else if (queue_strategy)
    {
      if (queue_strategy->must_queue (queue_empty))
        {
          try_sending_first = false;
        }
    }

  bool partially_sent = false;
  bool timeout_encountered = false;

  TAO::Transport::Drain_Constraints dc(
      max_wait_time, this->using_blocking_io_for_asynch_messages());

  if (try_sending_first)
    {
      ssize_t n = 0;
      size_t byte_count = 0;
      // ... in this case we must try to send the message first ...

      size_t const total_length = message_block->total_length ();

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
                                            dc);

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

      if (byte_count > 0)
      {
        partially_sent = true;
      }

      // If it was partially sent, then push to front of queue and don't flush
      if (errno == ETIME)
      {
        timeout_encountered = true;
        if (byte_count == 0)
        {
          //This request has timed out and none of it was sent to the transport
          //We can't return -1 here, since that would end up closing the tranpsort
          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("Transport[%d]::send_asynchronous_message_i, ")
                          ACE_TEXT ("timeout encountered before any bytes sent\n"),
                          this->id ()));
            }
          throw ::CORBA::TIMEOUT (
            CORBA::SystemException::_tao_minor_code (
              TAO_TIMEOUT_SEND_MINOR_CODE,
              ETIME),
            CORBA::COMPLETED_NO);
        }
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

  ACE_Time_Value *wait_time = (partially_sent ? 0: max_wait_time);
  if (this->queue_message_i (message_block, wait_time, !partially_sent)
      == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::")
                      ACE_TEXT ("send_asynchronous_message_i, ")
                      ACE_TEXT ("cannot queue message for  - %m\n"),
                      this->id ()));
        }
      return -1;
    }

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::send_asynchronous_message_i, ")
         ACE_TEXT ("message is queued\n"),
         this->id ()));
    }

  if (timeout_encountered && partially_sent)
    {
      //Must close down the transport here since we can't guarantee the
      //integrity of the GIOP stream (the next send may try to write to
      //the socket before looking at the queue).
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport[%d]::")
                      ACE_TEXT ("send_asynchronous_message_i, ")
                      ACE_TEXT ("timeout after partial send, closing.\n"),
                      this->id ()));
        }
      return -1;
    }
  else if (!timeout_encountered)
    {
      // We can't flush if we have already encountered a timeout
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
          int const result = flushing_strategy->schedule_output (this);
          if (result == TAO_Flushing_Strategy::MUST_FLUSH)
            {
              must_flush = true;
            }
        }

      if (must_flush)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - Transport[%d]::")
                          ACE_TEXT ("send_asynchronous_message_i, ")
                          ACE_TEXT ("flushing transport.\n"),
                          this->id ()));
            }

          size_t sent_byte = sent_byte_count_;
          int ret = 0;
          {
            typedef ACE_Reverse_Lock<ACE_Lock> TAO_REVERSE_LOCK;
            TAO_REVERSE_LOCK reverse (*this->handler_lock_);
            ACE_GUARD_RETURN (TAO_REVERSE_LOCK, ace_mon, reverse, -1);
            ret = flushing_strategy->flush_transport (this, max_wait_time);
          }

          if (ret == -1)
            {
              if (errno == ETIME)
                {
                  if (sent_byte == sent_byte_count_) // if nothing was actually flushed
                    {
                      //This request has timed out and none of it was sent to the transport
                      //We can't return -1 here, since that would end up closing the tranpsort
                      if (TAO_debug_level > 2)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                      ACE_TEXT ("TAO (%P|%t) - ")
                                      ACE_TEXT ("Transport[%d]::send_asynchronous_message_i, ")
                                      ACE_TEXT ("2 timeout encountered before any bytes sent\n"),
                                      this->id ()));
                        }
                      throw ::CORBA::TIMEOUT (CORBA::SystemException::_tao_minor_code
                                              (TAO_TIMEOUT_SEND_MINOR_CODE, ETIME),
                                              CORBA::COMPLETED_NO);
                    }
                }
              return -1;
            }
        }
    }
  return 0;
}

int
TAO_Transport::queue_message_i (const ACE_Message_Block *message_block,
                                ACE_Time_Value *max_wait_time, bool back)
{
  TAO_Queued_Message *queued_message = 0;
  ACE_NEW_RETURN (queued_message,
                  TAO_Asynch_Queued_Message (message_block,
                                             this->orb_core_,
                                             max_wait_time,
                                             0,
                                             true),
                  -1);
  if (back) {
    queued_message->push_back (this->head_, this->tail_);
  }
  else {
    queued_message->push_front (this->head_, this->tail_);
  }

  return 0;
}

/**
 * All the methods relevant to the incoming data path of the ORB are
 * defined below
 */
int
TAO_Transport::handle_input (TAO_Resume_Handle &rh,
                             ACE_Time_Value * max_wait_time)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
         ACE_TEXT ("TAO (%P|%t) - Transport[%d]::handle_input\n"),
         this->id ()));
    }

  // First try to process messages of the head of the incoming queue.
  int const retval = this->process_queue_head (rh);

  if (retval <= 0)
    {
      if (retval == -1)
        {
          if (TAO_debug_level > 2)
            {
              ACE_ERROR ((LM_ERROR,
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
      && q_data->missing_data () != TAO_MISSING_DATA_UNDEFINED)
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
  if (q_data->missing_data () != 0)
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

  if (q_data->more_fragments () ||
      q_data->msg_type () == GIOP::Fragment)
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
  if (q_data->missing_data () != 0)
    {
       return -1;
    }

  if (q_data->more_fragments () ||
      q_data->msg_type () == GIOP::Fragment)
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
         this->id (), q_data->missing_data ()));
    }

  size_t const recv_size = q_data->missing_data ();

  if (q_data->msg_block ()->space() < recv_size)
    {
      // make sure the message_block has enough space
      size_t const message_size = recv_size + q_data->msg_block ()->length();

      if (ACE_CDR::grow (q_data->msg_block (), message_size) == -1)
        {
          return -1;
        }
    }

  // Saving the size of the received buffer in case any one needs to
  // get the size of the message that is received in the
  // context. Obviously the value will be changed for each recv call
  // and the user is supposed to invoke the accessor only in the
  // invocation context to get meaningful information.
  this->recv_buffer_size_ = recv_size;

  // Read the message into the existing message block on heap
  ssize_t const n = this->recv (q_data->msg_block ()->wr_ptr(),
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

  q_data->msg_block ()->wr_ptr(n);
  q_data->missing_data (q_data->missing_data () - n);

  if (q_data->missing_data () == 0)
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
TAO_Transport::handle_input_parse_extra_messages (
  ACE_Message_Block &message_block)
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
      if (q_data->missing_data () == 0)
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

  // Optimizing access of constants
  size_t const header_length = this->messaging_object ()->header_length ();

  // Paranoid check
  if (header_length > message_block.space ())
    {
      return -1;
    }

  if (this->orb_core_->orb_params ()->single_read_optimization ())
    {
      recv_size = message_block.space ();
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
           && q_data->missing_data () == TAO_MISSING_DATA_UNDEFINED)
        {
          // There is a partial message on incoming_message_stack_
          // whose length is unknown so far. We need to consolidate
          // the GIOP header to get to know the payload size,
          recv_size = header_length - q_data->msg_block ()->length ();
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

  // Read the message into the message block that we have created on
  // the stack.
  ssize_t const n = this->recv (message_block.wr_ptr (),
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
      && q_data->missing_data () == TAO_MISSING_DATA_UNDEFINED)
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
      if (q_data->missing_data () == 0)
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

      if (this->messaging_object ()->parse_next_message (qd, mesg_length) == -1
          || (qd.missing_data () == 0
              && mesg_length > message_block.length ()) )
        {
          // extracting message failed
          return -1;
        }
      // POST: qd.missing_data_ == 0 --> mesg_length <= message_block.length()
      // This prevents seeking rd_ptr behind the wr_ptr

      if (qd.missing_data () != 0 ||
          qd.more_fragments () ||
          qd.msg_type () == GIOP::Fragment)
        {
          if (qd.missing_data () == 0)
            {
              // Dealing with a fragment
              TAO_Queued_Data *nqd = TAO_Queued_Data::duplicate (qd);

              if (nqd == 0)
                {
                  return -1;
                }

              // mark the end of message in new buffer
              char* end_mark = nqd->msg_block ()->rd_ptr ()
                             + mesg_length;
              nqd->msg_block ()->wr_ptr (end_mark);

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
          else if (qd.missing_data () != TAO_MISSING_DATA_UNDEFINED)
            {
              // Incomplete message, must be the last one in buffer

              if (qd.missing_data () != TAO_MISSING_DATA_UNDEFINED &&
                  qd.missing_data () > message_block.space ())
                {
                  // Re-Allocate correct size on heap
                  if (ACE_CDR::grow (qd.msg_block (),
                                     message_block.length ()
                                     + qd.missing_data ()) == -1)
                    {
                      return -1;
                    }
                }

              TAO_Queued_Data *nqd = TAO_Queued_Data::duplicate (qd);

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

              // correct the wr_ptr using the end_marker to point to the
              // end of the first message else the code after this will
              // see the full stream with all the messages
              message_block.wr_ptr (end_marker);

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

              int const retval = this->notify_reactor ();

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
          if (this->process_parsed_messages (&qd, rh) == -1)
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
         this->id(), qd->missing_data ()));
    }

#if TAO_HAS_TRANSPORT_CURRENT == 1
  // Update stats, if any
  if (this->stats_ != 0)
    this->stats_->messages_received (qd->msg_block ()->length ());
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

  switch (qd->msg_type ())
  {
    case GIOP::CloseConnection:
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
             ACE_TEXT ("received CloseConnection message - %m\n"),
             this->id()));
        }

      // Return a "-1" so that the next stage can take care of
      // closing connection and the necessary memory management.
      return -1;
    }
    break;
    case GIOP::Request:
    case GIOP::LocateRequest:
    {
      // Let us resume the handle before we go ahead to process the
      // request. This will open up the handle for other threads.
      rh.resume_handle ();

      if (this->messaging_object ()->process_request_message (this, qd) == -1)
        {
          // Return a "-1" so that the next stage can take care of
          // closing connection and the necessary memory management.
          return -1;
        }
    }
    break;
    case GIOP::Reply:
    case GIOP::LocateReply:
    {
      rh.resume_handle ();

      TAO_Pluggable_Reply_Params params (this);

      if (this->messaging_object ()->process_reply_message (params, qd) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("TAO (%P|%t) - Transport[%d]::process_parsed_messages, ")
                 ACE_TEXT ("error in process_reply_message - %m\n"),
                 this->id ()));
            }

          return -1;
        }

    }
    break;
    case GIOP::CancelRequest:
    {
      // The associated request might be incomplete residing
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
    break;
    case GIOP::MessageError:
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
    break;
    case GIOP::Fragment:
    {
      // Nothing to be done.
    }
    break;
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

          int const retval = this->notify_reactor ();

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
      int const retval = this->process_parsed_messages (qd, rh);

      // Delete the Queued_Data..
      TAO_Queued_Data::release (qd);

      return retval;
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
  int const retval = reactor->notify (eh, ACE_Event_Handler::READ_MASK);

  if (retval < 0 && TAO_debug_level > 2)
    {
      // @todo: need to think about what is the action that
      // we can take when we get here.
      ACE_ERROR ((LM_ERROR,
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

TAO_SYNCH_MUTEX &
TAO_Transport::output_cdr_lock (void)
{
  return this->output_cdr_mutex_;
}

void
TAO_Transport::messaging_init (TAO_GIOP_Message_Version const &version)
{
  this->messaging_object ()->init (version.major, version.minor);
}

void
TAO_Transport::pre_close (void)
{
  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport[%d]::pre_close\n"),
                  this->id_));
    }
  // @TODO: something needs to be done with is_connected_. Checking it is
  // guarded by a mutex, but setting it is not. Until the need for mutexed
  // protection is required, the transport cache is holding its own copy
  // of the is_connected_ flag, so that during cache lookups the cache
  // manager doesn't need to be burdened by the lock in is_connected().
  this->is_connected_ = false;
  this->transport_cache_manager ().mark_connected (this->cache_map_entry_,
                                                   false);
  this->purge_entry ();
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->handler_lock_));
    this->cleanup_queue_i ();
  }
}

bool
TAO_Transport::post_open (size_t id)
{
  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport::post_open, ")
                  ACE_TEXT ("tport id changed from %d to %d\n"), this->id_, id));
    }
  this->id_ = id;

  // When we have data in our outgoing queue schedule ourselves
  // for output
  if (!this->queue_is_empty_i ())
    {
      // If the wait strategy wants us to be registered with the reactor
      // then we do so. If registeration is required and it succeeds,
      // #REFCOUNT# becomes two.
      if (this->wait_strategy ()->register_handler () == 0)
        {
          if (this->flush_in_post_open_)
            {
              TAO_Flushing_Strategy *flushing_strategy =
                this->orb_core ()->flushing_strategy ();

              if (flushing_strategy == 0)
                throw CORBA::INTERNAL ();

              this->flush_in_post_open_ = false;
              (void) flushing_strategy->schedule_output (this);
            }
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
            {
              ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO (%P|%t) - Transport[%d]::post_open , ")
                     ACE_TEXT ("could not register the transport ")
                     ACE_TEXT ("in the reactor.\n"),
                     this->id ()));
            }

          return false;
        }
    }

  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, false);
    this->is_connected_ = true;
  }

  if (TAO_debug_level > 9 && !this->cache_map_entry_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport[%d]::post_open")
                            ACE_TEXT (", cache_map_entry_ is 0\n"), this->id_));
    }

  this->transport_cache_manager ().mark_connected (this->cache_map_entry_,
                                                   true);

  // update transport cache to make this entry available
  this->transport_cache_manager ().set_entry_state (
    this->cache_map_entry_,
    TAO::ENTRY_IDLE_AND_PURGABLE);

  return true;
}

void
TAO_Transport::allocate_partial_message_block (void)
{
  if (this->partial_message_ == 0)
    {
      // This value must be at least large enough to hold a GIOP message
      // header plus a GIOP fragment header
      size_t const partial_message_size =
        this->messaging_object ()->header_length ();
       // + this->messaging_object ()->fragment_header_length ();
       // deprecated, conflicts with not-single_read_opt.

      ACE_NEW (this->partial_message_,
               ACE_Message_Block (partial_message_size));
    }
}

void
TAO_Transport::set_bidir_context_info (TAO_Operation_Details &)
{
}

ACE_Time_Value const *
TAO_Transport::io_timeout(
    TAO::Transport::Drain_Constraints const & dc) const
{
  if (dc.block_on_io())
  {
    return dc.timeout();
  }
  if (this->wait_strategy()->can_process_upcalls())
  {
    return 0;
  }
  return dc.timeout();
}

bool
TAO_Transport::using_blocking_io_for_synch_messages (void) const
{
  if (this->wait_strategy()->can_process_upcalls())
  {
    return false;
  }
  return true;
}

bool
TAO_Transport::using_blocking_io_for_asynch_messages (void) const
{
  return false;
}

/*
 * Hook to add concrete implementations from the derived class onto
 * TAO's transport.
 */

//@@ TAO_TRANSPORT_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
