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
#include "tao/Connection_Cache_Manager.h"
#include "tao/Base_Connection_Property.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, IIOP_Connect, "$Id$")



TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    transport_ (this, 0, 0),
    refcount_ (1),
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
    transport_ (this, orb_core, flag),
    refcount_ (1),
    tcp_properties_ (ACE_static_cast
                     (TAO_IIOP_Properties *, arg))
{
}


TAO_IIOP_Connection_Handler::~TAO_IIOP_Connection_Handler (void)
{

  // If the socket has not already been closed.
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Cannot deal with errors, and therefore they are ignored.
      this->transport_.send_buffered_messages ();
    }
  else
    {
      // Dequeue messages and delete message blocks.
      this->transport_.dequeue_all ();
    }
}


// @@ Should I do something here to enable non-blocking?? (Alex).
// @@ Alex: I don't know if this is the place to do it, but the way to
//    do it is:
//    if (this->peer ().enable (ACE_NONBLOCK) == -1)
//       return -1;
//    Probably we will need to use the transport to decide if it is
//    needed or not.

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

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr addr;

  char client[MAXHOSTNAMELEN + 16];

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Verify that we can resolve the peer hostname.
  else if (addr.addr_to_string (client, sizeof (client)) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint endpoint (addr,
                              0);

  // Construct a property object
  TAO_Base_Connection_Property prop (&endpoint);

  // Add the handler to Cache
  if (this->orb_core ()->connection_cache ().cache_handler (&prop,
                                                            this) == -1)
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) unable to cache the handle \n")));
        }
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) IIOP connection from client")
                  ACE_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

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
TAO_IIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                           ACE_Reactor_Mask rm)
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("IIOP_Connection_Handler::handle_close ")
                 ACE_TEXT ("(%d, %d)\n"),
                 handle,
                 rm));

  --this->refcount_;
  if (this->refcount_ == 0 &&
      this->is_registered ())
    {
      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->is_registered (0);

      // Decrement the reference count
      this->decr_ref_count ();

      //return TAO_IIOP_SVC_HANDLER::handle_close (handle, rm);
    }

  return 0;
}

int
TAO_IIOP_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".

  // Call the implementation here
  return this->svc_i ();
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
  this->refcount_++;

  cout << "Getting called " <<endl;
  // Call the transport read the message
  int result = this->transport_.read_process_message (max_wait_time);

  // Now the message has been read
  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("IIOP_Connection_Handler::read_message \n")));

    }

  if (result == 0 || result == -1)
    {
      --this->refcount_;
      if (this->refcount_ == 0)
          this->decr_ref_count ();

      return result;
    }



  //
  // Take out all the information from the <message_state> and reset
  // it so that nested upcall on the same transport can be handled.
  //

  // Notice that the message_state is only modified in one thread at a
  // time because the reactor does not call handle_input() for the
  // same Event_Handler in two threads at the same time.

  // Copy message type.
  /*  TAO_GIOP_Message_State &ms = this->transport_.message_state_;
  CORBA::Octet message_type = ms.message_type;

  // Copy version.
  TAO_GIOP_Version giop_version = ms.giop_version;

  // Steal the input CDR from the message state.
  TAO_InputCDR input_cdr (ACE_InputCDR::Transfer_Contents (ms.cdr),
                          this->orb_core ());

  // Send the message state for the service layer like FT to log the
  // messages
  this->orb_core ()->services_log_msg_rcv (ms);

  // Reset the message state.
  this->transport_.message_state_.reset (0);
  result =
    this->acceptor_factory_->process_client_message (this->transport (),
                                                     this->orb_core (),
                                                     input_cdr,
                                                     message_type);

  if (result != -1)
  result = 0;*/

  --this->refcount_;
  if (this->refcount_ == 0)
    this->decr_ref_count ();
  //this->TAO_IIOP_SVC_HANDLER::handle_close ();

  return 0;
}

ACE_HANDLE
TAO_IIOP_Connection_Handler::fetch_handle (void)
{
  return this->get_handle ();
}

int
TAO_IIOP_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}


/*int
TAO_IIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  int r = this->transport ()->handle_client_input ();
  if (r == -1)
    return -1;
  return 0;
}
*/

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

/*int
TAO_IIOP_Client_Connection_Handler::handle_close (ACE_HANDLE handle,
                                                  ACE_Reactor_Mask rm)
{
  // @@ Alex: we need to figure out if the transport decides to close
  //    us or something else.  If it is something else (for example
  //    the cached connector trying to make room for other
  //    connections) then we should let the transport know, so it can
  //    in turn take appropiate action (such as sending exceptions to
  //    all waiting reply handlers).

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) IIOP_Client_Connection_Handler::")
                 ACE_TEXT ("handle_close (%d, %d)\n"), handle, rm));

  // Deregister this handler with the ACE_Reactor.
  return this->handle_cleanup ();
}

int
TAO_IIOP_Client_Connection_Handler::handle_close_i (ACE_HANDLE handle,
                                                    ACE_Reactor_Mask rm)
{
  // @@ Alex: we need to figure out if the transport decides to close
  //    us or something else.  If it is something else (for example
  //    the cached connector trying to make room for other
  //    connections) then we should let the transport know, so it can
  //    in turn take appropiate action (such as sending exceptions to
  //    all waiting reply handlers).

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) IIOP_Client_Connection_Handler::")
                 ACE_TEXT ("handle_close_i (%d, %d)\n"), handle, rm));

  // Deregister this handler with the ACE_Reactor.
  return this->handle_cleanup ();
}
*/

int
TAO_IIOP_Connection_Handler::handle_cleanup (void)
{
  // Call the implementation.
  if (this->reactor ())
    {
      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);
    }

  // Now do the decrement of the ref count
  this->decr_ref_count ();

  return 0;
}



// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
