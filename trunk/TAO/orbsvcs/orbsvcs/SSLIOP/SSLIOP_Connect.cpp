// $Id$

#include "ace/config-all.h"

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1

#include "SSLIOP_Connect.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/GIOP_Message_Lite.h"
#include "tao/GIOP_Message_Acceptors.h"
#include "tao/Server_Strategy_Factory.h"

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Connect.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(TAO_SSLIOP, SSLIOP_Connect, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_SSLIOP_Connect_Timeprobe_Description[] =
{
  "SSLIOP_Server_Connection_Handler::handle_input - start",
  "SSLIOP_Server_Connection_Handler::handle_input - end",

  "SSLIOP_Server_Connection_Handler::handle_locate - start",
  "SSLIOP_Server_Connection_Handler::handle_locate - end",

  "SSLIOP_Server_Connection_Handler::receive_request - end",

  "SSLIOP_Client_Connection_Handler::send_request - start",
  "SSLIOP_Client_Connection_Handler::send_request - end"};

enum
{
  // Timeprobe description table start key
  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START = 310,
  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_HANDLE_LOCATE_START,
  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_HANDLE_LOCATE_END,

  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_RECEIVE_REQUEST_END,

  TAO_SSLIOP_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START,
  TAO_SSLIOP_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_SSLIOP_Connect_Timeprobe_Description,
                                  TAO_SSLIOP_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_SSLIOP_Handler_Base::TAO_SSLIOP_Handler_Base (TAO_ORB_Core *orb_core)
  : TAO_SSL_SVC_HANDLER (orb_core->thr_mgr (), 0, 0)
{
}

TAO_SSLIOP_Handler_Base::TAO_SSLIOP_Handler_Base (ACE_Thread_Manager *t)
  : TAO_SSL_SVC_HANDLER (t, 0, 0)
{
}

// ****************************************************************

TAO_SSLIOP_Server_Connection_Handler::TAO_SSLIOP_Server_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SSLIOP_Handler_Base (t),
    transport_ (this, 0),
    acceptor_factory_ (0),
    orb_core_ (0),
    tss_resources_ (0),
    refcount_ (1),
    tcp_properties_ (0)

{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core_ != 0);
}

TAO_SSLIOP_Server_Connection_Handler::TAO_SSLIOP_Server_Connection_Handler (
  TAO_ORB_Core *orb_core,
  CORBA::Boolean /* lite_flag */,
  void *arg)
  : TAO_SSLIOP_Handler_Base (orb_core),
    transport_ (this, orb_core),
    acceptor_factory_ (orb_core),
    orb_core_ (orb_core),
    tss_resources_ (orb_core->get_tss_resources ()),
    refcount_ (1),
    tcp_properties_ (ACE_static_cast
                     (TAO_SSLIOP_Handler_Base::TCP_Properties *, arg))
{
  // The flag that is used to enable GIOPlite is *not* used for
  // SSLIOP.  GIOPlite introduces security holes.  It should not be
  // enabled for SSLIOP, so mark it as an unused argument in the
  // argument list.
}

TAO_SSLIOP_Server_Connection_Handler::~TAO_SSLIOP_Server_Connection_Handler (void)
{
}

int
TAO_SSLIOP_Server_Connection_Handler::open (void*)
{
#if !defined (ACE_LACKS_SOCKET_BUFSIZ)

  // @@ Pointers to size in the calls below used to be int*.
  if (this->peer ().set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                (void *)
                                &this->tcp_properties_->send_buffer_size,
                                sizeof (int)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                     (void *)
                                     &this->tcp_properties_->recv_buffer_size,
                                     sizeof (int)) == -1

           && errno != ENOTSUP)
    return -1;
#endif /* !ACE_LACKS_SOCKET_BUFSIZ */

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *)
                                &this->tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  (void) this->peer ().enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  if (TAO_debug_level > 0)
    {
      char client[MAXHOSTNAMELEN + 16];
      (void) addr.addr_to_string (client, sizeof (client));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) IIOP connection from ")
                  ACE_TEXT ("client <%s> on %d\n"),
                  client,
                  this->peer ().get_handle ()));
    }

  return 0;
}

