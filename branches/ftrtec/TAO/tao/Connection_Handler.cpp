//$Id$

#include "Connection_Handler.h"
#include "ORB_Core.h"
#include "Server_Strategy_Factory.h"
#include "debug.h"
#include "Object.h"
#include "Resume_Handle.h"
#include "Transport.h"
#include "Wait_Strategy.h"

#include "ace/SOCK.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Connection_Handler,
           "$Id$")

TAO_Connection_Handler::TAO_Connection_Handler (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
  , transport_ (0)
  , tss_resources_ (orb_core->get_tss_resources ())
{
  // @@todo: We need to have a distinct option/ method in the resource
  // factory for this and TAO_Transport.

  this->lock_ =
    this->orb_core_->resource_factory ()->create_cached_connection_lock ();

  // Put ourselves in the connection wait state as soon as we get
  // created
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_WAIT);
}

TAO_Connection_Handler::~TAO_Connection_Handler (void)
{
  int result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO (%P|%t) - Connection_Handler::~Connection_Handler,"
                  "release_os_resources() failed %p\n"));
    }

  // @@ TODO Use auto_ptr<>
  delete this->lock_;
}


int
TAO_Connection_Handler::set_socket_option (ACE_SOCK &sock,
                                           int snd_size,
                                           int rcv_size)
{
#if !defined (ACE_LACKS_SOCKET_BUFSIZ)

  if (sock.set_option (SOL_SOCKET,
                       SO_SNDBUF,
                       (void *) &snd_size,
                       sizeof (snd_size)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (sock.set_option (SOL_SOCKET,
                            SO_RCVBUF,
                            (void *) &rcv_size,
                            sizeof (int)) == -1
           && errno != ENOTSUP)
    return -1;
#else
   ACE_UNUSED_ARG (snd_size);
   ACE_UNUSED_ARG (rcv_size);
#endif /* !ACE_LACKS_SOCKET_BUFSIZ */

  (void) sock.enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  return 0;
}

int
TAO_Connection_Handler::svc_i (void)
{
  int result = 0;

  // Inheriting the ORB_Core tss stuff from the parent thread.
  this->orb_core_->inherit_from_parent_thread (this->tss_resources_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Connection_Handler::svc_i begin\n")));

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

  TAO_Resume_Handle rh (this->orb_core_,
                        ACE_INVALID_HANDLE);

  // We exit of the loop if
  // - If the ORB core is shutdown by another thread
  // - Or if the transport is null. This could happen if an error
  // occured.
  // - Or if during processing a return value of -1 is received.
  while (!this->orb_core_->has_shutdown ()
         && this->transport ()
         && result >= 0)
    {
      // Let the transport know that it is used
      (void) this->transport ()->update_transport ();

      result =
        this->transport ()->handle_input (rh,
                                          max_wait_time);

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

      current_timeout = timeout;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Connection_Handler::svc_i - "
                    "loop <%d>\n", current_timeout.msec ()));
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) - Connection_Handler::svc_i end\n"));

  return result;
}

void
TAO_Connection_Handler::transport (TAO_Transport* transport)
{
  this->transport_ = transport;

  // Enable reference counting on the event handler.
  this->transport_->event_handler_i ()->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

int
TAO_Connection_Handler::handle_output_eh (
    ACE_HANDLE, ACE_Event_Handler * eh)
{
  // Let the transport that it is going to be used
  (void) this->transport ()->update_transport ();

  // Instantiate the resume handle here.. This will automatically
  // resume the handle once data is written..
  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   eh->get_handle ());

  int return_value = 0;
  this->pre_io_hook (return_value);
  if (return_value != 0)
    {
      resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
      return return_value;
    }

  return_value = this->transport ()->handle_output ();

  this->pos_io_hook (return_value);
  if (return_value != 0)
    {
      resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
    }

  return return_value;
}

int
TAO_Connection_Handler::handle_input_eh (
    ACE_HANDLE h, ACE_Event_Handler * eh)
{
  // Let the transport know that it is used
  (void) this->transport ()->update_transport ();

  // Grab the transport id now and use the cached value for printing
  // since the  transport could dissappear by the time the thread
  // returns.
  size_t t_id =
    this->transport ()->id ();

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d\n",
                  t_id, handle, h));
    }

  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   eh->get_handle ());

  int return_value = 0;

  this->pre_io_hook (return_value);
  if (return_value != 0)
    return return_value;

  return_value = this->transport ()->handle_input (resume_handle);

  this->pos_io_hook (return_value);

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle ();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, retval = %d\n",
                  t_id, handle, h, return_value));
    }

  return return_value;
}

int
TAO_Connection_Handler::close_connection_eh (ACE_Event_Handler *eh)
{
  this->transport ()->send_connection_closed_notifications ();

  // Save the ID for debugging messages
  ACE_HANDLE handle = eh->get_handle ();
  size_t id = this->transport ()->id ();
  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::"
                   "close_connection, purging entry from cache\n",
                   handle));
    }
  this->transport ()->purge_entry ();

  // @@ This seems silly, but if we have no reason to be in the
  // reactor, then we dont remove ourselves.
  if (this->transport ()->wait_strategy ()->is_registered ())
    {
      ACE_Reactor *eh_reactor = eh->reactor ();

      // These checks are valid as long as the ORB_Core is not
      // shutdown. It is good to have these checks and they are valid
      // for most of the cases. Please see below for exceptions
      if (this->orb_core_->has_shutdown () == 0)
        {
          // The exception when these are not valid is for
          // RTCORBA. With RTCORBA on, you can threads in different
          // lanes creating handlers and registering them with reactor
          // in those respective lanes. These threads could then even
          // go away leaving the task of reclaiming these resources to
          // the main thread. For the main thread that takes the
          // responsibility of finalizing () the lanes and the pools,
          // the calls and comparison make no sense.
          ACE_Reactor * reactor =
            this->transport()->orb_core()->reactor ();
          ACE_ASSERT (eh_reactor == 0 || eh_reactor == reactor);

          ACE_Reactor * orb_core_reactor = this->orb_core_->reactor ();
          ACE_ASSERT (reactor == orb_core_reactor);

          if (eh_reactor == 0)
            eh_reactor = reactor;

          ACE_UNUSED_ARG (orb_core_reactor);
        }

      // The Reactor must not be null, otherwise something else is
      // horribly broken.
      ACE_ASSERT (eh_reactor != 0);

      if (TAO_debug_level)
        {
          ACE_DEBUG  ((LM_DEBUG,
                       "TAO (%P|%t) - Connection_Handler[%d]::"
                       "close_connection, removing from the reactor\n",
                       handle));
        }

      eh_reactor->remove_handler (handle,
                                  ACE_Event_Handler::ALL_EVENTS_MASK |
                                  ACE_Event_Handler::DONT_CALL);

      // Also cancel any timers, we may create those for time-limited
      // buffering
      if (TAO_debug_level)
        {
          ACE_DEBUG  ((LM_DEBUG,
                       "TAO (%P|%t) - Connection_Handler[%d]::"
                       "close_connection, cancel all timers\n",
                       handle));
        }

      eh_reactor->cancel_timer (eh);

      // @@ This seems silly, the reactor is a much better authority to
      //    find out if a handle is registered...
      this->transport ()->wait_strategy ()->is_registered (0);
    }

  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);

  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::"
                   "close_connection\n",
                   id));
    }

  return 1;
}

int
TAO_Connection_Handler::release_os_resources (void)
{
  return 0;
}

void
TAO_Connection_Handler::pre_io_hook (int & )
{
}

void
TAO_Connection_Handler::pos_io_hook (int & )
{
}
