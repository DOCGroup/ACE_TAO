// $Id$

#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/INET_Addr.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int
main (int argc,char *argv[])
{
  ACE_UNUSED_ARG(argc);
  ACE_UNUSED_ARG(argv);

  ACE_INET_Addr local ((u_short) 0);

  /* Instead of creating the ACE_SOCK_Dgram we created last time,
    we'll create an ACE_SOCK_Dgram_Bcast.  "Bcast" means, of course,
    "Broadcast".  This ACE object is clever enough to go out to the OS
    and find all of the network interfaces.  When you send() on a
    Dgram_Bcast, it will send the datagram out on all of those
    interfaces.  This is quiet handy if you do it on a multi-homed
    host that plays router...  */
  ACE_SOCK_Dgram_Bcast dgram;

  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "datagram open"),
                      -1);

  char buf[BUFSIZ];

  sprintf (buf, "Hello World!");

  /* The only other difference between us and the directed client is
    that we don't specify a host to receive the datagram.  Instead, we
    use the magic value "INADDR_BROADCAST".  All hosts are obliged to
    respond to datagrams directed to this address the same as they
    would to datagrams sent to their hostname.

    Remember, the Dgram_Bcast will send a datagram to all interfaces
    on the host.  That's true even if the address is for a specific
    host (and the host address makes sense for the interface).  The
    real power is in using an INADDR_BROADCAST addressed datagram
    against all interfaces.  */

  ACE_INET_Addr remote (PORT,
                        INADDR_BROADCAST);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Sending (%s) to the server.\n",
              buf));

  if (dgram.send (buf,
                  ACE_OS::strlen (buf) + 1,
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  if (dgram.recv (buf,
                  sizeof (buf),
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      -1);

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
