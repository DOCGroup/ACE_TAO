// $Id$

/* Include the header file where our client acceptor is defined.  */
#include "ace/Reactor.h"

/* For simplicity, we create our reactor in the global address space.
  In later tutorials we will do something more clever and
  appropriate. However, the purpose of this tutorial is to introduce a
  connection acceptance and handling, not the full capabilities of a
  reactor.  */
ACE_Reactor *g_reactor;

/* Include the header where we define our acceptor object.  An
  acceptor is an abstraction that allows a server to "accept"
  connections from clients.  */
#include "acceptor.h"

/* A TCP/IP server can listen to only one port for connection
  requests.  Well-known services can always be found at the same
  address. Lesser-known services are generally told where to listen by
  a configuration file or command-line parameter. For this example,
  we're satisfied with simply hard-coding a random but known value.  */
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int 
main (int, char *[])
{
  /* Create a Reactor instance.  Again, a global pointer isn't exactly
    the best way to handle this but for the simple example here, it
    will be OK.  We'll get cute with it later.  Note how we use the
    ACE_NEW_RETURN macro, which returns 1 if operator new fails. */
  ACE_NEW_RETURN (g_reactor,
                  ACE_Reactor,
                  1);

  /* Like the Reactor, I'm skimming over the details of the ADDR
    object. What it provides is an abstraction for addressing services
    in the network. All we need to know at this point is that we are
    creating an address object which specifies the TCP/IP port on
    which the server will listen for new connection requests.  */
  ACE_INET_Addr addr (PORT);

  Logging_Acceptor *peer_acceptor;

  /* We now create an acceptor object.  No connections will yet be
    established because the object isn't "open for business" at this
    time. Which brings us to the next line...  */
  ACE_NEW_RETURN (peer_acceptor,
                  Logging_Acceptor,
                  1);

  /* where the acceptor object is opened.  You'll find that most ACE
    objects have to be open()ed before they're of any use to you.  On
    this open() call, we're telling the acceptor where to listen for
    connections via the 'addr' object.  We're also telling it that we
    want it to be registered with our 'g_reactor' instance.  */
  if (peer_acceptor->open (addr, g_reactor) == -1 )
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "Opening Acceptor\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting up server logging daemon\n"));

  /* The reactor's handle_events member function is responsible for
    looking at all registered objects and invoking an appropriate
    member function when anything of interest occurs. When an event is
    processed, the handle_events function returns. In order to get all
    events, we embed this in an infinite loop.

    Since we put ourselves into an infinite loop, you'll need to
    CTRL-C to exit the program.  */

  for (;;)
    g_reactor->handle_events ();

  return 0;
}

