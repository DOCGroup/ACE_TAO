// $Id$

// This example tests the features of the ACE_SOCK_Acceptor and
// ACE_SOCK_Stream classes.  If the platform supports threads it uses
// a thread-per-connection concurrency model.

#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"

// Are we running verbosely?
static int verbose = 1;

// Entry point into the server task.

static void *
server (void *arg)
{
  ACE_INET_Addr cli_addr;
  ACE_SOCK_Stream new_stream;
  ACE_HANDLE handle = (ACE_HANDLE) (long) arg;

  new_stream.set_handle (handle);

  // Make sure we're not in non-blocking mode.
  if (new_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "disable"),
                       0);
                       
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client %s connected from %d\n",
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

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

static int
run_event_loop (u_short port)
{
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

  // Performs the iterative server activities.

  for (;;)
    {
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

      if (peer_acceptor.accept (new_stream, 0, &timeout) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
	  continue;
	}          

#if defined (ACE_HAS_THREADS)
      // Spawn a new thread and run the new connection in that thread of
      // control using the <server> function as the entry point.
      if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) server,
                                                  (void *) new_stream.get_handle (),
                                                  THR_DETACHED) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p\n",
                           "spawn"),
                          1);
#else
      server ((void *) new_stream.get_handle ());
#endif /* ACE_HAS_THREADS */
    }

  /* NOTREACHED */
}

int
main (int argc, char *argv[])
{
  return run_event_loop (argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT);
}
