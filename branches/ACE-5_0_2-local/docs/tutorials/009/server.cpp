// $Id$

/* The actual datagram operations here are exactly the same as those
   used in the previous tutorial.  What we've added is some logic that
   will prevent this server from responding to just any old datagram.
   I'll limit my comments to those pieces of code.  */

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

/* In order to be more selective, our server will be started with a
   "signature".  If none is given, we'll use the one here instead.  */
static const char *default_signature = "Hello World!";

int 
main (int argc, char *argv[])
{
  ACE_INET_Addr local (PORT);
  ACE_SOCK_Dgram dgram;

  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  char buf[BUFSIZ];
  ACE_INET_Addr remote;

  while (dgram.recv (buf,
                     sizeof (buf),
                     remote) != -1)
    {
      /* What did the client say?  */
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Received (%s) from (%s)\n",
                  buf,
                  remote.get_host_name ()));

      /* Use a simple string-op to decide if the client is one of our
        own.  Of course, you could have sent numeric values or even a
        struct of data. For this simple exercise, however, strings are
        just fine.  */
      if (ACE_OS::strcmp (buf,
                          argc > 1 ? argv[1] : default_signature))
        {
          /* If the client didn't say something we like then log it
            *and move on.  /
          ACE_DEBUG ((LM_DEBUG,
		      "(%P|%t) Client query does not match our signature (%s).  Response not sent.\n",
		      argc > 1 ? argv[1] : default_signature));
        }
      else
        {
          /* As before, we respond to the client's query.  */

          ACE_INET_Addr local ((u_short) 0);
          ACE_SOCK_Dgram peer;
          if (peer.open (local) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "response open"),
                              -1);
          sprintf (buf,
                   "I am here");
          if (peer.send (buf,
                         ACE_OS::strlen (buf) + 1,
                         remote) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "response send"),
                              -1);
        }
    }

  return 0;
}
