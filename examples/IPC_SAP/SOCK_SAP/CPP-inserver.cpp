// $Id$

// This example tests the features of the ACE_SOCK_Acceptor and
// ACE_SOCK_Stream classes when used with a thread-per-request
// concurrency model.

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Handle_Set.h"
#include "ace/Thread_Manager.h"

// Are we running verbosely?
static int verbose = 0;

// ACE SOCK_SAP server.

// Entry point into the server task.

static void *
server (void *arg)
{
  ACE_SOCK_Stream new_stream;
  ACE_HANDLE handle = (ACE_HANDLE) (long) arg;

  new_stream.set_handle (handle);

  // Read data from client (terminate on error).

  for (;;)
    {
      char buf[BUFSIZ];

      ssize_t r_bytes = new_stream.recv (buf, sizeof buf, 0);

      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (verbose && ACE::write_n (ACE_STDOUT, buf, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR, "%p\n", "ACE::write_n"));
      else if (new_stream.send_n (buf, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
    }

  // Close new endpoint (listening endpoint stays open).
  if (new_stream.close () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "close"));

  return 0;
}

int
main (int argc, char *argv[])
{
  u_short port = argc > 1
    ? ACE_OS::atoi (argv[1])
    : ACE_DEFAULT_SERVER_PORT;

  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr (port);

  // Create a server, reuse the address.
  if (peer_acceptor.open (server_addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      1);
  else if (peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_local_addr"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting server at port %d\n",
              server_addr.get_port_number ()));

  // Keep these objects out here to prevent excessive constructor
  // calls within the loop.
  ACE_SOCK_Stream new_stream;
  ACE_INET_Addr cli_addr;

  // Performs the iterative server activities.

  while (peer_acceptor.accept (new_stream, &cli_addr) != -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client %s connected from %d\n",
                  cli_addr.get_host_name (),
                  cli_addr.get_port_number ()));

#if defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) server,
                                                  (void *) new_stream.get_handle ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p\n",
                           "spawn"),
                          1);
#else
      server ((void *) new_stream.get_handle ());
#endif /* ACE_HAS_THREADS */
    }

  return 0;
}
