// $Id$

/* To establish a socket connection to a server, we'll need an
  ACE_SOCK_Connector.  */
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"

/* Unlike the previous two tutorials, we're going to allow the user to
  provide command line options this time.  Still, we need defaults in
  case that isn't done.  */
static u_short SERVER_PORT = ACE_DEFAULT_SERVER_PORT;
static const char *const SERVER_HOST = ACE_DEFAULT_SERVER_HOST;
static const int MAX_ITERATIONS = 4;

int
main (int argc, char *argv[])
{
  /* Accept the users's choice of hosts or use the default.  Then do
    the same for the TCP/IP port at which the server is listening as
    well as the number of iterations to perform.  */
  const char *server_host = argc > 1 ? argv[1] : SERVER_HOST;
  u_short server_port = argc > 2 ? ACE_OS::atoi (argv[2]) : SERVER_PORT;
  int max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : MAX_ITERATIONS;

  /* Build ourselves a Stream socket. This is a connected socket that
    provides reliable end-to-end communications. We will use the
    server object to send data to the server we connect to.  */
  ACE_SOCK_Stream server;

  /* And we need a connector object to establish that connection. The
    ACE_SOCK_Connector object provides all of the tools we need to
    establish a connection once we know the server's network
    address...  */
  ACE_SOCK_Connector connector;

  /* Which we create with an ACE_INET_Addr object. This object is
    given the TCP/IP port and hostname of the server we want to
    connect to.  */
  ACE_INET_Addr addr (server_port,
                      server_host);

  /* So, we feed the Addr object and the Stream object to the
    connector's connect() member function. Given this information, it
    will establish the network connection to the server and attach
    that connection to the server object.  */
  if (connector.connect (server, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  /* Just for grins, we'll send the server several messages.  */
  for (int i = 0; i < max_iterations; i++)
    {
      char buf[BUFSIZ];

      /* Create our message with the message number */
      ACE_OS::sprintf (buf,
                       "message = %d\n",
                       i + 1);
      /* Send the message to the server.  We use the server object's
        send_n() function to send all of the data at once. There is
        also a send() function but it may not send all of the
        data. That is due to network buffer availability and such. If
        the send() doesn't send all of the data, it is up to you to
        program things such that it will keep trying until all of the
        data is sent or simply give up. The send_n() function already
        does the "keep trying" option for us, so we use it.

        Like the send() method used in the servers we've seen, there
        are two additional parameters you can use on the send() and
        send_n() method calls.  The timeout parameter limits the
        amount of time the system will attempt to send the data to the
        peer.  The flags parameter is passed directly to the OS send()
        system call.  See send(2) for the valid flags values.  */
      if (server.send_n (buf,
                         ACE_OS::strlen (buf)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send"),
                          -1);
      else
        /* Pause for a second.  */
        ACE_OS::sleep (1);
    }

  /* Close the connection to the server.  The servers we've created so
    far all are based on the ACE_Reactor.  When we close(), the
    server's reactor will see activity for the registered event
    handler and invoke handle_input().  That, in turn, will try to
    read from the socket but get back zero bytes.  At that point, the
    server will know that we've closed from our side.  */
  if (server.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      -1);
  return 0;
}
