// $Id$

#include "tao/IIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           IIOP_Connection_Handler,
           "$Id$")


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_SVC_HANDLER (t, 0 , 0),
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


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          CORBA::Boolean flag,
                                                          void *arg)
  : TAO_IIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    tcp_properties_ (ACE_static_cast
                     (TAO_IIOP_Properties *, arg))
{
  TAO_IIOP_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_IIOP_Transport (this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport(specific_transport);
  TAO_Transport::release (specific_transport);
}


TAO_IIOP_Connection_Handler::~TAO_IIOP_Connection_Handler (void)
{
}

int
TAO_IIOP_Connection_Handler::open (void*)
{
  if (this->set_socket_option (this->peer (),
                               this->tcp_properties_->send_buffer_size,
                               this->tcp_properties_->recv_buffer_size) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.

  ACE_INET_Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE_INET_Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (local_addr.get_ip_address () == remote_addr.get_ip_address ()
      && local_addr.get_port_number () == remote_addr.get_port_number ())
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];
          ACE_TCHAR local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof(local_as_string));
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO(%P|%t) - TAO_IIOP_Connection_Handler::open, ")
                      ACE_LIB_TEXT("Holy Cow! The remote addr and ")
                      ACE_LIB_TEXT("local addr are identical (%s == %s)\n"),
                      remote_as_string, local_as_string));
        }
      return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client, sizeof (client)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("TAO (%P|%t) IIOP connection to peer ")
                  ACE_LIB_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  this->transport ()->id ((int) this->get_handle ());

  return 0;
}

int
TAO_IIOP_Connection_Handler::activate (long flags,
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
                 ACE_LIB_TEXT ("TAO (%P|%t) IIOP_Connection_Handler::activate %d ")
                 ACE_LIB_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  this->transport ()->id ((int) this->get_handle ());

  return TAO_IIOP_SVC_HANDLER::activate (flags,
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
TAO_IIOP_Connection_Handler::svc (void)
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
TAO_IIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
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
               ACE_LIB_TEXT ("TAO (%P|%t) ")
               ACE_LIB_TEXT ("IIOP_Connection_Handler::handle_close ")
               ACE_LIB_TEXT ("(%d, %d)\n"),
               handle,
               rm));

  long upcalls = this->decr_pending_upcalls ();

  // Just return incase the upcall count goes below 0.
  if (upcalls < 0)
    return 0;

  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Just close the socket irrespective of what the upcall count
      // is.
      this->peer().close ();

      // Set the handle to be INVALID_HANDLE
      this->set_handle (ACE_INVALID_HANDLE);
    }

  // If the upcall count is zero start the cleanup.
  if (upcalls == 0)
    this->handle_close_i ();

  return 0;
}


void
TAO_IIOP_Connection_Handler::handle_close_i (void)
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_LIB_TEXT ("TAO (%P|%t) ")
                 ACE_LIB_TEXT ("IIOP_Connection_Handler::handle_close_i ")
                 ACE_LIB_TEXT ("(%d)\n"),
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

  // Close the handle..
  // Remove the entry as it is invalid
  this->transport ()->purge_entry ();

  // Signal the transport that we will no longer have
  // a reference to it.  This will eventually call
  // TAO_Transport::release ().
  this->transport (0);

  // Follow usual Reactor-style lifecycle semantics and commit
  // suicide.
  this->destroy ();
}



int
TAO_IIOP_Connection_Handler::resume_handler (void)
{
  return TAO_RESUMES_CONNECTION_HANDLER;
}

int
TAO_IIOP_Connection_Handler::handle_output (ACE_HANDLE)
{
  // Instantiate the resume handle here.. This will automatically
  // resume the handle once data is written..
  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   this->get_handle ());
  return this->transport ()->handle_output ();
}

int
TAO_IIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint endpoint (
      addr,
      this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&prop,
                                                                                  this->transport ());
}

int
TAO_IIOP_Connection_Handler::process_listen_point_list (
    IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("(%P|%t) Listening port [%d] on [%s]\n"),
                      listen_point.port,
                      ACE_TEXT_CHAR_TO_TCHAR(listen_point.host.in ())));
        }

      // Construct an  IIOP_Endpoint object
      TAO_IIOP_Endpoint endpoint (addr,
        this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

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


int
TAO_IIOP_Connection_Handler::handle_input (ACE_HANDLE)
{
  // Increase the reference count on the upcall that have passed us.
  long upcalls = this->incr_pending_upcalls ();

  TAO_Resume_Handle resume_handle (this->orb_core (),
                                   this->get_handle ());

  int retval = this->transport ()->handle_input_i (resume_handle);

  // The upcall is done. Bump down the reference count
  upcalls = this->decr_pending_upcalls ();

  if (upcalls == 0)
    {
      this->handle_close_i ();

      // As we have already performed the handle closing we dont want
      // to return a  -1. Doing so would make the reactor call
      // handle_close () which could be harmful.
      retval = 0;
    }
  else if (upcalls < 0)
    {
      // As we have already performed the handle closing we dont want
      // to return a  -1. Doing so would make the reactor call
      // handle_close () which could be harmful.
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





// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
