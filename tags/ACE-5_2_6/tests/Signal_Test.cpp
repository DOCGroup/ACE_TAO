// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Signal_Test.cpp
//
// = DESCRIPTION
//      This program tests the signal handling capabilities of ACE on
//      various OS platforms that support sending signals between
//      processes.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Process.h"
#include "ace/Signal.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"

ACE_RCSID(tests, Signal_Test, "$Id$")

#if !defined (ACE_LACKS_FORK) && !defined (ACE_LACKS_UNIX_SIGNALS)

// Global options.
static size_t n_iterations = 100000;

// Keeps track of whether we're the child or not.
static int child = 0;

// Keep track of the child pid.
static pid_t child_pid = 0;

// Keep track of the (original) parent pid.
static pid_t parent_pid = 0;

// Keep track of which test we're running.
static int test_number = 0;

// Coordinate the shutdown between threads.
static sig_atomic_t shut_down = 0;

static int
handle_signal (int signum)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) received signal %S\n"),
              signum));

  switch (signum)
    {
    case SIGCHLD:
      // Signal to the main thread to shut down.
      shut_down = 1;

      // This should only occur for the asynchronous case, so we don't
      // need to return -1!
      return 0;
    case SIGINT:
      /* FALLTHRU */
    case SIGTERM:
      // Shut down our thread using <ACE_Thread_Manager::exit>.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) shutting down due to %S\n"),
                  signum));

      // Signal to the worker thread to shut down.
      shut_down = 1;

      // Bail out and close down.
      return -1;
      /* NOTREACHED */

    case SIGHUP:
      {
        // Shutdown the child.

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) killing child pid %d \n"),
                    child_pid));
        int result = ACE_OS::kill (child_pid,
                                   SIGTERM);
        ACE_ASSERT (result != -1);

        return -1;
      }
      /* NOTREACHED */
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) %p\n"),
                         "sigwait"),
                        -1);
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) signal %S unexpected\n"),
                         signum),
                        -1);
      /* NOTREACHED */
    }
}

// This function handles signals synchronously.

static void *
synchronous_signal_handler (void *)
{
  ACE_Sig_Set sigset;

  // Register signal handlers.
  if (child)
    {
      sigset.sig_add (SIGINT);
      sigset.sig_add (SIGTERM);
    }
  else
    sigset.sig_add (SIGHUP);

  for (;;)
    {
      // Block waiting for SIGINT, SIGTERM, or SIGHUP, depending on
      // whether we're the parent or child process.
      if (handle_signal (ACE_OS::sigwait (sigset)) == -1)
        break;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) handled signal\n")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) synchronous signal handler done\n")));

  return 0;
}

// This function arranges to handle signals asynchronously, which is
// necessary if an OS platform lacks threads.

static void *
asynchronous_signal_handler (void *)
{
  ACE_Sig_Set sigset;

  // Register signal handlers.
  if (child)
    {
      sigset.sig_add (SIGINT);
      sigset.sig_add (SIGTERM);
    }
  else
    {
      sigset.sig_add (SIGCHLD);
      sigset.sig_add (SIGHUP);
    }

  // Register the <handle_signal> method to process all the signals in
  // <sigset>.
  ACE_Sig_Action sa (sigset,
                     (ACE_SignalHandler) handle_signal);
  ACE_UNUSED_ARG (sa);

  return 0;
}

// Function that runs in the child process in its own worker thread.

static void *
worker_child (void *arg)
{
  long handle_signals_synchronously =
    ACE_reinterpret_cast (long, arg);

  for (size_t i = 0; i < n_iterations; i++)
    {
      if (shut_down > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) we've been shutdown!\n")));
          break;
        }

      // Every 100 iterations sleep for 2 seconds.
      if ((i % 100) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) sleeping for 2 seconds\n")));
          ACE_OS::sleep (2);
        }

      // After 1000 iterations sent a SIGHUP to our parent.
      if ((i % 1000) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) sending SIGHUP to parent process %d\n"),
                      parent_pid));
          int result = ACE_OS::kill (parent_pid,
                                     SIGHUP);
          if (result == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p\n"),
                          ACE_TEXT ("kill")));
              ACE_ASSERT (result != -1);
            }
        }
    }

  if (handle_signals_synchronously)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) sending SIGINT to ourselves\n")));
      // We need to do this to dislodge the signal handling thread if
      // it hasn't shut down on its own accord yet.
      int result = ACE_OS::kill (ACE_OS::getpid (),
                                 SIGINT);
      ACE_ASSERT (result != -1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) finished running child\n")));
  return 0;
}

// This function runs the parent process in a separate worker thread.

