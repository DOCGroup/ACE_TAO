// $Id$

#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/INET_Addr.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int
main (int argc, char *argv[])
{
  ACE_INET_Addr local ((u_short) 0);
  ACE_INET_Addr remote (PORT, INADDR_BROADCAST);
  ACE_SOCK_Dgram_Bcast dgram;

  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  char buf[BUFSIZ];

  sprintf (buf,
           argc > 1 ? argv[1] : "Hello World!");
  if (dgram.send (buf,
                  ACE_OS::strlen (buf) + 1,
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);
  ACE_Time_Value timeout (2, 0);
  if (dgram.recv (buf,
                  sizeof (buf),
                  remote,
                  0,
                  &timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) The server at (%s) said (%s)\n",
	      remote.get_host_name (),
              buf));
  return 0;
}
