// $Id$

/* We try to keep main() very simple.  One of the ways we do that is
   to push much of the complicated stuff into worker objects.  In this
   case, we only need to include the acceptor header in our main
   source file.  We let it worry about the "real work".  */

#include "client_acceptor.h"

/* As before, we create a simple signal handler that will set our
   finished flag.  There are, of course, more elegant ways to handle
   program shutdown requests but that isn't really our focus right
   now, so we'll just do the easiest thing.  */

static sig_atomic_t finished = 0;
extern "C" void handler (int)
{
  finished = 1;
}

/* A server has to listen for clients at a known TCP/IP port.  The
   default ACE port is 10002 (at least on my system) and that's good
   enough for what we want to do here.  Obviously, a more robust
   application would take a command line parameter or read from a
   configuration file or do some other clever thing.  Just like the
   signal handler above, though, that's what we want to focus on, so
   we're taking the easy way out.  */

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

/* Finally, we get to main.  Some C++ compilers will complain loudly
   if your function signature doesn't match the prototype.  Even
   though we're not going to use the parameters, we still have to
   specify them.  */

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG(argc);
  ACE_UNUSED_ARG(argv);

  /* In our earlier servers, we used a global pointer to get to the
    reactor. I've never really liked that idea, so I've moved it into
    main() this time. When we get to the Client_Handler object you'll
    see how we manage to get a pointer back to this reactor.  */
  ACE_Reactor reactor;

  /* The acceptor will take care of letting clients connect to us.  It
    will also arrange for a Client_Handler to be created for each new
    client.  Since we're only going to listen at one TCP/IP port, we
    only need one acceptor.  If we wanted, though, we could create
    several of these and listen at several ports.  (That's what we
    would do if we wanted to rewrite inetd for instance.)  */
  Client_Acceptor peer_acceptor;

  /* Create an ACE_INET_Addr that represents our endpoint of a
    connection. We then open our acceptor object with that Addr.
    Doing so tells the acceptor where to listen for connections.
    Servers generally listen at "well known" addresses.  If not, there
    must be some mechanism by which the client is informed of the
    server's address.

    Note how ACE_ERROR_RETURN is used if we fail to open the acceptor.
    This technique is used over and over again in our tutorials.  */
  if (peer_acceptor.open (ACE_INET_Addr (PORT), &reactor) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  /* Install our signal handler.  You can actually register signal
    handlers with the reactor.  You might do that when the signal
    handler is responsible for performing "real" work.  Our simple
    flag-setter doesn't justify deriving from ACE_Event_Handler and
    providing a callback function though.  */
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  /* Like ACE_ERROR_RETURN, the ACE_DEBUG macro gets used quite a bit.
    It's a handy way to generate uniform debug output from your
    program.  */
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting up server daemon\n"));

  /* This will loop "forever" invoking the handle_events() method of
    our reactor. handle_events() watches for activity on any
    registered handlers and invokes their appropriate callbacks when
    necessary.  Callback-driven programming is a big thing in ACE, you
    should get used to it. If the signal handler catches something,
    the finished flag will be set and we'll exit.  Conveniently
    enough, handle_events() is also interrupted by signals and will
    exit back to the while() loop.  (If you want your event loop to
    not be interrupted by signals, checkout the <i>restart</i> flag on
    the open() method of ACE_Reactor if you're interested.)  */
  while (!finished)
    reactor.handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shutting down server daemon\n"));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Guard<ACE_Mutex>;
template class ACE_Atomic_Op<ACE_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Guard<ACE_Mutex>
#pragma instantiate ACE_Atomic_Op<ACE_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
