// $Id$

#include "ace/SOCK_Connector.h"

static u_short SERVER_PORT = ACE_DEFAULT_SERVER_PORT;
static const char *const SERVER_HOST = ACE_DEFAULT_SERVER_HOST;
static const int MAX_ITERATIONS = 4;

int main (int argc, char *argv[])
{
  const char *server_host = argc > 1 ? argv[1]                : SERVER_HOST;
  u_short server_port     = argc > 2 ? ACE_OS::atoi (argv[2]) : SERVER_PORT;
  int max_iterations      = argc > 3 ? ACE_OS::atoi (argv[3]) : MAX_ITERATIONS;

  ACE_SOCK_Stream server;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (server_port, server_host);

  if (connector.connect (server, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  
  for (int i = 0; i < max_iterations; i++)
    {
      char buf[BUFSIZ];

      ::sprintf (buf, "message = %d\n", i + 1);

      if (server.send_n ( buf, strlen(buf) ) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
      else
        ACE_OS::sleep (1);
    }

  if (server.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
