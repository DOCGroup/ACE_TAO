// -*- C++ -*-
// $Id$

#include "tao/Connection_Handler.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Resume_Handle.h"
#include "tao/Transport.h"
#include "tao/Wait_Strategy.h"

#include "ace/SOCK.h"
#include "ace/Reactor.h"
#include "ace/os_include/sys/os_socket.h"
#include "ace/Svc_Handler.h"

//@@ CONNECTION_HANDLER_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

#if !defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Connection_Handler::TAO_Connection_Handler (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    transport_ (0),
    connection_pending_ (false),
    is_closed_ (false)
{
  // Put ourselves in the connection wait state as soon as we get
  // created
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_WAIT,
                       this->orb_core_->leader_follower ());
}

TAO_Connection_Handler::~TAO_Connection_Handler (void)
{
  //@@ CONNECTION_HANDLER_DESTRUCTOR_ADD_HOOK
}

int
TAO_Connection_Handler::shared_open (void)
{
  // This reference counting is related to asynch connections.  It
  // should probably be managed by the ACE_Strategy_Connector, since
  // that's really the reference being managed here.  also, whether
  // open ultimately succeeds or fails, the connection attempted is
  // ending, so the reference must be removed in any case.
  this->cancel_pending_connection();

  return 0;
}

int
TAO_Connection_Handler::set_socket_option (ACE_SOCK &sock,
                                           int snd_size,
                                           int rcv_size)
{
#if !defined (ACE_LACKS_SO_SNDBUF)
  if (snd_size != 0
      && sock.set_option (SOL_SOCKET,
                          SO_SNDBUF,
                          (void *) &snd_size,
                          sizeof (snd_size)) == -1
      && errno != ENOTSUP)
  {
    return -1;
  }
#endif /* !ACE_LACKS_SO_SNDBUF */

#if !defined (ACE_LACKS_SO_RCVBUF)
  if (rcv_size != 0
      && sock.set_option (SOL_SOCKET,
                          SO_RCVBUF,
                          (void *) &rcv_size,
                          sizeof (int)) == -1
      && errno != ENOTSUP)
  {
    return -1;
  }
#endif /* !ACE_LACKS_SO_RCVBUF */

#if defined (ACE_LACKS_SO_SNDBUF) && defined (ACE_LACKS_SO_RCVBUF)
   ACE_UNUSED_ARG (snd_size);
   ACE_UNUSED_ARG (rcv_size);
#endif

  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).
  (void) sock.enable (ACE_CLOEXEC);

  return 0;
}

int
TAO_Connection_Handler::svc_i (void)
{
  int result = 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Connection_Handler::svc_i begin\n")));

  // Here we simply synthesize the "typical" event loop one might find
  // in a reactive handler, except that this can simply block waiting
  // for input.

  ACE_Time_Value *max_wait_time = 0;
  ACE_Time_Value timeout;
  ACE_Time_Value current_timeout;

  if (this->orb_core_->thread_per_connection_timeout (timeout))
    {
      current_timeout = timeout;
      max_wait_time = &current_timeout;
    }

  TAO_Resume_Handle rh (this->orb_core_, ACE_INVALID_HANDLE);

  // We exit of the loop if
  // - If the ORB core is shutdown by another thread
  // - Or if the transport is null. This could happen if an error
  // occured.
  // - Or if during processing a return value of -1 is received.
  while (!this->orb_core_->has_shutdown () && result >= 0)
    {
      // Let the transport know that it is used
      (void) this->transport ()->update_transport ();

      result = this->transport ()->handle_input (rh, max_wait_time);

      if (result == -1 && errno == ETIME)
        {
          // Ignore timeouts, they are only used to wake up and
          // shutdown.
          result = 0;

          // Reset errno to make sure we don't trip over an old value
          // of errno in case it is not reset when the recv() call
          // fails if the socket has been closed.
          errno = 0;
        }
      else if (result == -1)
        {
          // Something went wrong with the socket. Just quit
          return result;
        }

      current_timeout = timeout;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Connection_Handler::svc_i - "
                    "loop <%d>\n", current_timeout.msec ()));
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) - Connection_Handler::svc_i - end\n"));

  return result;
}

void
TAO_Connection_Handler::transport (TAO_Transport* transport)
{
  this->transport_ = transport;

  // Enable reference counting on the event handler.
  this->transport_->event_handler_i ()->reference_counting_policy ().value (
      ACE_Event_Handler::Reference_Counting_Policy::ENABLED
    );
}

int
TAO_Connection_Handler::handle_output_eh (ACE_HANDLE, ACE_Event_Handler * eh)
{
  // Let the transport that it is going to be used
  (void) this->transport ()->update_transport ();

  // Instantiate the resume handle here.. This will automatically
  // resume the handle once data is written..
  TAO_Resume_Handle resume_handle (this->orb_core (), eh->get_handle ());

  int return_value = 0;
  this->pre_io_hook (return_value);
  if (return_value != 0)
    {
      resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
      return return_value;
    }

  // The default constraints are to never block.
  TAO::Transport::Drain_Constraints dc;
  if (this->transport ()->handle_output (dc) == TAO_Transport::DR_ERROR)
    {
      return_value = -1;
    }

  this->pos_io_hook (return_value);

  if (return_value != 0)
    {
      resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
    }

  return return_value;
}