static void *
worker_parent (void *arg)
{
  long handle_signals_synchronously =
    ACE_reinterpret_cast (long, arg);
  ACE_Process_Options options;

  ACE_TCHAR *l_argv[3];
  ACE_TCHAR pid_str[100];
  // Store the parent's process id so we can pass it to the child
  // portably.  Also, pass the test number, as well.
  ACE_OS::sprintf (pid_str,
                   "-p %ld -t %d",
                   ACE_static_cast (long, parent_pid),
                   test_number);

  // We're going to create a new process that runs this program again,
  // so we need to indicate that it's the child.
  const ACE_TCHAR *t = ACE_TEXT (".")
                       ACE_DIRECTORY_SEPARATOR_STR
                       ACE_TEXT ("Signal_Test")
                       ACE_PLATFORM_EXE_SUFFIX
                       ACE_TEXT (" -c");
  l_argv[0] = ACE_const_cast (ACE_TCHAR *,
                              t);
  l_argv[1] = pid_str;
  l_argv[2] = 0;

  ACE_ARGV argv (l_argv);

  // Generate a command-line!
  options.command_line (argv.buf ());
  ACE_Process pm;

  child_pid = pm.spawn (options);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) spawning child process %d\n"),
              child_pid));

  ACE_ASSERT (child_pid != -1);

  // Perform a <wait> until our child process has exited.

  if (handle_signals_synchronously)
    {
      int status;
      // Wait for the child process to exit.
      pm.wait (&status);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) reaped child with status %d\n"),
                  status));
    }
  else
    while (shut_down == 0)
      {
        // Wait for a signal to arrive.
        if (ACE_OS::sigsuspend (0) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("sigsuspend")));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) got signal!\n")));
      }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) parent worker done\n")));
  return 0;
}

// This is the driver function that spawns threads to run the test for
// the parent and the child process.

static void
run_test (ACE_THR_FUNC worker,
          long handle_signals_in_separate_thread,
          long handle_signals_synchronously)
{
#if defined (ACE_HAS_THREADS)
  if (handle_signals_synchronously)
    {
      int result;
      {
        // Block all signals before spawning the threads.  Then,
        // unblock these signals as the scope is exited.
        ACE_Sig_Guard guard;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) spawning worker thread\n")));
        result = ACE_Thread_Manager::instance ()->spawn
          (worker,
           ACE_reinterpret_cast (void *,
                                 handle_signals_synchronously),
           THR_DETACHED);
        ACE_ASSERT (result != -1);

        if (handle_signals_in_separate_thread)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) spawning signal handler thread\n")));

            result = ACE_Thread_Manager::instance ()->spawn
              (synchronous_signal_handler,
               0,
               THR_DETACHED);
            ACE_ASSERT (result != -1);

            // Wait for the other threads to finish.
            result = ACE_Thread_Manager::instance ()->wait ();
            ACE_ASSERT (result != -1);
          }
      }
      if (handle_signals_in_separate_thread == 0)
        {
          synchronous_signal_handler (0);

          // Wait for the other thread to finish.
          result = ACE_Thread_Manager::instance ()->wait ();
          ACE_ASSERT (result != -1);
        }
    }
  else
#else
    // Don't remove this since otherwise some compilers give warnings
    // when ACE_HAS_THREADS is disabled!
    ACE_UNUSED_ARG (synchronous_signal_handler);
#endif /* ACE_HAS_THREADS */
    {
      ACE_UNUSED_ARG (handle_signals_in_separate_thread);
      // Arrange to handle signals asynchronously.
      asynchronous_signal_handler (0);
      (*worker) (ACE_reinterpret_cast (void *,
                                       handle_signals_synchronously));
    }
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:chp:t:");

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      n_iterations = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'c':
      child = 1;
      break;
    case 'p':
      parent_pid = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 't':
      test_number = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) usage:\n"
		  ACE_TEXT ("-i <iterations>\n")
		  ACE_TEXT ("-c\n")
		  ACE_TEXT ("-p <parent_pid>\n")
		  ACE_TEXT ("-t <test_number>\n")));
      break;
  }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)
    {
      ACE_APPEND_LOG (ACE_TEXT ("Signal_Test-child"));
      parse_args (argc, argv);

      if (test_number == 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) **** test 1: handle signals synchronously in separate thread\n")));

          // First, handle signals synchronously in separate thread.
          run_test (worker_child, 1, 1);
        }
      else if (test_number == 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) **** test 2: handle signals synchronously in this thread\n")));

          // Next, handle signals synchronously in this thread.
          run_test (worker_child, 0, 1);
        }
      else if (test_number == 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) **** test 3: handle signals asynchronously in this thread\n")));

          // Finally, handle signals asynchronously in this thread.
          run_test (worker_child, 0, 0);
        }

      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("Signal_Test"));
      ACE_INIT_LOG (ACE_TEXT ("Signal_Test-child"));

      // We need to get the process id here to work around "features"
      // of Linux threads...
      parent_pid = ACE_OS::getpid ();

#if !defined (linux)
      // Linux threads don't support this use-case very well.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 1: handle signals synchronously in a separate thread\n")));

      test_number++;
      // Run the parent logic for the signal test, first by handling
      // signals synchronously in a separate thread.
      run_test (worker_parent, 1L, 1L);
#else
      // Must increment anyhow.
      test_number++;
#endif /* linux */

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 2: handle signals synchronously in this thread\n")));

      test_number++;
      // And next by handling synchronously signals in this thread.
      run_test (worker_parent, 0L, 1L);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 3: handle signals asynchronously in this thread\n")));

      test_number++;
      // And finally by handling asynchronously signals in this thread.
      run_test (worker_parent, 0L, 0L);

      ACE_END_TEST;
    }
  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Signal_Test"));
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("The ACE_Process capability is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* !ACE_LACKS_FORK && !defined (ACE_LACKS_UNIX_SIGNALS) */
