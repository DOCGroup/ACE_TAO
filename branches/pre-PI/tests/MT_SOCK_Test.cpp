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
#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"

ACE_RCSID(tests, MT_SOCK_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

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
#if defined (ACE_HAS_BROKEN_NON_BLOCKING_CONNECTS)
  ACE_Time_Value *timeout = 0;
#else
  ACE_Time_Value *timeout = &tv;
#endif /* ACE_HAS_BROKEN_NON_BLOCKING_CONNECTS */

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) starting timed connect\n")));
  // Initiate timed connection with server.

  // Attempt a timed connect to the server.
  if (con.connect (cli_stream,
                   server_addr,
                   timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("connection failed")),
                      0);

  if (cli_stream.get_local_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("get_local_addr")),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) connected client at %d\n"),
              client_addr.get_port_number ()));

  if (cli_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("disable")));

  // Send data to server (correctly handles "incomplete writes").

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (cli_stream.send_n (c, 1) == -1)
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("send_n")));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) closing writer\n")));

  // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("close_writer")));
  char buf[1];

  // Wait for handshake with server.
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("recv_n")));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) received handshake from server\n")));

  // Close the connection completely.
  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("close")));
  return 0;
}

static void *
server (void *arg)
{
  ACE_SOCK_Acceptor *peer_acceptor =
    ACE_static_cast (ACE_SOCK_Acceptor *, arg);

  if (peer_acceptor->enable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("enable")));

  // Keep these objects out here to prevent excessive constructor
  // calls...
  ACE_SOCK_Stream new_stream;
  ACE_INET_Addr cli_addr;
  ACE_Handle_Set handle_set;
  const ACE_Time_Value def_timeout (ACE_DEFAULT_TIMEOUT);
  ACE_Time_Value tv (def_timeout);

  // Performs the iterative server activities.

  for (;;)
    {
      char buf[BUFSIZ];

      handle_set.reset ();
      handle_set.set_bit (peer_acceptor->get_handle ());

      int result = ACE_OS::select (int (peer_acceptor->get_handle ()) + 1,
                                   handle_set,
                                   0, 0, &tv);
      ACE_ASSERT (tv == def_timeout);

      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("select")),
                          0);
      else if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) select timed out, shutting down\n")));
          return 0;
        }

      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR).

      while ((result = peer_acceptor->accept (new_stream,
                                              &cli_addr)) != -1)
        {
          char *t = ACE_ALPHABET;

          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) client %s connected from %d\n"),
                      cli_addr.get_host_name (),
                      cli_addr.get_port_number ()));
        
          // Enable non-blocking I/O.
          if (new_stream.enable (ACE_NONBLOCK) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("(%P|%t) %p\n"),
                               ASYS_TEXT ("enable")),
                              0);
          handle_set.reset ();
          handle_set.set_bit (new_stream.get_handle ());
        
          // Read data from client (terminate on error).
        
          for (ssize_t r_bytes; ;)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("(%P|%t) waiting in select\n")));
              if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
                                  handle_set,
                                  0, 0, 0) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ASYS_TEXT ("(%P|%t) %p\n"),
                                   ASYS_TEXT ("select")),
                                  0);
              while ((r_bytes = new_stream.recv (buf, 1)) > 0)
                {
                  ACE_ASSERT (*t == buf[0]);
                  t++;
                }
        
              if (r_bytes == 0)
                {
                  // Handshake back with client.
                  ACE_DEBUG ((LM_DEBUG,
                              ASYS_TEXT ("(%P|%t) reached end of input, connection closed by client\n")));

                  if (new_stream.send_n ("", 1) != 1)
                    ACE_ERROR ((LM_ERROR,
                                ASYS_TEXT ("(%P|%t) %p\n"),
                                ASYS_TEXT ("send_n")));

                  // Close endpoint.
                  if (new_stream.close () == -1)
                    ACE_ERROR ((LM_ERROR,
                                ASYS_TEXT ("(%P|%t) %p\n"),
                                ASYS_TEXT ("close")));
                  break;
                }
              else if (r_bytes == -1)
                {
                  if (errno == EWOULDBLOCK || errno == EAGAIN)
                    ACE_DEBUG ((LM_DEBUG,
                                ASYS_TEXT ("(%P|%t) no input available, going back to reading\n")));
                  else
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       ASYS_TEXT ("(%P|%t) %p\n"),
                                       ASYS_TEXT ("recv_n")),
                                      0);
                }
            }
        }
      if (result == -1)
        {
          if (errno == EWOULDBLOCK)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("(%P|%t) no connections available, going back to accepting\n")));
          else
            ACE_ERROR ((LM_ERROR,
                        ASYS_TEXT ("(%P|%t) %p\n"),
                        ASYS_TEXT ("accept")));
        }
    }
  ACE_NOTREACHED (return 0);
}

static void
spawn (void)
{
  // Acceptor
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr;

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (ACE_Addr::sap_any) == -1
      || peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("open")));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
      for (size_t i = 0; i < ACE_MAX_CLIENTS; i++)
        {
          switch (ACE_OS::fork (ASYS_TEXT ("child")))
            {
            case -1:
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) %p\n", "fork failed"));
              i = ACE_MAX_CLIENTS;      
              // Break out of 'for' loop.
              break;
            case 0:
              client (&server_addr);
              exit (0);
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
                    "(%P|%t) reaping pid %d\n",
                    pid));

#elif defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (server),
           (void *) &peer_acceptor,
           THR_BOUND | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("(%P|%t) %p\n%a"),
                    ASYS_TEXT ("spawn failed")));

      if (ACE_Thread_Manager::instance ()->spawn_n
          (ACE_MAX_CLIENTS,
           ACE_THR_FUNC (client),
           (void *) &server_addr,
           THR_BOUND | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("(%P|%t) %p\n%a"),
                    ASYS_TEXT ("spawn failed")));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
#else
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) only one thread may be run in a process on this platform\n%a"),
                  1));
#endif /* !ACE_LACKS_FORK */    
      peer_acceptor.close ();
    }
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("MT_SOCK_Test"));

  spawn ();

  ACE_END_TEST;
  return 0;
}
