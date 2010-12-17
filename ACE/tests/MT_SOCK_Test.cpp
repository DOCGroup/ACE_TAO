/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MT_SOCK_Test.cpp
//
// = DESCRIPTION
//     This is a multi-threaded torture test of the
//     <ACE_SOCK_Acceptor> and <ACE_SOCK_Connector> classes. The test
//     forks 30 processes or spawns 30 threads (depending upon the
//     platform) and then executes client and server allowing them to
//     connect and exchange data.  Note that most of the connections
//     will fail since we're overrunning the size of the listen queue
//     for the acceptor-mode socket.
//
// = AUTHOR
//    Doug Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Time_Value.h"

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// Normally the test will have BACKLOG < NUM_CLIENTS to force some
// of the connections to fail.
// Do NOT use ACE_DEFAULT_BACKLOG here, because that will likely
// be set to some other value. (i.e. Win32 = SOMAXCONN)
static const int BACKLOG = 5;
static const int NUM_CLIENTS = 30;

#if !defined (ACE_LACKS_FORK) || defined (ACE_HAS_THREADS)

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
                             ACE_DEFAULT_SERVER_HOST);
  ACE_INET_Addr client_addr;
  ACE_SOCK_Stream cli_stream;
  ACE_SOCK_Connector con;
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Time_Value *timeout = &tv;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) client: Connecting...\n")));
  // Initiate timed connection with server.

  // Attempt a timed connect to the server.
  if (con.connect (cli_stream,
                   server_addr,
                   timeout) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("client: Connection timed out.")));
      return 0;
    }

  if (cli_stream.get_local_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("client: get_local_addr")),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client: Connected at %d\n"),
              client_addr.get_port_number ()));

  if (cli_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("client: disable")));

  // Send data to server (correctly handles "incomplete writes").

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: Sending data...\n"));

  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (cli_stream.send_n (c, 1) == -1)
      {
        // This is, I believe, more of an issue with WinXP-64 _server_
        // side, but we can trap it here since we know we're connecting
        // to localhost. Some Windows versions will appear to accept
        // connections at the TCP level past the listen backlog but if
        // data arrives before the actual application-level accept() occurs,
        // the connection is reset. This is caused when we trip the Windows
        // SYN attack prevention (http://technet2.microsoft.com/WindowsServer/
        // en/library/910c8482-e5e5-4e2c-9ea4-11301ddfc4661033.mspx?mfr=true)
        // So, if we get a reset on the first send, don't flag the error -
        // just note it and act like the connection was refused.
        if (c == ACE_ALPHABET && errno == ECONNRESET) // First byte sent
          {
            ACE_DEBUG
              ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) client: Connection refused (delayed)\n")));
            cli_stream.close ();
            return 0;
          }

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) (errno %d) %p\n"), ACE_ERRNO_GET,
                    ACE_TEXT ("client: send_n")));
        ACE_ERROR ((LM_ERROR, "client: Closing stream.\n"));
        cli_stream.close();
        return 0;
      }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client: Closing writer...\n")));

  // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("client: close_writer")));
  char buf[1];

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client: Waiting for server handshake...\n")));

  // Wait for handshake with server.
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
    ACE_TEXT ("client: recv_n")));

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) client: Handshake received. Closing stream.\n")));

  // Close the connection completely.
  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("client: close")));
  return 0;
}

