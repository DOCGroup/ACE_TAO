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
  , reference_count_ (1)
{
  // @@todo: We need to have a distinct option/ method in the resource
  // factory for this and TAO_Transport.

  this->refcount_lock_ =
    this->orb_core_->resource_factory ()->create_cached_connection_lock ();

  this->lock_ =
    this->orb_core_->resource_factory ()->create_cached_connection_lock ();

  // Put ourselves in the connection wait state as soon as we get
  // created
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_WAIT);
}


TAO_Connection_Handler::~TAO_Connection_Handler (void)
{
  ACE_ASSERT(this->transport_ == 0);
  ACE_ASSERT(this->reference_count_ == 0);

  // @@ TODO Use auto_ptr<>
  delete this->lock_;
  delete this->refcount_lock_;
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
  // The transport can be reset, but not changed!
  ACE_ASSERT(this->transport_ == 0 || transport == 0);

  TAO_Transport * tmp = 0;
  {
    // Make the change atomic
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    tmp = this->transport_;
    this->transport_ = TAO_Transport::_duplicate (transport);
  }

  if (tmp != 0) {
    tmp->connection_handler_closing ();
    TAO_Transport::release (tmp);
  }
}

long
TAO_Connection_Handler::incr_refcount (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, -1);

  ACE_ASSERT(this->reference_count_ > 0);
  return ++this->reference_count_;
}

long
TAO_Connection_Handler::decr_refcount (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, -1);

    if(--this->reference_count_ > 0)
      return this->reference_count_;
  }

  ACE_ASSERT(this->reference_count_ == 0);


  int r = this->release_os_resources ();

  if (r == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO (%P|%t) - Connection_Handler::"
                  "decr_refcount, release_os_resources() failed %p\n"));
    }

  delete this;

  return 0;
}

void
TAO_Connection_Handler::connection_close_wait (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSE_WAIT);
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

  if (this->close_connection () == 0)
    {
      if (TAO_debug_level)
        ACE_DEBUG  ((LM_DEBUG,
                     "TAO (%P|%t) - Connection_Handler[%d]::"
                     "handle_close, connection closing or already closed\n",
                     my_handle));
      return 0;
    }

  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) - Connection_Handler[%d]::"
                 "handle_close, connection fully closed\n",
                 my_handle));

  return 0;
}