int
TAO_SSLIOP_Server_Connection_Handler::activate (long flags,
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
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("SSLIOP_Server_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  return TAO_SSL_SVC_HANDLER::activate (flags,
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
TAO_SSLIOP_Server_Connection_Handler::handle_close (ACE_HANDLE handle,
                                                  ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) SSLIOP_Server_Connection_Handler::handle_close "
                 "(%d, %d)\n",
                 handle,
                 rm));

  --this->refcount_;
  if (this->refcount_ == 0)
    {
      // Remove the handle from the ORB Core's handle set so that it
      // isn't included in the set that is passed to the reactor upon
      // ORB destruction.
      TAO_Server_Strategy_Factory *f =
        this->orb_core_->server_factory ();

      if (f->activate_server_connections () == 0)
        (void) this->orb_core_->remove_handle (handle);

      return TAO_SSL_SVC_HANDLER::handle_close (handle, rm);
    }

  return 0;
}

int
TAO_SSLIOP_Server_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".
  int result = 0;

  // Inheriting the ORB_Core tss stuff from the parent thread.
  this->orb_core_->inherit_from_parent_thread (this->tss_resources_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) SSLIOP_Server_Connection_Handler::svc begin\n"));

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

  while (!this->orb_core_->has_shutdown ()
         && result >= 0)
    {
      result = handle_input_i (ACE_INVALID_HANDLE, max_wait_time);
      if (result == -1 && errno == ETIME)
        {
          // Ignore timeouts, they are only used to wake up and
          // shutdown.
          result = 0;
        }
      current_timeout = timeout;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) SSLIOP_Server_Connection_Handler::svc - "
                    "loop <%d>\n", current_timeout.msec ()));
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) SSLIOP_Server_Connection_Handler::svc end\n"));

  return result;
}

int
TAO_SSLIOP_Server_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_i (h);
}

int
TAO_SSLIOP_Server_Connection_Handler::handle_input_i (ACE_HANDLE,
                                                      ACE_Time_Value *max_wait_time)
{
  this->refcount_++;

  int result =
    this->acceptor_factory_.handle_input (this->transport (),
                                          this->orb_core_,
                                          this->transport_.message_state_,
                                          max_wait_time);

  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n "),
                  ACE_TEXT ("SSLIOP_Server_Connection_Handler::handle_input, ")
                  ACE_TEXT ("handle_input")));
    }

  if (result == 0 || result == -1)
    {
      --this->refcount_;
      if (this->refcount_ == 0)
        this->TAO_SSL_SVC_HANDLER::handle_close ();

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
  TAO_GIOP_Message_State &ms = this->transport_.message_state_;
  CORBA::Octet message_type = ms.message_type;

  // Copy version.
  TAO_GIOP_Version giop_version = ms.giop_version;

  // Steal the input CDR from the message state.
  TAO_InputCDR input_cdr (ACE_InputCDR::Transfer_Contents (ms.cdr),
                          this->orb_core_);

  // Reset the message state.
  this->transport_.message_state_.reset (0);

  result =
    this->acceptor_factory_.process_client_message (this->transport (),
                                                    this->orb_core_,
                                                    input_cdr,
                                                    message_type);

  if (result != -1)
    result = 0;

  --this->refcount_;
  if (this->refcount_ == 0)
    this->TAO_SSL_SVC_HANDLER::handle_close ();


  return result;
}

// ****************************************************************

//    transport obj.
TAO_SSLIOP_Client_Connection_Handler::
TAO_SSLIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SSLIOP_Handler_Base (t),
    transport_ (this, 0),
    orb_core_ (0)
{
  // This constructor should *never* get called.  See comments in .h
  ACE_ASSERT (this->orb_core_ != 0);
}