int
TAO_Connection_Handler::handle_input_eh (ACE_HANDLE h, ACE_Event_Handler *eh)
{
  // If we can't process upcalls just return
  if (!this->transport ()->wait_strategy ()->can_process_upcalls ())
    {
      if (TAO_debug_level > 6)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Connection_Handler[%d]::handle_input_eh, "
                    "not going to handle_input on transport "
                    "because upcalls temporarily suspended on this thread\n",
                    this->transport()->id()));
      return 0;
    }

  int const result = this->handle_input_internal (h, eh);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO_Connection_Handler::handle_input_internal (
    ACE_HANDLE h, ACE_Event_Handler * eh)
{
  // Let the transport know that it is used
  (void) this->transport ()->update_transport ();

  // Grab the transport id now and use the cached value for printing
  // since the  transport could dissappear by the time the thread
  // returns.
  size_t const t_id = this->transport ()->id ();

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d\n",
                  t_id, handle, h));
    }

  TAO_Resume_Handle resume_handle (this->orb_core (), eh->get_handle ());

  int return_value = 0;

  this->pre_io_hook (return_value);

  if (return_value != 0)
    return return_value;

  return_value = this->transport ()->handle_input (resume_handle);

  this->pos_io_hook (return_value);

  // Bug 1647; might need to change resume_handle's flag or
  // change handle_input return value.
  resume_handle.handle_input_return_value_hook(return_value);

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle ();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, retval = %d\n",
                  t_id, handle, h, return_value));
    }

  if (return_value == -1)
    resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
  return return_value;
}

int
TAO_Connection_Handler::close_connection_eh (ACE_Event_Handler *eh)
{
  {
    // Make sure we only close once
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->is_closed_mutex_,
                      0);
    if (this->is_closed_)
      {
        return 1;
      }
    else
      {
        this->is_closed_ = true;
      }
  }

  // Save the ID for debugging messages
  ACE_HANDLE const handle = eh->get_handle ();

  size_t const id = this->transport ()->id ();
  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::"
                   "close_connection_eh, purging entry from cache\n",
                   handle));
    }

  this->transport ()->pre_close ();

  // @@ This seems silly, but if we have no reason to be in the
  // reactor, then we dont remove ourselves.
  if (this->transport ()->wait_strategy ()->is_registered ())
    {
      ACE_Reactor *eh_reactor = eh->reactor ();

      if (this->orb_core_->has_shutdown () == 0)
        {
          // If the ORB is nil, get the reactor from orb_core which gets it
          // from LF.
          if (eh_reactor == 0)
            eh_reactor = this->transport()->orb_core()->reactor ();
        }

      // The Reactor must not be null, otherwise something else is
      // horribly broken.
      ACE_ASSERT (eh_reactor != 0);

      if (TAO_debug_level)
        {
          ACE_DEBUG  ((LM_DEBUG,
                       "TAO (%P|%t) - Connection_Handler[%d]::"
                       "close_connection_eh, removing from the reactor\n",
                       handle));
        }

      // Use id instead of handle. Why? "handle" may be invalid for RW
      // cases  when drop_reply_on_shutdown is on, and when the
      // orb_core is shutting down. This means that the handler will
      // be left behind in the reactor which would create problems
      // later. Just forcefully remove them. If none exists reactor
      // will make things safer.
      ACE_HANDLE tmp_handle = handle;
      if (this->orb_core_->has_shutdown ())
        tmp_handle = (ACE_HANDLE) id;

      eh_reactor->remove_handler (tmp_handle,
                                  ACE_Event_Handler::ALL_EVENTS_MASK |
                                  ACE_Event_Handler::DONT_CALL);

      // Also cancel any timers, we may create those for time-limited
      // buffering
      if (TAO_debug_level)
        {
          ACE_DEBUG  ((LM_DEBUG,
                       "TAO (%P|%t) - Connection_Handler[%d]::"
                       "close_connection_eh, cancel all timers\n",
                       handle));
        }

      eh_reactor->cancel_timer (eh);

      // @@ This seems silly, the reactor is a much better authority to
      //    find out if a handle is registered...
      this->transport ()->wait_strategy ()->is_registered (false);
    }

  // This call should be made only after the cache and reactor are
  // cleaned up. This call can make upcalls to the application which
  // in turn can make remote calls (Bug 1551 and Bug 1482). The remote
  // calls from the application can try to use this handler from the
  // cache or from the reactor. So clean them up before this is
  // called.
  this->transport ()->send_connection_closed_notifications ();
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                       this->orb_core_->leader_follower ());

  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::"
                   "close_connection_eh end\n",
                   id));
    }

  return 1;
}

/*
 * Comment hook to comment the base class implementations
 * that do nothing. Specialized versions from derived
 * class will directly override these methods. Add
 * all methods that are virtual, have do nothing implementations
 * within this hook for later specialization.
 */
//@@ CONNECTION_HANDLER_SPL_COMMENT_HOOK_START

int
TAO_Connection_Handler::set_dscp_codepoint (CORBA::Boolean)
{
  return 0;
}

int
TAO_Connection_Handler::set_dscp_codepoint (CORBA::Long)
{
  return 0;
}

int
TAO_Connection_Handler::release_os_resources (void)
{
  return 0;
}

//@@ CONNECTION_HANDLER_SPL_COMMENT_HOOK_END

void
TAO_Connection_Handler::pre_io_hook (int &)
{
}

void
TAO_Connection_Handler::pos_io_hook (int &)
{
}

int
TAO_Connection_Handler::close_handler (u_long)
{
  {
    // Make sure we only close once
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->is_closed_mutex_,
                      0);
                      
    if (this->is_closed_)
      {
        return 0;
      }
    else
      {
        this->is_closed_ = true;
      }
  }

  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                       this->orb_core_->leader_follower ());

  // If there was a pending connection cancel it.
  this->cancel_pending_connection ();

  // Purge transport from cache if it's in cache.
  this->transport ()->purge_entry();

  return 0;
}

//@@ CONNECTION_HANDLER_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