int
TAO_Connection_Handler::handle_output_eh (
    ACE_HANDLE, ACE_Event_Handler * eh)
{
  // We are going to use the transport object to write data. Just make
  // sure that we have transport objects to proceed. This  is
  // perfectly valid assert
  ACE_ASSERT (this->transport () != 0);

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
  // We are going to use the transport object to read data. Just make
  // sure that we have transport objects to proceed. This  is
  // perfectly valid assert
  ACE_ASSERT (this->transport () != 0);

  // Let the transport know that it is used
  (void) this->transport ()->update_transport ();

  // Grab the transport id now and use the cached value for printing
  // since the  transport could dissappear by the time the thread
  // returns.
  size_t t_id =
    this->transport ()->id ();


  // Increase the reference count on the upcall that have passed us.
  //
  // REFCNT: Matches decr_refcount() in this function...
  long refcount = this->incr_refcount ();
  ACE_ASSERT (refcount > 0);

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, refcount = %d\n",
                  t_id, handle, h, refcount));
    }

  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   eh->get_handle ());

  int return_value = 0;

  this->pre_io_hook (return_value);
  if (return_value != 0)
    {
      // REFCNT: Matches incr_refcount() at the beginning...
      refcount = this->decr_refcount ();
      ACE_ASSERT (refcount >= 0);
      return return_value;
    }


  return_value = this->transport ()->handle_input_i (resume_handle);

  this->pos_io_hook(return_value);

  // REFCNT: Matches incr_refcount() at the beginning...
  refcount = this->decr_refcount ();
  ACE_ASSERT (refcount >= 0);

  if (TAO_debug_level > 6)
    {
      ACE_HANDLE handle = eh->get_handle ();
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Connection_Handler[%d]::handle_input, "
                  "handle = %d/%d, refcount = %d, retval = %d\n",
                  t_id, handle, h, refcount, return_value));
    }

  if (return_value == -1 || refcount == 0)
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
TAO_Connection_Handler::close_connection_eh (ACE_Event_Handler * eh)
{
  // Perform a double checked locking on the underlying ACE_HANDLE
  ACE_HANDLE handle = eh->get_handle ();

  // If the handle is ACE_INVALID_HANDLE then there is no work to be
  // done in this function, and we return immediately.  Returning 0
  // indicates the caller (handle_close() most likely), that there is
  // no work to be done.
  if (handle == ACE_INVALID_HANDLE)
    {
      return 0;
    }

  size_t id = 0;
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

    handle = eh->get_handle ();

    // Double-checked locking.
    if(handle == ACE_INVALID_HANDLE)
      {
        return 0;
      }

    // Before closing the socket we need to remove ourselves from the
    // Reactor.  Sounds silly, as supposedly handle_close() was called
    // *BY* the Reactor, but the Reactor calls handle_close() with
    // only the masks implied by the handle_XXX() call that returned
    // -1, and it does *NOT* remove the Event Handler from all masks.
    // Furthermore, this method is also called outside the Reactor
    // event loop, for example, when an I/O error is detected during a
    // write().

    // The following assertion is true because:
    //
    //
    // 1) When a connection handler is initialized Transport is not zero
    //    and the handle is *NOT* ACE_INVALID_HANDLE.
    // 2) The code here is run only once, if we get to this point the
    //    handle was not ACE_INVALID_HANDLE
    // 3) this->transport() is only reset after we run this code
    //    successfully
    //
    // Or: for this code to run the handle must have changed state from
    // something valid to ACE_INVALID_HANDLE, and the transport() field
    // will not be changed before that state transition.
    //
    ACE_ASSERT (this->transport () != 0);

    // Save the ID for debugging messages
    id = this->transport()->id ();
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
        ACE_Reactor * eh_reactor = eh->reactor ();

        // These checks are valid as long as the ORB_Core is not
        // shutdown. It is good to have these checks and they are valid
        // for most of the cases. Please see below for exceptions
        if (this->orb_core_->has_shutdown () == 0)
          {
            // The exception when these are not valid is for RTCORBA. With
            // RTCORBA on, you can threads in different lanes creating
            // handlers and registering them with reactor in those
            // respective lanes. These threads could then even go away
            // leaving the task of reclaiming these resources to the main
            // thread. For the main thread that takes the responsibility
            // of finalizing () the lanes and the pools, the calls and
            // comparison make no sense.
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

        int r =
          eh_reactor->remove_handler (handle,
                                      (ACE_Event_Handler::ALL_EVENTS_MASK
                                       | ACE_Event_Handler::DONT_CALL));
        if(r == -1 && TAO_debug_level)
          {
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - Connection_Handler[%d]::"
                        "close_connection, problem in remove_handler (%d)\n",
                        handle, r));
          }

        // Also cancel any timers, we may create those for time-limited
        // buffering
        if (TAO_debug_level)
          {
            ACE_DEBUG  ((LM_DEBUG,
                         "TAO (%P|%t) - Connection_Handler[%d]::"
                         "close_connection, cancel all timers and refcount [%d]\n",
                         handle, reference_count_));
          }
        r = eh_reactor->cancel_timer (eh);
        if (r == -1 && TAO_debug_level)
          {
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - Connection_Handler[%d]::"
                        "close_connection, error cancelling timers\n",
                        handle));
          }

        // @@ This seems silly, the reactor is a much better authority to
        //    find out if a handle is registered...
        this->transport ()->wait_strategy ()->is_registered (0);
      }

    this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);
  }

  ACE_ASSERT (this->transport () != 0);

  // Signal the transport that we will no longer have
  // a reference to it.  This will eventually call
  // TAO_Transport::release ().
  this->transport (0);

  // The Reactor (or the Connector) holds an implicit reference.
  // REFCNT: Matches start count
  // REFCNT: only this or handle_input_eh() are called
  long refcount = this->decr_refcount ();

  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - Connection_Handler[%d]::"
                   "close_connection, refcount = %d\n",
                   id, refcount));
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
