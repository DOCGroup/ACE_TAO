// $Id$


#include "tao/IIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Base_Transport_Property.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, IIOP_Connect, "$Id$")



TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    pending_upcalls_ (1),
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
                                                          CORBA::Boolean /* flag*/,
                                                          void *arg)
  : TAO_IIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    pending_upcalls_ (1),
    tcp_properties_ (ACE_static_cast
                     (TAO_IIOP_Properties *, arg))
{
  TAO_IIOP_Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO_IIOP_Transport(this, orb_core, 0));

  // store this pointer (indirectly increment ref count)
  this->transport(specific_transport);
  TAO_Transport::release (specific_transport);
}


TAO_IIOP_Connection_Handler::~TAO_IIOP_Connection_Handler (void)
{
  if (this->transport () != 0) {
    // If the socket has not already been closed.
    if (this->get_handle () != ACE_INVALID_HANDLE)
      {
        // Cannot deal with errors, and therefore they are ignored.
        this->transport ()->send_buffered_messages ();
      }
    else
      {
        // Dequeue messages and delete message blocks.
        this->transport ()->dequeue_all ();
      }
  }
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
          char remote_as_string[MAXHOSTNAMELEN + 16];
          char local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof(local_as_string));
          ACE_ERROR ((LM_ERROR,
                      "TAO(%P|%t) - TAO_IIOP_Connection_Handler::open, "
                      "Holy Cow! The remote addr and "
                      "local addr are identical (%s == %s)\n",
                      remote_as_string, local_as_string));
        }
      return -1;
    }

  if (TAO_debug_level > 0)
    {
      char client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client, sizeof (client)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) IIOP connection from client")
                  ACE_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  this->transport ()->id ( (int) this->get_handle ());

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
                 ACE_TEXT ("TAO (%P|%t) IIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
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
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("IIOP_Connection_Handler::handle_close ")
                 ACE_TEXT ("(%d, %d)\n"),
                 handle,
                 rm));

  --this->pending_upcalls_;
  if (this->pending_upcalls_ <= 0)
    {
      if (this->is_registered ())
        {
          // Make sure there are no timers.
          this->reactor ()->cancel_timer (this);

          // Set the flag to indicate that it is no longer registered with
          // the reactor, so that it isn't included in the set that is
          // passed to the reactor on ORB destruction.
          this->is_registered (0);
        }

      // Close the handle..
      if (this->get_handle () != ACE_INVALID_HANDLE)
        {
          // Send the buffered messages first
          this->transport ()->send_buffered_messages ();

          // Mark the entry as invalid
          this->transport ()->mark_invalid ();

          // Signal the transport that we will no longer have
          // a reference to it.  This will eventually call
          // TAO_Transport::release ().
          this->transport (0);

          this->peer ().close ();
        }

      // Follow usual Reactor-style lifecycle semantics and commit
      // suicide.
      delete this;
    }

  return 0;
}

ACE_HANDLE
TAO_IIOP_Connection_Handler::fetch_handle (void)
{
  return this->get_handle ();
}


int
TAO_IIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                             const void *)
{
  // This method is called when buffering timer expires.
  //
  ACE_Time_Value *max_wait_time = 0;

  TAO_Stub *stub = 0;
  int has_timeout;
  this->orb_core ()->call_timeout_hook (stub,
                                        has_timeout,
                                        *max_wait_time);

  // Cannot deal with errors, and therefore they are ignored.
  this->transport ()->send_buffered_messages (max_wait_time);

  return 0;
}

int
TAO_IIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint endpoint (addr,
                              0);

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->transport_cache ().cache_transport (&prop,
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


      // Construct an  IIOP_Endpoint object
      TAO_IIOP_Endpoint endpoint (addr,
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


int
TAO_IIOP_Connection_Handler::handle_input (ACE_HANDLE h)

{
  return this->handle_input_i (h);
}


int
TAO_IIOP_Connection_Handler::handle_input_i (ACE_HANDLE,
                                             ACE_Time_Value *max_wait_time)
{
  this->pending_upcalls_++;

  // Call the transport read the message
  int result = this->transport ()->read_process_message (max_wait_time);

  // Now the message has been read
  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("IIOP_Connection_Handler::read_message \n")));

    }

  // The upcall is done. Bump down the reference count
  if (--this->pending_upcalls_ <= 0)
    result = -1;

  if (result == -1)
    return result;
  else if (result == 0)
    // Requires another call to handle_input  ()
    return 1;

  return 0;
}







// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
