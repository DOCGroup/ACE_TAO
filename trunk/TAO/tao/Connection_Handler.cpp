//$Id$

#include "Connection_Handler.h"
#include "ORB_Core.h"
#include "Server_Strategy_Factory.h"
#include "debug.h"
#include "Object.h"
#include "Resume_Handle.h"
#include "Transport.h"
#include "Wait_Strategy.h"
#include "Resume_Handle.h"

#include "ace/SOCK.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Connection_Handler,
           "$Id$")

TAO_Connection_Handler::TAO_Connection_Handler (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
   transport_ (0),
   tss_resources_ (orb_core->get_tss_resources ()),
   pending_upcalls_ (1),
   reference_count_ (1),
   pending_upcall_lock_ (0)
{
  // NOTE: Why should the refcount be
  // @@todo: We need to have a distinct option/ method in the resource
  // factory for this and TAO_Transport.
  this->pending_upcall_lock_ =
    this->orb_core_->resource_factory ()->create_cached_connection_lock ();

  // Put ourselves in the connection wait state as soon as we get
  // created
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_WAIT);
}


TAO_Connection_Handler::~TAO_Connection_Handler (void)
{
  // Set some of the pointers that we hold to zero explicitly, so that
  // nobody tries to access these
  this->orb_core_ = 0;
  this->tss_resources_ = 0;
  TAO_Transport::release (this->transport_);

  delete this->pending_upcall_lock_;
  this->pending_upcall_lock_ = 0;
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
                ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i begin\n")));

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

  while (!this->orb_core_->has_shutdown ()
         && result >= 0)
    {
      result =
        this->transport ()->handle_input_i (rh,
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
                    ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i - ")
                    ACE_TEXT ("loop <%d>\n"), current_timeout.msec ()));
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) TAO_Connection_Handler::svc_i end\n")));

  return result;
}

void
TAO_Connection_Handler::transport (TAO_Transport* transport)
{
  if (this->transport_ != 0) {
    this->transport_->connection_handler_closing ();
  }

  this->transport_ = TAO_Transport::_duplicate (transport);
}

int
TAO_Connection_Handler::incr_refcount (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->pending_upcall_lock_, -1);

  return ++this->reference_count_;
}

void
TAO_Connection_Handler::decr_refcount (void)
{
  {
    ACE_GUARD (ACE_Lock,
               ace_mon,
               *this->pending_upcall_lock_);

    --this->reference_count_;
  }

  if (this->reference_count_ == 0)
    this->handle_close_i ();
}

int
TAO_Connection_Handler::incr_pending_upcalls (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->pending_upcall_lock_, -1);

  return ++this->pending_upcalls_;


}

int
TAO_Connection_Handler::decr_pending_upcalls (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->pending_upcall_lock_, -1);

  return --this->pending_upcalls_;
}

int
TAO_Connection_Handler::handle_close_eh (
    ACE_HANDLE handle, unsigned long reactor_mask, ACE_Event_Handler * eh)
{
  ACE_HANDLE my_handle = eh->get_handle ();

  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::handle_close_eh, "
                   "(%d, %d)\n",
                   my_handle, handle, reactor_mask));
    }

  if(my_handle == ACE_INVALID_HANDLE)
    {
      return 0;
    }

  // Just close the socket irrespective of what the upcall count is,
  // we need to cleanup OS resources ASAP.
  this->release_os_resources ();

  // Set the handle to be INVALID_HANDLE
  eh->set_handle (ACE_INVALID_HANDLE);

  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);

  // @@ TODO All this code dealing with upcalls is fishy, upcalls are
  //    incremented/decremented in handle_input_i(), and only
  //    decremented here!  Also: the reference count is decremented
  //    here, while it is incremented in a selected few other places,
  //    in a very confusing ways.
  long upcalls = this->decr_pending_upcalls ();

  ACE_ASSERT(upcalls >= 0);

  // If the upcall count is zero start the cleanup.
  if (upcalls == 0)
    this->decr_refcount ();

  return 0;
}

void
TAO_Connection_Handler::handle_close_i_eh (
    ACE_Event_Handler * eh)
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) - Connection_Handler[%d]::handle_close_i, "
                 "\n",
                 this->transport ()->id ()));

  if (this->transport ()->wait_strategy ()->is_registered ())
    {
      // Make sure there are no timers.
      ACE_ASSERT(eh->reactor() != 0);
      eh->reactor ()->cancel_timer (eh);

      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->transport ()->wait_strategy ()->is_registered (0);
    }

  // Close the handle..
  // Remove the entry as it is invalid
  this->transport ()->purge_entry ();

  // Signal the transport that we will no longer have
  // a reference to it.  This will eventually call
  // TAO_Transport::release ().
  this->transport (0);

  delete this;
}

int
TAO_Connection_Handler::handle_output_eh (
    ACE_HANDLE, ACE_Event_Handler * eh)
{
  // Instantiate the resume handle here.. This will automatically
  // resume the handle once data is written..
  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   eh->get_handle ());

  int return_value = 0;
  this->pre_io_hook (return_value);
  if (return_value != 0)
    {
      return return_value;
    }

  return_value = this->transport ()->handle_output ();

  this->pos_io_hook(return_value);
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
  // Increase the reference count on the upcall that have passed us.
  long upcalls = this->incr_pending_upcalls ();

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - IIOP_Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, upcalls = %d\n",
                  this->transport()->id(), handle, h, upcalls));
    }
  ACE_ASSERT (upcalls > 0);

  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   eh->get_handle ());

  int return_value = 0;

  this->pre_io_hook (return_value);
  if (return_value != 0)
    {
      return return_value;
    }

  return_value = this->transport ()->handle_input_i (resume_handle);

  this->pos_io_hook(return_value);

  // The upcall is done. Bump down the reference count
  upcalls = this->decr_pending_upcalls ();

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - IIOP_Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, upcalls = %d, retval = %d\n",
                  this->transport()->id(), handle, h, upcalls, return_value));
    }
  ACE_ASSERT (upcalls >= 0);

  if (upcalls == 0)
    {
      this->decr_refcount ();

      // As we have already performed the handle closing (indirectly)
      // we dont want to return a  -1. Doing so would make the reactor
      // call handle_close () which could be harmful.
      return_value = 0;
    }

  if (return_value != 0 || upcalls == 0)
    {
      // This is really a odd case. We could have a race condition if
      // we dont do this. Looks like this what happens
      // - imagine we have more than 1 server threads
      // - The server has got more than one connection from the
      //   clients
      // - The clients make requests and they start dissappearing.
      // - The connections start getting closed
      // - at that point one of the server threads is woken up to
      //   and handle_input () is called.
      // - the handle_input sees no data and so is about return a -1.
      // - if the handle is resumed, it looks like the oen more thread
      //   gets access to the handle and the handle_input is called by
      //   another thread.
      // - at that point of time if the thread returning -1 to the
      //   reactor starts closing down the handler, bad things start
      //   happening.
      // Looks subtle though. After adding this I dont see anything
      // bad happenin and so let us stick with it...
      resume_handle.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
    }

  return return_value;
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
