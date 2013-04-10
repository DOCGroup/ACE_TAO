// $Id$

// This example tests the features of the <ACE_SOCK_Acceptor>,
// <ACE_SOCK_Stream>, and <ACE_Svc_Handler> classes.  If the platform
// supports threads it uses a thread-per-connection concurrency model.
// Otherwise, it uses a single-threaded iterative server model.

#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"
#include "ace/Profile_Timer.h"
#include "ace/Basic_Types.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_main.h"
#include "ace/Truncate.h"


// Are we running verbosely?
static int verbose = 0;

static void
run_server (ACE_THR_FUNC server,
            ACE_HANDLE handle)
{
#if defined (ACE_HAS_THREADS)
  // Spawn a new thread and run the new connection in that thread of
  // control using the <server> function as the entry point.
  if (ACE_Thread_Manager::instance ()->spawn (server,
                                              reinterpret_cast<void *> (handle),
                                              THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "spawn"));
#else
  (*server) (reinterpret_cast<void *> (handle));
#endif /* ACE_HAS_THREADS */
}

// Function entry point into the twoway server task.

static ACE_THR_FUNC_RETURN
twoway_server (void *arg)
{
  ACE_INET_Addr cli_addr;
  ACE_SOCK_Stream new_stream;
  ACE_HANDLE handle = (ACE_HANDLE) (intptr_t) arg;

  new_stream.set_handle (handle);

  // Make sure we're not in non-blocking mode.
  if (new_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "disable"),
                       0);
  else if (new_stream.get_remote_addr (cli_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_remote_addr"),
                       0);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client %s connected from %d\n",
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

  size_t total_bytes = 0;
  size_t message_count = 0;

  char *request = 0;

  // Read data from client (terminate on error).

  for (;;)
    {
      ACE_INT32 len;

      ssize_t r_bytes = new_stream.recv_n ((void *) &len,
                                           sizeof (ACE_INT32));
      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (r_bytes != sizeof (ACE_INT32))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) %p\n",
                      "recv_n failed"));
          break;
        }
      else
        {
          len = ACE_NTOHL (len);
          ACE_NEW_RETURN (request,
                          char [len],
                          0);
        }

      // Subtract off the sizeof the length prefix.
      r_bytes = new_stream.recv_n (request,
                                   len - sizeof (ACE_UINT32));
      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (verbose
               && ACE::write_n (ACE_STDOUT,
                                request,
                                r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));
      else if (new_stream.send_n (request,
                                  r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "send_n"));

      total_bytes += size_t (r_bytes);
      message_count++;

      delete [] request;
      request = 0;
    }

  // Close new endpoint (listening endpoint stays open).
  new_stream.close ();

  delete [] request;
  return 0;
}

// Function entry point into the oneway server task.

static ACE_THR_FUNC_RETURN
oneway_server (void *arg)
{
  ACE_INET_Addr cli_addr;
  ACE_SOCK_Stream new_stream;
  ACE_HANDLE handle = (ACE_HANDLE) (intptr_t) arg;

  new_stream.set_handle (handle);

  // Make sure we're not in non-blocking mode.
  if (new_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "disable"),
                       0);
  else if (new_stream.get_remote_addr (cli_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_remote_addr"),
                       0);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client %s connected from %d\n",
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

  // Timer business
  ACE_Profile_Timer timer;
  timer.start ();

  size_t total_bytes = 0;
  size_t message_count = 0;

  char *request = 0;

  // Read data from client (terminate on error).

  for (;;)
    {
      ACE_INT32 len;

      ssize_t r_bytes = new_stream.recv_n ((void *) &len,
                                           sizeof (ACE_INT32));
      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (r_bytes != sizeof (ACE_INT32))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) %p\n",
                      "recv_n failed"));
          break;
        }
      else
        {
          len = ACE_NTOHL (len);
          ACE_NEW_RETURN (request,
                          char [len],
                          0);
        }

      // Subtract off the sizeof the length prefix.
      r_bytes = new_stream.recv_n (request,
                                   len - sizeof (ACE_UINT32));

      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (verbose
               && ACE::write_n (ACE_STDOUT, request, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));

      total_bytes += size_t (r_bytes);
      message_count++;

      delete [] request;
      request = 0;
    }

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  double messages_per_sec = double (message_count) / et.real_time;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\t\tmessages = %d\n\t\ttotal bytes = %d\n\t\tmbits/sec = %f\n\t\tusec-per-message = %f\n\t\tmessages-per-second = %0.00f\n"),
              message_count,
              total_bytes,
              (((double) total_bytes * 8) / et.real_time) / (double) (1024 * 1024),
              (et.real_time / (double) message_count) * 1000000,
              messages_per_sec < 0 ? 0 : messages_per_sec));

  // Close new endpoint (listening endpoint stays open).
  new_stream.close ();

  delete [] request;
  return 0;
}

static int
run_event_loop (u_short port)
{
  // Raise the socket handle limit to the maximum.
  ACE::set_handle_limit ();

  // Create the oneway and twoway acceptors.
  ACE_SOCK_Acceptor twoway_acceptor;
  ACE_SOCK_Acceptor oneway_acceptor;

  // Create the oneway and twoway server addresses.
  ACE_INET_Addr twoway_server_addr (port);
  ACE_INET_Addr oneway_server_addr (port + 1);

  // Create acceptors, reuse the address.
  if (twoway_acceptor.open (twoway_server_addr, 1) == -1
      || oneway_acceptor.open (oneway_server_addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      1);
  // Check to see what addresses we actually got bound to!
  else if (twoway_acceptor.get_local_addr (twoway_server_addr) == -1
           || oneway_acceptor.get_local_addr (oneway_server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_local_addr"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting twoway server at port %d and oneway server at port %d\n",
              twoway_server_addr.get_port_number (),
              oneway_server_addr.get_port_number ()));

  // Keep these objects out here to prevent excessive constructor
  // calls within the loop.
  ACE_SOCK_Stream new_stream;

  ACE_Handle_Set handle_set;
  handle_set.set_bit (twoway_acceptor.get_handle ());
  handle_set.set_bit (oneway_acceptor.get_handle ());

  // Performs the iterative server activities.

  for (;;)
    {
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
      ACE_Handle_Set temp = handle_set;

      int result = ACE_OS::select (ACE_Utils::truncate_cast<int> ((intptr_t)oneway_acceptor.get_handle ()) + 1,
                                   (fd_set *) temp,
                                   0,
                                   0,
                                   timeout);
      if (result == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "select"));
      else if (result == 0 && verbose)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) select timed out\n"));
      else
        {
          if (temp.is_set (twoway_acceptor.get_handle ()))
            {
              if (twoway_acceptor.accept (new_stream) == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              "%p\n",
                              "accept"));
                  continue;
                }
              else
                ACE_DEBUG ((LM_DEBUG,
                            "(%P|%t) spawning twoway server\n"));

              // Run the twoway server.
              run_server (twoway_server,
                          new_stream.get_handle ());
            }
          if (temp.is_set (oneway_acceptor.get_handle ()))
            {
              if (oneway_acceptor.accept (new_stream) == -1)
                {
                  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
                  continue;
                }
              else
                ACE_DEBUG ((LM_DEBUG,
                            "(%P|%t) spawning oneway server\n"));

              // Run the oneway server.
              run_server (oneway_server,
                          new_stream.get_handle ());
            }
        }
    }

  /* NOTREACHED */
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  u_short port = ACE_DEFAULT_SERVER_PORT;

  if (argc > 1)
    port = ACE_OS::atoi (argv[1]);

  return run_event_loop (port);
}
