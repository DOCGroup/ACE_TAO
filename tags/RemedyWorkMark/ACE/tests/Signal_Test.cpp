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
#include "ace/ACE.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/SString.h"



#if !defined (ACE_LACKS_UNIX_SIGNALS)

// Global options.
static size_t n_iterations = 10000;

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
  // ACE_DEBUG / ACE_ERROR invocations have been #if'd out because
  // they are "unsafe" when handler is invoked asynchronously.  On
  // NetBSD 3.X, calls to change the thread's signal mask block as
  // a lock seems to be held by the signal trampoline code.

#if 0
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) received signal %S\n"),
              signum));
#endif

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
#if 0
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) shutting down due to %S\n"),
                  signum));
#endif

      // Signal to the worker thread to shut down.
      shut_down = 1;

      // Bail out and close down.
      return -1;
      /* NOTREACHED */

    case SIGHUP:
      {
        // Shutdown the child.

#if 0
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) killing child pid %d\n"),
                    child_pid));
#endif
        int const result = ACE_OS::kill (child_pid,
                                         SIGTERM);
        ACE_TEST_ASSERT (result != -1);

        return -1;
      }
      /* NOTREACHED */
    case -1:
#if 0
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  "sigwait"));
#endif
      return -1;
      /* NOTREACHED */
    default:
#if 0
      ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("(%P|%t) signal %S unexpected\n"),
                 signum));
#endif
      return -1;
      /* NOTREACHED */
    }
}

// This function handles signals synchronously.

static ACE_THR_FUNC_RETURN
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
      int signr = ACE_OS::sigwait (sigset);
      if (signr == -1)
        {
          if (errno != EINTR)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("sigwait")));
          continue;
        }
      if (handle_signal (signr) == -1)
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

static ACE_THR_FUNC_RETURN
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

static ACE_THR_FUNC_RETURN
worker_child (void *arg)
{
  long handle_signals_synchronously =
    reinterpret_cast <long> (arg);

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
          int const result = ACE_OS::kill (parent_pid,
                                           SIGHUP);
          if (result == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p\n"),
                          ACE_TEXT ("kill")));
              ACE_TEST_ASSERT (result != -1);
            }
        }
    }

  if (handle_signals_synchronously)
    {
      if (!shut_down)
        {
          ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) sending SIGINT to ourselves\n")));
          // We need to do this to dislodge the signal handling thread if
          // it hasn't shut down on its own accord yet.
          int const result = ACE_OS::kill (ACE_OS::getpid (), SIGINT);
          ACE_TEST_ASSERT (result != -1);
        }
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) finished running child\n")));
  return 0;
}

// This function runs the parent process in a separate worker thread.
static ACE_THR_FUNC_RETURN
worker_parent (void *arg)
{
  long handle_signals_synchronously =
    reinterpret_cast <long> (arg);
  ACE_Process_Options options;

  ACE_TCHAR *l_argv[3];
  ACE_TCHAR pid_str[100];
  // Store the parent's process id so we can pass it to the child
  // portably.  Also, pass the test number, as well.
  ACE_OS::sprintf (pid_str,
                   ACE_TEXT ("-p %ld -t %d"),
                   static_cast <long> (parent_pid),
                   test_number);

  // We're going to create a new process that runs this program again,
  // so we need to indicate that it's the child.
  const ACE_TCHAR *t = ACE_TEXT (".")
                       ACE_DIRECTORY_SEPARATOR_STR
                       ACE_TEXT ("%sSignal_Test")
                       ACE_PLATFORM_EXE_SUFFIX
                       ACE_TEXT (" -c");
  l_argv[0] = const_cast <ACE_TCHAR *> (t);
  l_argv[1] = pid_str;
  l_argv[2] = 0;

  ACE_ARGV argv (l_argv);

  // Generate a command-line!
  ACE_TString exe_sub_dir;
  const char *subdir_env = ACE_OS::getenv ("ACE_EXE_SUB_DIR");
  if (subdir_env)
    {
      exe_sub_dir = ACE_TEXT_CHAR_TO_TCHAR (subdir_env);
      exe_sub_dir += ACE_DIRECTORY_SEPARATOR_STR;
    }

  options.command_line (argv.buf (), exe_sub_dir.c_str ());
  ACE_Process pm;

  child_pid = pm.spawn (options);

  if (child_pid == ACE_INVALID_PID)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) spawning child process failed\n"), 0);
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) spawning child process %d\n"),
                child_pid));

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
        if (ACE_OS::sigsuspend (0) == -1 && errno != EINTR)
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
      // For the synchronous signal tests, block signals to prevent
      // asynchronous delivery to default handler (at least necessary
      // on linux and solaris; POSIX spec also states that signal(s)
      // should be blocked before call to sigwait())
      ACE_Sig_Guard guard;

      int result;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) spawning worker thread\n")));
      result = ACE_Thread_Manager::instance ()->spawn
                (worker,
                  reinterpret_cast <void *> (handle_signals_synchronously),
                  THR_DETACHED);
      ACE_TEST_ASSERT (result != -1);

      if (handle_signals_in_separate_thread)
        {
          ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) spawning signal handler thread\n")));

          result = ACE_Thread_Manager::instance ()->spawn
            (synchronous_signal_handler,
             0,
             THR_DETACHED);
          ACE_TEST_ASSERT (result != -1);
        }
      else
        {
          synchronous_signal_handler (0);
        }

      // Wait for the thread(s) to finish.
      result = ACE_Thread_Manager::instance ()->wait ();
      ACE_TEST_ASSERT (result != -1);
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
      (*worker) (reinterpret_cast <void *> (handle_signals_synchronously));
    }
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:chp:t:"));

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
                  ACE_TEXT ("(%P|%t) usage:\n")
                  ACE_TEXT ("-i <iterations>\n")
                  ACE_TEXT ("-c\n")
                  ACE_TEXT ("-p <parent_pid>\n")
                  ACE_TEXT ("-t <test_number>\n")));
      break;
  }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_OS::signal(SIGHUP, SIG_DFL);

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

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 1: handle signals synchronously in a separate thread\n")));

#ifdef ACE_HAS_THREADS
      ++test_number;
      // Run the parent logic for the signal test, first by handling
      // signals synchronously in a separate thread.
      run_test (worker_parent, 1L, 1L);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 2: handle signals synchronously in this thread\n")));

      ++test_number;
      // And next by handling synchronously signals in this thread.
      run_test (worker_parent, 0L, 1L);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) **** test 3: handle signals asynchronously in this thread\n")));
#else
      test_number += 2;
#endif /* ACE_HAS_THREADS */

      ++test_number;
      // And finally by handling asynchronously signals in this thread.
      run_test (worker_parent, 0L, 0L);

      ACE_END_TEST;
    }
  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Signal_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("The Unix Signals capability is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* !defined (ACE_LACKS_UNIX_SIGNALS) */