static void *
server (void *arg)
{
  ACE_SOCK_Acceptor *peer_acceptor =
    static_cast<ACE_SOCK_Acceptor *> (arg);

  if (peer_acceptor->enable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("server: enable acceptor")));

  // Keep these objects out here to prevent excessive constructor
  // calls...
  ACE_SOCK_Stream new_stream;
  ACE_INET_Addr cli_addr;
  const ACE_Time_Value def_timeout (ACE_DEFAULT_TIMEOUT);
  ACE_Time_Value tv (def_timeout);

  // We want some of the clients to get connection failures, but on
  // a really fast machine with a good network card and multiple
  // processors this may never happen.
  // Add a sleep() to allow the client threads to complete.
  ACE_OS::sleep(def_timeout);

  int num_clients_connected = 0;

  // Performs the iterative server activities.
  for (;;)
    {
      char buf[BUFSIZ];

      ACE_DEBUG((LM_DEBUG, "(%P|%t) server: Waiting for connection...\n"));

      int result = ACE::handle_read_ready (peer_acceptor->get_handle (), &tv);
      ACE_TEST_ASSERT (tv == def_timeout);

      if (result == -1)
        {
          if (errno == ETIME)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) server: Test finished.\n")));
              // The meaning of the backlog parameter for listen() varies by
              // platform. For some reason lost to history, the specified value
              // is typically backlog * 1.5, backlog * 1.5 + 1, or event taken
              // literally as on Windows. We'll accept any number less than
              // backlog * 2 as valid.
              if (num_clients_connected > BACKLOG * 2)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) server: Incorrect # client ")
                            ACE_TEXT ("connections. Expected:%d-%d Actual:%d\n"),
                            BACKLOG, BACKLOG * 2, num_clients_connected));
              return 0;
            }

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) %p\n"),
                             ACE_TEXT ("server: handle_read_ready acceptor")),
                            0);
        }

      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR).

      while ((result = peer_acceptor->accept (new_stream,
                                              &cli_addr)) != -1)
        {
          const char *t = ACE_ALPHABET;

          ++num_clients_connected;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server: Client %C connected from %d\n"),
                      cli_addr.get_host_name (),
                      cli_addr.get_port_number ()));

          // Enable non-blocking I/O.
          if (new_stream.enable (ACE_NONBLOCK) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("server: enable non blocking i/o")),
                              0);
          // Read data from client (terminate on error).
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server: Waiting for data...\n")));

          for (ssize_t r_bytes; ;)
            {
              if (ACE::handle_read_ready (new_stream.get_handle (), 0) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("(%P|%t) %p\n"),
                                   ACE_TEXT ("stream handle_read_ready")),
                                  0);

              ACE_DEBUG ((LM_DEBUG, "(%P|%t) server: Receiving data...\n"));

              while ((r_bytes = new_stream.recv (buf, 1)) > 0)
                {
                  ACE_TEST_ASSERT (*t == buf[0]);
                  t++;
                }

              ACE_DEBUG((LM_DEBUG, "(%P|%t) server: Received data.\n"));

              if (r_bytes == 0)
                {
                  // Handshake back with client.
                  ACE_DEBUG
                    ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server: Connection closed by client.\n")));

                  ACE_DEBUG
                    ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server: Sending handshake.\n")));

                  if (new_stream.send_n ("", 1) != 1)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) %p\n"),
                                ACE_TEXT ("server: send_n")));

                  ACE_DEBUG ((LM_DEBUG, "(%P|%t) server: Closing stream.\n"));

                  // Close endpoint.
                  if (new_stream.close () == -1)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) %p\n"),
                                ACE_TEXT ("server: close")));
                  break;
                }
              else if (r_bytes == -1)
                {
                  if (errno == EWOULDBLOCK || errno == EAGAIN)
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) server: (EWOULDBLOCK) Waiting for more data...\n")));
                  else
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ("(%P|%t) %p\n"),
                                       ACE_TEXT ("server: recv_n")),
                                      0);
                }
            }
        }
      if (result == -1)
        {
          if (errno == EWOULDBLOCK)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) server: No more connections pending.\n")));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("server: accept")));
        }
    }
  ACE_NOTREACHED (return 0);
}

#endif /* !ACE_LACKS_FORK || ACE_HAS_THREADS */

static void
spawn (int num_clients)
{
  // Acceptor
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr;

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (ACE_Addr::sap_any, 0, PF_UNSPEC, BACKLOG) == -1
      || peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("spawn: open")));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
    for (int i = 0; i < num_clients; i++)
        {
          switch (ACE_OS::fork (ACE_TEXT ("child")))
            {
            case -1:
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p\n"), "spawn: fork failed"));
              i = num_clients;
              // Break out of 'for' loop.
              break;
            case 0:
              client (&server_addr);
              ACE_OS::exit (0);
              /* NOTREACHED */
            default:
              break;
            }
        }

      server ((void *) &peer_acceptor);

      peer_acceptor.close();

      // Reap the child pids.
      for (pid_t pid; (pid = ACE_OS::wait ()) != -1; )
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) spawn: reaping pid %d\n"), pid));

#elif defined (ACE_HAS_THREADS)

      ACE_DEBUG((LM_DEBUG, "Spawning server...\n"));

      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (server),
           (void *) &peer_acceptor,
           THR_BOUND | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n%a"),
                    ACE_TEXT ("spawn: failed"),
                    1));

      ACE_DEBUG((LM_DEBUG, "Spawning %d clients...\n", num_clients));

      if (ACE_Thread_Manager::instance ()->spawn_n
          (num_clients,
           ACE_THR_FUNC (client),
           (void *) &server_addr,
           THR_BOUND | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n%a"),
                    ACE_TEXT ("spawn: failed 2"),
                    1));

      ACE_DEBUG((LM_DEBUG, "Waiting for threads to finish...\n"));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
#else
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("(%P|%t) ")
                  ACE_TEXT ("spawn: only one thread may be run")
                  ACE_TEXT (" in a process on this platform\n")));
#endif /* !ACE_LACKS_FORK */

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Threads complete. Closing Acceptor.\n")));

      peer_acceptor.close ();
    }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MT_SOCK_Test"));

  spawn (NUM_CLIENTS);

  ACE_END_TEST;
  return 0;
}