TAO_SSLIOP_Client_Connection_Handler::
TAO_SSLIOP_Client_Connection_Handler (ACE_Thread_Manager *t,
                                      TAO_ORB_Core* orb_core,
                                      CORBA::Boolean /* flag */,
                                      void *arg)
  : TAO_SSLIOP_Handler_Base (t),
    transport_ (this, orb_core),
    orb_core_ (orb_core),
    tcp_properties_ (ACE_static_cast
                     (TAO_IIOP_Handler_Base::TCP_Properties *, arg))
{
  // The flag that is used to enable GIOPlite is *not* used for
  // SSLIOP.  GIOPlite introduces security holes.  It should not be
  // enabled for SSLIOP, so mark it as an unused argument in the
  // argument list.
}

TAO_SSLIOP_Client_Connection_Handler::~TAO_SSLIOP_Client_Connection_Handler (void)
{
}


// @@ Should I do something here to enable non-blocking?? (Alex).
// @@ Alex: I don't know if this is the place to do it, but the way to
//    do it is:
//    if (this->peer ().enable (ACE_NONBLOCK) == -1)
//       return -1;
//    Probably we will need to use the transport to decide if it is
//    needed or not.

int
TAO_SSLIOP_Client_Connection_Handler::open (void *)
{
#if !defined (ACE_LACKS_SOCKET_BUFSIZ)

  if (this->peer ().set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                (void *) &tcp_properties_->send_buffer_size,
                                sizeof (int)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                     (void *) &tcp_properties_->recv_buffer_size,
                                     sizeof (int)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* ACE_LACKS_SOCKET_BUFSIZ */

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("NODELAY failed\n")),
                      -1);
#endif /* ! ACE_LACKS_TCP_NODELAY */

  (void) this->peer ().enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  if (TAO_debug_level > 0)
    {
      char server[MAXHOSTNAMELEN + 16];

      (void) addr.addr_to_string (server, sizeof (server));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) IIOP connection to server ")
                  ACE_TEXT ("<%s> on %d\n"),
                  server,
                  this->peer ().get_handle ()));
    }

  // Register the handler with the Reactor if necessary.
  return this->transport ()->wait_strategy ()->register_handler ();
}

int
TAO_SSLIOP_Client_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}

int
TAO_SSLIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  int r = this->transport ()->handle_client_input ();
  if (r == -1)
    return -1;
  return 0;
}

int
TAO_SSLIOP_Client_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                                    const void *)
{
  //
  // This method is called when buffering timer expires.
  //

  ACE_Time_Value *max_wait_time = 0;

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  TAO_RelativeRoundtripTimeoutPolicy *timeout_policy =
    this->orb_core_->stubless_relative_roundtrip_timeout ();

  // Automatically release the policy
  CORBA::Object_var auto_release = timeout_policy;

  ACE_Time_Value max_wait_time_value;

  // If max_wait_time is not zero then this is not the first attempt
  // to send the request, the timeout value includes *all* those
  // attempts.
  if (timeout_policy != 0)
    {
      timeout_policy->set_time_value (max_wait_time_value);
      max_wait_time = &max_wait_time_value;
    }

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

  // Cannot deal with errors, and therefore they are ignored.
  this->transport ()->send_buffered_messages (max_wait_time);

  return 0;
}

int
TAO_SSLIOP_Client_Connection_Handler::handle_close (ACE_HANDLE handle,
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
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("SSLIOP_Client_Connection_Handler::")
                 ACE_TEXT ("handle_close (%d, %d)\n"),
                 handle,
                 rm));

  if (this->recycler ())
    this->recycler ()->mark_as_closed (this->recycling_act ());

  // Deregister this handler with the ACE_Reactor.
  return this->handle_cleanup ();
}

int
TAO_SSLIOP_Client_Connection_Handler::handle_close_i (ACE_HANDLE handle,
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
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("SSLIOP_Client_Connection_Handler::")
                 ACE_TEXT ("handle_close_i (%d, %d)\n"),
                 handle,
                 rm));

  if (this->recycler ())
    this->recycler ()->mark_as_closed_i (this->recycling_act ());

  return this->handle_cleanup ();
}

int
TAO_SSLIOP_Client_Connection_Handler::handle_cleanup (void)
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

  this->peer ().close ();

  return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif  /* ACE_HAS_SSL */
