// $Id$

// This example tests the features of the ACE_SOCK_Acceptor and
// ACE_SOCK_Stream classes.  If the platform supports threads it uses
// a thread-per-connection concurrency model.

#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"
#include "ace/Profile_Timer.h"

// Are we running verbosely?
static int verbose = 0;

// Function entry point into the twoway server task.

static void *
twoway_server (void *arg)
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

  // Timer business
  ACE_Profile_Timer timer;
  timer.start ();

  size_t total_bytes = 0;
  size_t message_count = 0;

  void *buf;
  ACE_UINT32 len;
  
  if (new_stream.recv_n ((void *) &len,
                         sizeof (ACE_UINT32)) != sizeof (ACE_UINT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      0);
  else
    {
      len = ntohl (len);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) reading messages of size %d\n",
                  len));
      buf = ACE_OS::malloc (len);
    }
  // Read data from client (terminate on error).

  for (;;)
    {
      ssize_t r_bytes = new_stream.recv (buf, len);
      
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

      total_bytes += size_t (r_bytes);
      message_count++;
    }

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n"),
	      et.real_time,
	      et.user_time,
	      et.system_time));

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\t messages = %d\n\t\t total bytes = %d\n\t\tmbits/sec = %f\n\t\tusec-per-message = %f\n"),
	      message_count,
              total_bytes,
	      (((double) total_bytes * 8) / et.real_time) / (double) (1024 * 1024),
	      ((et.user_time + et.system_time) / (double) message_count) * 1000000));

  // Close new endpoint (listening endpoint stays open).
  new_stream.close ();

  ACE_OS::free (buf);
  return 0;
}

// Function entry point into the oneway server task.

static void *
oneway_server (void *arg)
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

  // Timer business
  ACE_Profile_Timer timer;
  timer.start ();

  size_t total_bytes = 0;
  size_t message_count = 0;

  void *buf;
  ACE_UINT32 len;
  
  if (new_stream.recv_n ((void *) &len,
                         sizeof (ACE_UINT32)) != sizeof (ACE_UINT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      0);
  else
    {
      len = ntohl (len);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) reading messages of size %d\n",
                  len));
      buf = ACE_OS::malloc (len);
    }
  // Read data from client (terminate on error).

  for (;;)
    {
      ssize_t r_bytes = new_stream.recv (buf, len);
      
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

      total_bytes += size_t (r_bytes);
      message_count++;
    }

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n"),
	      et.real_time,
	      et.user_time,
	      et.system_time));

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\t messages = %d\n\t\t total bytes = %d\n\t\tmbits/sec = %f\n\t\tusec-per-message = %f\n"),
	      message_count,
              total_bytes,
	      (((double) total_bytes * 8) / et.real_time) / (double) (1024 * 1024),
	      ((et.user_time + et.system_time) / (double) message_count) * 1000000));

  // Close new endpoint (listening endpoint stays open).
  new_stream.close ();

  ACE_OS::free (buf);
  return 0;
}

static int
run_event_loop (u_short port)
{
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

  fd_set handles;
  
  FD_ZERO (&handles);
  FD_SET (twoway_acceptor.get_handle (), &handles);
  FD_SET (oneway_acceptor.get_handle (), &handles);

  // Performs the iterative server activities.

  for (;;)
    {
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
      fd_set temp = handles;

      int result = ACE_OS::select (int (oneway_acceptor.get_handle ()) + 1,
                                   (fd_set *) &temp,
                                   0,
                                   0,
                                   timeout);
      if (result == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n", "select"));
      else if (result == 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) select timed out\n"));
      else
        {
          if (FD_ISSET (twoway_acceptor.get_handle (), &temp))
            {
              if (twoway_acceptor.accept (new_stream) == -1)
                {
                  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
                  continue;
                }          
              else
                ACE_DEBUG ((LM_DEBUG,
                            "(%P|%t) spawning twoway server\n"));

#if defined (ACE_HAS_THREADS)
              // Spawn a new thread and run the new connection in that thread of
              // control using the <server> function as the entry point.
              if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) twoway_server,
                                                          (void *) new_stream.get_handle (),
                                                          THR_DETACHED) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%P|%t) %p\n",
                                   "spawn"),
                          1);
#else
              twoway_server ((void *) new_stream.get_handle ());
#endif /* ACE_HAS_THREADS */
            }
          if (FD_ISSET (oneway_acceptor.get_handle (), &temp))
            {
              if (oneway_acceptor.accept (new_stream) == -1)
                {
                  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
                  continue;
                }          
              else
                ACE_DEBUG ((LM_DEBUG,
                            "(%P|%t) spawning oneway server\n"));

#if defined (ACE_HAS_THREADS)
              // Spawn a new thread and run the new connection in that thread of
              // control using the <server> function as the entry point.
              if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) oneway_server,
                                                          (void *) new_stream.get_handle (),
                                                          THR_DETACHED) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%P|%t) %p\n",
                                   "spawn"),
                          1);
#else
              oneway_server ((void *) new_stream.get_handle ());
#endif /* ACE_HAS_THREADS */
            }
        }
    }

  /* NOTREACHED */
}

int
main (int argc, char *argv[])
{
  return run_event_loop (argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT);
}
