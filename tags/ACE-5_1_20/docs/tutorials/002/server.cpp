// $Id$

/* As before, we need a few ACE objects as well as our Logging_Handler
  declaration.  */
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "handler.h"

/* We'll still use the global reactor pointer.  There's a snappy way
   around this that shows up in later server tutorials.  */
ACE_Reactor *g_reactor;

/* This was hinted at in Tutorial 1.  Remember the hand-coded acceptor
  that we created there?  This template does all of that and more and
  better.  If you find yourself creating code that doesn't feel like a
  part of your application, there's a good chance that ACE has a
  template or framework component to do it for you.  */
typedef ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR> Logging_Acceptor;

/* One of the new things will be a signal handler so that we can exit
  the application somewhat cleanly.  The 'finished' flag is used
  instead of the previous infninite loop and the 'handler' will set
  that flag in respose to SIGINT (CTRL-C).
  The invocation of ACE_Reactor::notify() will cause the
  handle_events() to return so that we can see the new value of 'finished'.
*/
static sig_atomic_t finished = 0;
extern "C" void handler (int)
{
  finished = 1;
  g_reactor->notify();
}

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int
main (int, char **)
{
  // Create the reactor we'll register our event handler derivatives with.
  ACE_NEW_RETURN (g_reactor,
                  ACE_Reactor,
                  1);

  // Create the acceptor that will listen for client connetions
  Logging_Acceptor peer_acceptor;

  /* Notice how similar this is to the open() call in Tutorial 1.  I
    read ahead when I created that one so that it would come out this
    way...  */
  if (peer_acceptor.open (ACE_INET_Addr (PORT),
                          g_reactor) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  /* Here's the easiest way to respond to signals in your application.
    Simply construct an ACE_Sig_Action object with a "C" function and
    the signal you want to capture.  As you might expect, there is
    also a way to register signal handlers with a reactor but we take
    the easy-out here.  */
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting up server logging daemon\n"));

  // Perform logging service until the signal handler receives SIGINT.
  while (!finished)
    g_reactor->handle_events ();

  // Close the acceptor so that no more clients will be taken in.
  peer_acceptor.close();

  // Free up the memory allocated for the reactor.
  delete g_reactor;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shutting down server logging daemon\n"));
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

