// $Id$

#include "UIOP_Connection_Handler.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Transport.h"
#include "UIOP_Endpoint.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Timeprobe.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Base_Transport_Property.h"
#include "tao/GIOP_Message_Lite.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Resume_Handle.h"
#include "tao/Thread_Lane_Resources.h"

#if !defined (__ACE_INLINE__)
# include "UIOP_Connection_Handler.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, UIOP_Connection_Handler, "$Id$")

TAO_UIOP_Connection_Handler::TAO_UIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    uiop_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_UIOP_Connection_Handler::TAO_UIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          CORBA::Boolean flag,
                                                          void *arg)
  : TAO_UIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    uiop_properties_ (ACE_static_cast
                     (TAO_UIOP_Properties *, arg))
{
  TAO_UIOP_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_UIOP_Transport(this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport(specific_transport);
  TAO_Transport::release (specific_transport);
}


TAO_UIOP_Connection_Handler::~TAO_UIOP_Connection_Handler (void)
{
}

int
TAO_UIOP_Connection_Handler::open (void*)
{
  if (this->set_socket_option (this->peer (),
                               this->uiop_properties_->send_buffer_size,
                               this->uiop_properties_->recv_buffer_size) == -1)
    return -1;

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_UNIX_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) UIOP connection to server ")
                ACE_TEXT ("<%s> on %d\n"),
                addr.get_path_name (), this->peer ().get_handle ()));

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  this->state_changed (TAO_LF_Event::LFS_SUCCESS);

  return 0;
}

int
TAO_UIOP_Connection_Handler::activate (long flags,
                                       int n_threads,
                                       int force_active,
                                       long priority,
                                       int grp_id,
                                       ACE_Task_Base *task,
                                       ACE_hthread_t thread_handles[],
                                       void *stack[],
                                       size_t stack_size[],
                                       ACE_thread_t  thread_names[])
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) UIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  return TAO_UIOP_SVC_HANDLER::activate (flags,
                                         n_threads,
                                         force_active,
                                         priority,
                                         grp_id,
                                         task,
                                         thread_handles,
                                         stack,
                                         stack_size,
                                         thread_names);
}

int
TAO_UIOP_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".

  // Clear the non-blocking mode here
  ACE_Flag_Manip::clr_flags (this->get_handle (),
                             ACE_NONBLOCK);

  // Call the implementation here
  return this->svc_i ();
}


int
TAO_UIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                           ACE_Reactor_Mask rm)
{
  // @@ TODO this too similar to IIOP_Connection_Handler::handle_close,
  //    in fact, even the comments were identical.  IMHO needs
  //    re-factoring.
  ACE_HANDLE my_handle = this->get_handle ();

  if (TAO_debug_level)
    {
      ACE_DEBUG  ((LM_DEBUG,
                   "TAO (%P|%t) - UIOP_Connection_Handler[%d]::handle_close, "
                   "(%d, %d)\n",
                   my_handle, handle, rm));
    }

  if(my_handle == ACE_INVALID_HANDLE)
    {
      return 0;
    }
  this->peer().close ();

  this->set_handle (ACE_INVALID_HANDLE);

  long upcalls = this->decr_pending_upcalls ();
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED);

  if (upcalls < 0)
    return 0;

  if (upcalls == 0)
    this->decr_refcount ();

  return 0;
}

void
TAO_UIOP_Connection_Handler::handle_close_i (void)
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("UIOP_Connection_Handler::handle_close_i ")
                 ACE_TEXT ("(%d)\n"),
                 this->transport ()->id ()));
  if (this->transport ()->wait_strategy ()->is_registered ())
    {
      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->transport ()->wait_strategy ()->is_registered (0);
    }

  // Remove the entry as it is invalid
  this->transport ()->purge_entry ();

  // Signal the transport that we will no longer have
  // a reference to it.  This will eventually call
  // TAO_Transport::release ().
  this->transport (0);

  this->destroy ();
}

int
TAO_UIOP_Connection_Handler::resume_handler (void)
{
  return TAO_RESUMES_CONNECTION_HANDLER;
}


int
TAO_UIOP_Connection_Handler::handle_output (ACE_HANDLE)
{
  TAO_Resume_Handle  resume_handle (this->orb_core (),
                                    this->get_handle ());
  return this->transport ()->handle_output ();
}

int
TAO_UIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_UNIX_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  UIOP_Endpoint object
  TAO_UIOP_Endpoint endpoint (addr);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&prop,
                                                                                  this->transport ());
}

int
TAO_UIOP_Connection_Handler::handle_input (ACE_HANDLE)
{
  this->incr_pending_upcalls ();

  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   this->get_handle ());

  int retval =
    this->transport ()->handle_input_i (resume_handle);

  // The upcall is done. Bump down the reference count
  long upcalls = this->decr_pending_upcalls ();

  // Try to clean up things if the upcall count has reached 0
  if (upcalls == 0)
    {
      this->decr_refcount ();

      // As we have already performed the handle closing we dont want
      // to return a  -1. Doing so would make the reactor call
      // handle_close () which could be harmful.
      retval = 0;
    }
  else if (upcalls < 0)
    {
      retval = 0;
    }

  if (retval == -1)
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

  return retval;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /*TAO_HAS_UIOP == 1*/
