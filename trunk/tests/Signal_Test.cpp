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
//      a given OS platform.
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

// Coordinate the shutdown between threads.
static ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> shut_down (0);

static int
handle_signal (int signum)
{
  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT ("(%P|%t) received signal %S\n"),
              signum));

  switch (signum)
    {
    case SIGINT:
      /* FALLTHRU */
    case SIGTERM:
      // Shut_Down our thread using <ACE_Thread_Manager::exit>.
      ACE_DEBUG ((LM_DEBUG, 
                  ASYS_TEXT ("(%P|%t) shutting down\n")));

      // Signal to the main thread to shut_down.
      shut_down = 1;

      // Bail out and close down.
      return -1;
      /* NOTREACHED */

    case SIGHUP:
      {
        // Shutdown the child.
          
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%P|%t) killing child pid %d \n"),
                    child_pid));
        int result = ACE_OS::kill (child_pid, 
                                   SIGTERM);
        ACE_ASSERT (result != -1);

        return -1;
      }
      /* NOTREACHED */
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P|%t) %p\n"),
                         "sigwait"),
                        -1);
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_ERROR, 
                         ASYS_TEXT ("(%P|%t) signal %S unexpected\n"),
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
    {
      sigset.sig_add (SIGHUP);
    }

  for (;;)
    {
      // Block waiting for SIGINT, SIGTERM, or SIGHUP, depending on
      // whether we're the parent or child process.
      if (handle_signal (ACE_OS::sigwait (sigset)) == -1)
        break;
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) handled signal\n")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) parent handler done\n")));

  return 0;
}

#if 0 // !defined (ACE_HAS_THREADS)
// This function arranges to handle signals asynchronously, i.e., if
// the platform lacks threads.

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

#endif /* !ACE_HAS_THREADS */

// Function that runs in the child process in its own worker thread.

static void *
worker_child (void *)
{
  for (size_t i = 0; i < n_iterations; i++)
    {
      if (shut_down > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) we've been shutdown!\n")));
          break;
        }

      // Every 100 iterations sleep for 2 seconds.
      if ((i % 100) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) sleeping for 2 seconds\n")));
          ACE_OS::sleep (2);
        }

      // After 1000 iterations sent a SIGHUP to our parent.
      if ((i % 1000) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) sending SIGHUP to parent process %d\n"),
                      parent_pid));
          int result = ACE_OS::kill (parent_pid,
                                     SIGHUP);
          ACE_ASSERT (result != -1);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) finished running child\n")));
  return 0;
}

// This is the driver function that spawns threads to run the test for
// the parent and the child process.

static void
run_test (ACE_THR_FUNC worker)
{
#if 1 // defined (ACE_HAS_THREADS)
  int result;
  {
    // Block all signals before spawning the threads.  Then, 
    // unblock these signals as the scope is exited.
    ACE_Sig_Guard guard;
    
    result = ACE_Thread_Manager::instance ()->spawn 
      (worker, 0, THR_DETACHED);
    ACE_ASSERT (result != -1);
  
#if 0
    result = ACE_Thread_Manager::instance ()->spawn 
      (synchronous_signal_handler, 0, THR_DETACHED);
    ACE_ASSERT (result != -1);
#endif /* 0 */
  }
  synchronous_signal_handler (0);

  // Wait for the other thread to finish.
  result = ACE_Thread_Manager::instance ()->wait ();
  ACE_ASSERT (result != -1);
#else
  asynchronous_signal_handler (0);
  (*worker) (0);
#endif /* ACE_HAS_THREADS */
}

// This function runs the parent process in a separate worker thread.

static void *
worker_parent (void *)
{
  ACE_Process_Options options;

  ASYS_TCHAR *l_argv[3];
  TCHAR pid_str[12];
  // Store the parent's process id so we can pass it to the child
  // portably.
  ACE_OS::sprintf (pid_str, "%d", parent_pid);

  // We're going to create a new process that runs this program again,
  // so we need to indicate that it's the child.
  l_argv[0] = ASYS_TEXT (".")
              ACE_DIRECTORY_SEPARATOR_STR
              ASYS_TEXT ("Signal_Test")
              ACE_PLATFORM_EXE_SUFFIX
              ASYS_TEXT (" -c -p");
  l_argv[1] = pid_str;
  l_argv[2] = 0;
  
  ACE_ARGV argv (l_argv);

  // Generate a command-line!
  options.command_line (argv.buf ());
  ACE_Process pm;

  child_pid = pm.spawn (options);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) spawning child %d\n"),
              child_pid));

  ACE_ASSERT (child_pid != -1);

  // Perform a <wait> until our child process has exited.
  
#if 0
  while (shut_down == 0)
    {
      // Wait for a signal to arrive.
      if (ACE_OS::sigsuspend (0) == -1)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("(%P|%t) %p\n"),
                    ASYS_TEXT ("sigsuspend")));
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) got signal!\n")));
    }
#else
  pm.wait ();
#endif /* */ 

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) parent worker done\n")));
  return 0;
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:chp:");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      n_iterations = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'c':
      {
        child = 1;
        break;
      }
    case 'p':
      parent_pid = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) usage:\n"
		  ASYS_TEXT ("-i <iterations>\n")));
      break;
  }
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  if (argc > 1)
    {
      ACE_APPEND_LOG (ASYS_TEXT ("Signal_Test-child"));
      parse_args (argc, argv);

      run_test (worker_child);
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ASYS_TEXT ("Signal_Test"));
      ACE_INIT_LOG (ASYS_TEXT ("Signal_Test-child"));

      // We need to set this here to work around "features" of Linux
      // threads...
      parent_pid = ACE_OS::getpid ();

      // Run the parent logic for the signal test.
      run_test (worker_parent);

      ACE_END_TEST;
      return 0;
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_SYNCH_MUTEX, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_SYNCH_MUTEX, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int 
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Signal_Test"));
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("The ACE_Process capability is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* !ACE_LACKS_FORK && !defined (ACE_LACKS_UNIX_SIGNALS) */
