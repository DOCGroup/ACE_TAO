// $Id$

#if !defined (CPP_CONNECTOR_C)
#define CPP_CONNECTOR_C

#include "CPP-connector.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Signal.h"

template <typename PEER_STREAM>
Peer_Handler<PEER_STREAM>::Peer_Handler (ACE_Reactor *r)
  : action_ (&Peer_Handler<PEER_STREAM>::uninitialized)
{
  this->reactor (r);
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("activating %d\n"),
              this->peer ().get_handle ()));
  this->action_ = &Peer_Handler<PEER_STREAM>::connected;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("please enter input..: ")));

  if (this->reactor ())

#if defined (ACE_WIN32)
    // On Win32, the stdin HANDLE must be registered directly (and not
    // as a socket).
    this->reactor ()->register_handler (this,
                                        ACE_STDIN);
#else
    // On non-Win32, the stdin HANDLE must be registered as a normal
    // handle with the <READ_Mask>.
    this->reactor ()->register_handler (ACE_STDIN,
                                        this,
                                        ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 */
  else
    {
      while (this->connected () != -1)
        continue;

      this->handle_close (ACE_INVALID_HANDLE,
                          ACE_Event_Handler::READ_MASK);
    }
  return 0;
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::close (u_long)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Connect not successful: ending reactor event loop\n")));
  this->reactor ()->end_reactor_event_loop();
  return 0;
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::uninitialized (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("uninitialized!\n")));
  return 0;
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::connected (void)
{
  char buf[BUFSIZ];

  ssize_t n = ACE_OS::read (ACE_STDIN,
                            buf,
                            sizeof buf);

  if (n > 0
      && this->peer ().send_n (buf,
                               n) != n)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("write failed")),
                      -1);
  else if (n == 0)
    {
      // Explicitly close the connection.
      if (this->peer ().close () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("close")),
                          1);
      return -1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("please enter input..: ")));
      return 0;
    }
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::stdio (void)
{
  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("in stdio\nplease enter input..: ")));

  ssize_t n = ACE_OS::read (ACE_STDIN,
                            buf,
                            sizeof buf);
  if (n > 0)
    {
      if (ACE_OS::write (ACE_STDOUT,
                         buf,
                         n) != n)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write")),
                          -1);
      return 0;
    }
  else
    return -1;
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::handle_timeout (const ACE_Time_Value &,
                                       const void *)
{
  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Connect timedout. ")));
  return this->close ();
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("in handle_output\n")));

  return (this->*action_) ();
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::handle_signal (int,
                                          siginfo_t *,
                                          ucontext_t *)
{
  // @@ Note that this code is not portable to all OS platforms since
  // it uses print statements within signal handler context.
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("in handle_signal\n")));

  return (this->*action_) ();
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("in handle_input\n")));

  return (this->*action_) ();
}

template <typename PEER_STREAM> int
Peer_Handler<PEER_STREAM>::handle_close (ACE_HANDLE h,
                                     ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("closing down handle %d with mask %d\n"),
              h,
              mask));

  if (this->action_ == &Peer_Handler<PEER_STREAM>::stdio)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("moving to closed state\n")));
      this->reactor ()->end_reactor_event_loop ();
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("moving to stdio state\n")));
      this->action_ = &Peer_Handler<PEER_STREAM>::stdio;
      this->peer ().close ();
      ACE_OS::rewind (stdin);

      if (this->reactor ())
#if defined (ACE_WIN32)
        // On Win32, the std handle must be registered directly (and not
        // as a socket)
        return this->reactor ()->register_handler (this,
                                                   ACE_STDIN);
#else
        // On non-Win32, the std handle must be registered as a normal
        // handle with the READ mask
        return this->reactor ()->register_handler (ACE_STDIN,
                                                   this,
                                                   ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 */
      else
        delete this;
    }

  return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR> int
IPC_Client<SVC_HANDLER, PEER_CONNECTOR>::svc (void)
{
  if (this->reactor ())
    this->reactor ()->run_reactor_event_loop ();

  return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR> int
IPC_Client<SVC_HANDLER, PEER_CONNECTOR>::fini (void)
{
  return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
IPC_Client<SVC_HANDLER, PEER_CONNECTOR>::IPC_Client (void)
  : done_handler_ (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
{
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR> int
IPC_Client<SVC_HANDLER, PEER_CONNECTOR>::init (int argc, ACE_TCHAR *argv[])
{
  // Call down to the CONNECTOR's open() method to do the
  // initialization.
  this->inherited::open (ACE_Reactor::instance ());

  const ACE_TCHAR *r_addr = argc > 1 ? argv[1] :
    ACE_SERVER_ADDRESS (ACE_DEFAULT_SERVER_HOST,
                        ACE_DEFAULT_SERVER_PORT_STR);
  ACE_Time_Value timeout (argc > 2
                          ? ACE_OS::atoi (argv[2])
                          : ACE_DEFAULT_TIMEOUT);

  // Handle signals through the ACE_Reactor.
  if (ACE_Reactor::instance ()->register_handler
      (SIGINT,
       &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                      -1);
  typename PEER_CONNECTOR::PEER_ADDR remote_addr (r_addr);
  this->options_.set (ACE_Synch_Options::USE_REACTOR,
                      timeout);

  SVC_HANDLER *sh;
  ACE_NEW_RETURN (sh,
                  SVC_HANDLER (this->reactor ()),
                  -1);

  // Connect to the peer.
  if (this->connect (sh,
                     remote_addr,
                     this->options_) == -1
      && errno != EWOULDBLOCK)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")),
                      -1);
  else
    return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
IPC_Client<SVC_HANDLER, PEER_CONNECTOR>::~IPC_Client (void)
{
}

#endif /* CPP_CONNECTOR_C */
