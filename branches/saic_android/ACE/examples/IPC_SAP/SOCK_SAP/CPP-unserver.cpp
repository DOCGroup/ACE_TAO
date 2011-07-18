// $Id$

// This example tests the features of the ACE_LSOCK_Acceptor and
// ACE_LSOCK_Stream classes.  If the platform supports threads it uses
// a thread-per-request concurrency model.

#include "ace/LSOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"



#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// Are we running verbosely?
static int verbose = 1;

// Entry point into the server task.

static void *
server (void *arg)
{
  ACE_UNIX_Addr cli_addr;
  ACE_LSOCK_Stream new_stream;
  ACE_HANDLE handle = (ACE_HANDLE) (long) arg;

  new_stream.set_handle (handle);

  // Make sure we're not in non-blocking mode.
  if (new_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("disable")),
                       0);

  if (new_stream.get_remote_addr (cli_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("get_remote_addr")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client connected from %C\n"),
              cli_addr.get_path_name ()));

  // Read data from client (terminate on error).

  for (;;)
    {
      char buf[BUFSIZ];

      ssize_t r_bytes = new_stream.recv (buf, sizeof buf);

      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("recv")));
          break;

        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) reached end of input, connection closed by client\n")));
          break;
        }
      else if (verbose && ACE::write_n (ACE_STDOUT, buf, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("ACE::write_n")));
      else if (new_stream.send_n (buf, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("send_n")));
    }

  // Close new endpoint (listening endpoint stays open).
  if (new_stream.close () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("close")));

  return 0;
}

static int
run_event_loop (const ACE_TCHAR rendezvous[])
{
  ACE_LSOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_UNIX_Addr server_addr (rendezvous);

  ACE_OS::unlink (rendezvous);

  // Create a server.

  if (peer_acceptor.open (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      1);
  else if (peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("get_local_addr")),
                       -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("starting server %C\n"),
              server_addr.get_path_name ()));

  // Keep these guys out here to prevent excessive constructor
  // calls...
  ACE_LSOCK_Stream new_stream;

  // Performs the iterative server activities.

  for (;;)
    {
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

      if (peer_acceptor.accept (new_stream, 0, &timeout) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("accept")));
          continue;
        }

#if defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) server,
                                                  reinterpret_cast<void *> (new_stream.get_handle ()),
                                                  THR_DETACHED) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("spawn")),
                          1);
#else
      server (reinterpret_cast<void *> (new_stream.get_handle ()));
#endif /* ACE_HAS_THREADS */
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_event_loop (argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS);
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "this platform does not support UNIX-domain sockets\n"), -1);
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
