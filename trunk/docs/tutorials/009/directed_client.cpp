// $Id$

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int 
main (int argc, char *argv[])
{
  ACE_INET_Addr local ((u_short) 0);
  ACE_INET_Addr remote (PORT,
                        argc > 1 ? argv[1] : "localhost");
  ACE_SOCK_Dgram dgram;

  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  char buf[BUFSIZ];

  /* In order to conform to the "protocol" required by the server, we
    allow the user to specify a signature.  A default matching the
    server's default is also available.  */
  sprintf (buf,
           argc > 2 ? argv[2] : "Hello World!");

  if (dgram.send (buf,
                  ACE_OS::strlen (buf) + 1,
                  remote) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  /* Because we may have sent a signature that the server doesn't
    honor, we have to have some way to get out of the recv().  Most
    ACE objects that have potential for infinite blocking give you the
    option of providing a timeout.  recv() is no exception.  Here, we
    construct an ACE_Time_Value representing two seconds and no
    micro-seconds.  If recv() fails to get a response within the two
    seconds, it will return -1.  */
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

  /* Note: The fourth parameter to recv() is for flags.  These flags
    are passed directly to the underlying recv() or recvfrom() system
    call.  For Linux, resonable values are: MSG_OOB process
    out-of-band data MSG_PEEK peek at incoming message (but leave it
    in the OS buffers) MSG_WAITALL wait for full request or error See
    your system documentation for the gory details.  */

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) The server said (%s)\n",
              buf));

  return 0;
}
