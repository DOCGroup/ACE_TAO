// $Id$

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

/* Once again, we use the default server port.  In a "real" system,
   the server's port (or ports) would be published in some way so that
   clients would know where to "look".  We could even add entries to
   the operating system's services file and use a service name instead
   of a number.  We'll come back to that in some other tutorial
   though.  For now, let's stay simple.  */
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

/* Our goal here is to develop a client that can send a datagram to a
   server running on a known host.  We'll use a command-line argument
   to specify the hostname instead of hard-coding it.  */
int 
main (int argc,char *argv[])
{
  /* All datagrams must have a point of origin.  Since we intend to
    transmit instead of receive, we initialize an address with zero
    and let the OS choose a port for us.  We could have chosen our own
    value between 1025 and 65535 as long as it isn't already in use.

    The biggest difference between client and server when datagrams
    are used is the fact that servers tend to have a known/fixed
    address at which they listen and clients tend to have arbitrary
    addresses assigned by the OS.  */
  ACE_INET_Addr local((u_short) 0);

  /* And here is our datagram object.  */
  ACE_SOCK_Dgram dgram;
	
  /* Notice that this looks a lot like the server application.
    There's no difference in creating server datagrams an client
    datagrams.  You can even use a zero-constructed address for your
    server datagram as long as you tell the client where you're
    listening (eg -- by writting into a file or some such).  */
  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "datagram open"),
                      -1);

  /* Yep.  We've seen this before too...  */
  char buf[BUFSIZ];

  /* Ok, now we're doing something different.  */
  sprintf (buf, "Hello World!");

  /* Just like sending a telegram, we have to address our datagram.
    Here, we create an address object at the desired port on the
    chosen host.  To keep us from crashing, we'll provide a default
    host name if we aren't given one.  */
  ACE_INET_Addr remote (PORT,
                        argc > 1 ? argv[1] : "localhost");

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Sending (%s) to the server.\n",
              buf));
  /* Now we send our buffer of stuff to the remote address.  This is
    just exactly what the server did after receiving a client message.
    Datagrams are rather orthogonal that way: they don't generally
    make much of a fuss about being either client or server.  */
  if (dgram.send (buf,
                  ACE_OS::strlen (buf) + 1,
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  /* Now we've turned around and put ourselves into "server mode" by
    invoking the recv() method.  We know our server is going to send
    us something, so we hang out here and wait for it.  Because we
    know datagrams are unreliable, there is a chance that the server
    will respond but we won't hear.  You might consider providing a
    timeout on the recv() in that case.  If recv() fails due to
    timeout it will return -1 and you can then resend your query and
    attempt the recv() again.

    Like the server application, we have to give the recv() an
    uninitialized addr object so that we can find out who is talking
    back to us.  */
  if (dgram.recv (buf,
                  sizeof (buf),
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      -1);

  /* Find out what the server had to say.  */
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) The server said:  %s\n",
              buf));

  /* Using the "remote" object instance, find out where the server
    lives.  We could then save this address and use directed datagrams
    to chat with the server for a while.  */
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) The server can be found at:  (%s:%d)\n",
              remote.get_host_name(),
              PORT));
  
  return 0;
}
