// $Id$

// ============================================================================
//
// = FILENAME
//    Connection_Handler.cpp
//
// = DESCRIPTION
//      This test illustrates how to use the Acceptor pattern to
//      create multiple threads, each running its own Reactor.  You
//      can connect to this via telnet and keep typing until you enter
//      '^D'.
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Service_Config.h"
#include "ace/Thread.h"
#include "ace/Sig_Adapter.h"

#include "Connection_Handler.h"



int
Connection_Handler::open (void *)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) in open()\n")));
  //FUZZ: enable check_for_lack_ACE_OS

  // Make ourselves an Active Object.
  return this->activate (THR_NEW_LWP | THR_DETACHED);
}

int
Connection_Handler::close (u_long)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) in close()\n")));
  //FUZZ: enable check_for_lack_ACE_OS

  // Shut ourself down.  Note that this doesn't destroy the thread,
  // just the state of the object.
  this->destroy ();
  return 0;
}

int
Connection_Handler::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) in svc()\n")));

  this->finished_ = 0;

  // Create our own personal Reactor just for this thread.  Note that
  // we create this on the stack of the thread since it's only used
  // for the duration of this connection!

  ACE_Reactor reactor;

  // Each <ACE_Svc_Handler> has its own <ACE_Reactor *>.  By default,
  // this points to the <Acceptor's> Reactor.  However, we can point
  // it to our local Reactor, which is what we do in this case.
  this->reactor (&reactor);

  // Register ourselves to handle input in this thread without
  // blocking.
  if (this->reactor ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("can' (%P|%t) t register with reactor\n")),
                      -1);
  // Schedule a timer.
  else if (this->reactor ()->schedule_timer (this,
                                             (const void *) this,
                                             ACE_Time_Value (2),
                                             ACE_Time_Value (2)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) connected with client\n")));

  // Keep looping until we receive SIGQUIT or the client shutsdown.

  while (this->finished_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) handling events\n")));
      this->reactor ()->handle_events ();
    }

  // Cancel all pending timeouts.
  this->reactor ()->cancel_timer (this);

  // Remove ourselves from the Reactor.
  this->reactor ()->remove_handler
    (this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

  // Zero-out the Reactor field so it isn't accessed later on.
  this->reactor (0);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) exiting svc\n")));
  return 0;
}

int
Connection_Handler::handle_close (ACE_HANDLE,
                                  ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) in handle_close\n")));

  // Signal the svc() event loop to shut down.
  this->finished_ = 1;
  return 0;
}

int
Connection_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) handle_input\n")));

  switch (this->peer ().recv (buf, sizeof buf))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) %p bad read\n"),
                         ACE_TEXT ("client logger")),
                        -1);
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) closing log daemon (fd = %d)\n"),
                         this->get_handle ()),
                        -1);
    default:
      if (buf[0] == (char) EOF)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) closing log daemon (fd = %d)\n"),
                           this->get_handle ()),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) from client: %s"), buf));
    }

  return 0;
}

int
Connection_Handler::handle_signal (int signum,
                                   siginfo_t *,
                                   ucontext_t *)
{
  // @@ Note that this code is not portable to all OS platforms since
  // it uses print statements within signal handler context.

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("received signal %S\n"), signum));

  this->finished_ = 1;
  return 0;
}

int
Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                    const void *arg)
{
#if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (arg);
#endif /* ACE_NDEBUG */

  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) handling timeout from this = %@\n"),
              this));
  return 0;
}

// Define an Acceptor for the <Connection_Handler>.

typedef ACE_Acceptor <Connection_Handler, ACE_SOCK_ACCEPTOR>
        Connection_Acceptor;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) in main\n")));

  // Acceptor factory.
  Connection_Acceptor peer_acceptor;

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  // Register the signal handler adapter.
  if (ACE_Reactor::instance ()->register_handler (SIGINT, &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                      -1);

  // Open the Acceptor.
  else if (peer_acceptor.open (ACE_INET_Addr (port),
                               ACE_Reactor::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting up connection server\n")));

  // Perform connection service until we receive SIGINT.

  while (ACE_Reactor::event_loop_done() == 0)
    ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) shutting down connection server\n")));
  return 0;
}

