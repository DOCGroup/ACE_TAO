// $Id$

#include "DIOP_Connection_Handler.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"

#include "DIOP_Transport.h"
#include "DIOP_Endpoint.h"

#if !defined (__ACE_INLINE__)
# include "DIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, DIOP_Connect, "$Id$")

TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_DIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    tcp_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_DIOP_Connection_Handler::TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          CORBA::Boolean flag,
                                                          void *arg)
  : TAO_DIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    tcp_properties_ (ACE_static_cast
                     (TAO_DIOP_Properties *, arg))
{
  TAO_DIOP_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_DIOP_Transport(this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport(specific_transport);
  TAO_Transport::release (specific_transport);
}


TAO_DIOP_Connection_Handler::~TAO_DIOP_Connection_Handler (void)
{
  this->udp_socket_.close ();
}

// DIOP Additions - Begin
ACE_HANDLE
TAO_DIOP_Connection_Handler::get_handle (void) const
{
  return this->udp_socket_.get_handle ();
}


const ACE_INET_Addr &
TAO_DIOP_Connection_Handler::addr (void)
{
  return this->addr_;
}


void
TAO_DIOP_Connection_Handler::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}


const ACE_INET_Addr &
TAO_DIOP_Connection_Handler::local_addr (void)
{
  return local_addr_;
}


void
TAO_DIOP_Connection_Handler::local_addr (const ACE_INET_Addr &addr)
{
  local_addr_ = addr;
}


const ACE_SOCK_Dgram &
TAO_DIOP_Connection_Handler::dgram (void)
{
  return this->udp_socket_;
}
// DIOP Additions - End


int
TAO_DIOP_Connection_Handler::open (void*)
{
  this->udp_socket_.open (this->local_addr_);

  if(TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_DIOP_Connection_Handler::open -")
                 ACE_TEXT("listening on: <%s:%u>\n"),
                 this->local_addr_.get_host_name (),
                 this->local_addr_.get_port_number ()));
  }

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  return 0;
}

int
TAO_DIOP_Connection_Handler::open_server (void)
{
  this->udp_socket_.open (this->local_addr_);
  if( TAO_debug_level > 5)
  {
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("\nTAO (%P|%t) TAO_DIOP_Connection_Handler::open_server -")
                 ACE_TEXT("listening on %s:%d\n"),
                 this->local_addr_.get_host_name (),
                 this->local_addr_.get_port_number ()
               ));
  }

  this->transport ()->id ((int) this->get_handle ());

  return 0;
}

int
TAO_DIOP_Connection_Handler::activate (long flags,
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
                 ACE_TEXT ("TAO (%P|%t) DIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));
  // @@ Michael: I believe we do not need active service handlers right now.
  // @@ Frank: Not disabled yet...

  // Set the id in the transport now that we're active.
  this->transport ()->id ((int) this->get_handle ());

  return TAO_DIOP_SVC_HANDLER::activate (flags,
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
TAO_DIOP_Connection_Handler::svc (void)
{
  // @@ Michael: I believe we do not need active service handlers right now.
  // @@ Frank: Not yet...

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
TAO_DIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                           ACE_Reactor_Mask rm)
{
  // @@ Alex: we need to figure out if the transport decides to close
  //    us or something else.  If it is something else (for example
  //    the cached connector trying to make room for other
  //    connections) then we should let the transport know, so it can
  //    in turn take appropiate action (such as sending exceptions to
  //    all waiting reply handlers).
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("DIOP_Connection_Handler::handle_close ")
                 ACE_TEXT ("(%d, %d)\n"),
                 handle,
                 rm));

  long upcalls =
    this->decr_pending_upcalls ();

  ACE_ASSERT (upcalls >= 0);

  // Try to clean up things if the upcall count has reached 0
  if (upcalls == 0)
    this->handle_close_i ();

  return 0;
}

void
TAO_DIOP_Connection_Handler::handle_close_i (void)
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("DIOP_Connection_Handler::handle_close_i ")
                 ACE_TEXT ("(%d)\n"),
                 this->transport ()->id ()));

  // @@ Why are we doing checks for is_registered flags here if the
  // handlers are not registered with the reactor? - Bala
  if (this->transport ()->wait_strategy ()->is_registered ())
    {
      // @@ Frank: Added reactor check.  not sure if this is right?
      if (this->reactor ())
        {
          // Make sure there are no timers.
          this->reactor ()->cancel_timer (this);
        }

      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->transport ()->wait_strategy()->is_registered (0);
    }

  // Close the handle..
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Remove the entry as it is invalid
      this->transport ()->purge_entry ();

      // Signal the transport that we will no longer have
      // a reference to it.  This will eventually call
      // TAO_Transport::release ().
      this->transport (0);
    }

  // Follow usual Reactor-style lifecycle semantics and commit
  // suicide.
  this->destroy ();
}


int
TAO_DIOP_Connection_Handler::resume_handler (void)
{
  return TAO_RESUMES_CONNECTION_HANDLER;
}


int
TAO_DIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  DIOP_Endpoint object
  TAO_DIOP_Endpoint endpoint (addr,
                              0);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&prop,
                                                                                  this->transport ());
}

// @@ Frank: Hopefully this isn't needed
/*
int
TAO_DIOP_Connection_Handler::process_listen_point_list (
    DIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      DIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());


      // Construct an  DIOP_Endpoint object
      TAO_DIOP_Endpoint endpoint (addr,
                                  0);

      // Construct a property object
      TAO_Base_Transport_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      int retval = this->transport ()->recache_transport (&prop);
      if (retval == -1)
        return retval;

      // Make the handler idle and ready for use
      this->transport ()->make_idle ();
    }

  return 0;
}
*/

int
TAO_DIOP_Connection_Handler::handle_input (ACE_HANDLE)
{
  // Increase the reference count on the upcall that have passed us.
  this->incr_pending_upcalls ();

  TAO_Resume_Handle  resume_handle (this->orb_core (),
                                    this->get_handle ());

  int retval = this->transport ()->handle_input_i (resume_handle);

  // Now the message has been read
  if (retval == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("DIOP_Connection_Handler::handle_input \n")));

    }

  // The upcall is done. Bump down the reference count
  long upcalls = this->decr_pending_upcalls ();

  ACE_ASSERT (upcalls >= 0);

  // Try to clean up things if the upcall count has reached 0
  if (upcalls == 0)
    {
      this->handle_close_i ();

      // As we have already performed the handle closing we dont want
      // to return a  -1. Doing so would make the reactor call
      // handle_close () which could be harmful.
      retval = 0;
    }

  if (retval == -1)
    {
      // @@ Bala: I am not sure why I am doing this here. But let it
      // be there for uniformity sake..
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
  // @@ Michael:
  // We always return 0, as we do not have any send errors.
  return 0;
}



// @@ Frank: From DIOP_Connect.cpp
int
TAO_DIOP_Connection_Handler::handle_cleanup (void)
{
  // Deregister this handler with the ACE_Reactor.
  if (this->reactor ())
    {
      ACE_Reactor_Mask mask =
        ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;

      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Remove self from reactor.
      this->reactor ()->remove_handler (this, mask);
    }

  return 0;
}




// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Instantiated in IIOP_Connection_Handler.cpp
//template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

template class ACE_Concurrency_Strategy<TAO_DIOP_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_DIOP_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Instantiated in IIOP_Connection_Handler.cpp
//#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#pragma instantiate ACE_Concurrency_Strategy<TAO_DIOP_Connection_Handler>
#pragma instantiate ACE_Creation_Strategy<TAO_DIOP_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
