// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Manager_Test.cpp
//
// = DESCRIPTION
//      This program tests the group management mechanisms provided by
//      the <ACE_Process_Manager>.  In particular, it illustrates the
//      use of the following variants of the
//      <ACE_Process_Manager::wait> method:
//  
//      1. ACE_TEST_ONE_CHILD --
//
//         This test uses a version of <wait> requires the parent to
//         use the signal handling mechanism in
//         <one_child_sig_handler> to keep track of the counting of
//         remaining processes.  Therefore, this <wait> method is less
//         portable than the version that's used in the
//         <ACE_TEST_MULTIPLE_CHILDREN> test.
//
//      2. ACE_TEST_MULTIPLE_CHILDREN --
//
//         This test uses a version of <wait> that doesn't require any
//         signal handling by the parent, i.e., it just uses one of
//         the underlying <ACE_OS::wait> methods that's portable to
//         all versions of UNIX, as well as Windows NT.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu> 
//
// ============================================================================

#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Process_Manager.h"
#include "ace/Signal.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, Process_Manager_Test, "$Id$")

#if !defined (ACE_LACKS_FORK) && defined (ACE_HAS_THREADS)

// Global options.
static size_t n_iterations = 100000;

enum
{
  ACE_TEST_ONE_CHILD = 1,
  ACE_TEST_MULTIPLE_CHILDREN = 2
};

// Keeps track of which of the child tests enumerated above that we're
// running.
static int child_test = 0;

// Process ids of the children.
static pid_t child_pids[ACE_MAX_PROCESSES];

// Coordinate the shutdown between threads.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> shut_down (0);

// This function handles signals synchronously in a separate thread of
// control for the <ACE_TEST_ONE_CHILD> test.

static void *
one_child_sig_handler (void *)
{
  ACE_Sig_Set sigset;

  // Register ourselves as a "dummy" signal handler so that this
  // processes' disposition isn't SIG_IGN (which is the default).
  ACE_Sig_Action sa ((ACE_SignalHandler) one_child_sig_handler, SIGCHLD);
  ACE_UNUSED_ARG (sa);

  // Register signal handlers.
  if (child_test == ACE_TEST_ONE_CHILD)
    {
      sigset.sig_add (SIGINT);
      sigset.sig_add (SIGTERM);
    }
  else
    {
      sigset.sig_add (SIGCHLD);
      sigset.sig_add (SIGHUP);
    }

  for (int result;;)
    {
      // Block waiting for SIGINT, SIGCHLD, SIGTERM, or SIGHUP,
      // depending on whether we're the parent or child process.

      int signum = ACE_OS::sigwait (sigset);

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

          // Bail out.
          return 0;
	  /* NOTREACHED */

	case SIGCHLD:
	  
	  for (;;)
	    {
              int child_exit_status;

              // This method simply "reaps" the exit status of the
              // child without blocking.  Note that it also decrements
              // the count of waiting children by one.
	      pid_t pid =
                ACE_Process_Manager::instance ()->wait 
                (-1,
                 &child_exit_status,
                 WNOHANG);
              // Check to see if there are anymore children to reap.
	      if (pid == -1) 
		break;

	      ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("(%P|%t) reaped child pid %d with exit status %d\n"),
                          pid,
                          child_exit_status));
	    }

          // Bail out.
          return 0;
	  /* NOTREACHED */

        case SIGHUP:
          // Shutdown the child.
          
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) killing child pid %d \n"),
                      child_pids[0]));
          result = ACE_OS::kill (child_pids[0], 
                                 SIGTERM);
          ACE_ASSERT (result != -1);

          // Continue looping on <sigwait> until the child process
          // exits.
          break;
	  /* NOTREACHED */
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("(%P|%t) %p\n"),
                             "sigwait"),
                            0);
	  /* NOTREACHED */
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     ASYS_TEXT ("(%P|%t) signal %S unexpected\n"),
                             signum),
                            0);
	  /* NOTREACHED */
	}
    }
}

// Function that runs in the child process in its own worker thread
// for the <ACE_TEST_ONE_CHILD> test.

static void *
one_child_worker_child (void *)
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
          pid_t pid = ACE_OS::getppid ();
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) sending SIGHUP to parent process %d\n"),
                      pid));
          int result = ACE_OS::kill (pid, SIGHUP);
          ACE_ASSERT (result != -1);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) finished running child\n")));
  return 0;
}

// This function handles signals synchronously in a separate thread of
// control for the <ACE_TEST_MULTIPLE_CHILDREN> test.

static void *
multiple_children_sig_handler (void *)
{
  ACE_Sig_Set sigset;

  // Register signal handlers.
  if (child_test == ACE_TEST_MULTIPLE_CHILDREN)
    sigset.sig_add (SIGINT);
  else
    {
      sigset.sig_add (SIGINT);
      sigset.sig_add (SIGHUP);
    }

  for (;;)
    {
      // Block waiting for SIGINT or SIGHUP, depending on whether
      // we're the parent or child process.

      int signum = ACE_OS::sigwait (sigset);

      ACE_DEBUG ((LM_DEBUG, 
		  ASYS_TEXT ("(%P|%t) received signal %S\n"),
		  signum));

      switch (signum)
	{
	case SIGINT:
	  // Shut_Down our thread using <ACE_Thread_Manager::exit>.
          if (shut_down > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("(%P|%t) we've been shutdown!\n")));
          else
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("(%P|%t) hum, something weird happened...\n")));

          // Bail out.
          return 0;
	  /* NOTREACHED */

        case SIGHUP:
          // Continue looping on <sigwait> until all child processes
          // exit.
          break;
	  /* NOTREACHED */

	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("(%P|%t) %p\n"),
                             "sigwait"),
                            0);
	  /* NOTREACHED */
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     ASYS_TEXT ("(%P|%t) signal %S unexpected\n"),
                             signum),
                            0);
	  /* NOTREACHED */
	}
    }

  // @@ We never actually reach this code.  Comment to fix warning.
  // return 0;
}

// Function that runs in the child process in its own worker thread
// for the <ACE_TEST_MULTIPLE_CHILDREN> test.

static void *
multiple_children_worker_child (void *)
{
  for (size_t i = 0; i < n_iterations; i++)
    {
      // Every 10000 iterations sleep for 1 second.
      if ((i % 10000) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) sleeping for 1 second\n")));
          ACE_OS::sleep (1);
        }

      // After 25000 iterations sent a SIGHUP to our parent.
      if ((i % 25000) == 0)
        {
          pid_t pid = ACE_OS::getppid ();
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) sending SIGHUP to parent process %d\n"),
                      pid));
          int result = ACE_OS::kill (pid, SIGHUP);
          ACE_ASSERT (result != -1);
        }
    }

  // Signal to the <sig_wait> thread to shut_down.
  shut_down = 1;

  // Send ourselves a signal to shut down the <sigwait> thread!
  int result = ACE_OS::kill (ACE_OS::getpid (), 
                             SIGINT);
  ACE_ASSERT (result != -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) finished running child\n")));

  return 0;
}

// This is the driver function that spawns threads to run the various
// tests for the parent and the child process.

static void
run_test (ACE_THR_FUNC handler,
          ACE_THR_FUNC worker)
{
  // Block all signals.
  ACE_Sig_Set sigset (1);
  int result;
  
  result = ACE_OS::thr_sigsetmask (SIG_BLOCK,
                                   sigset,
                                   0);
  ACE_ASSERT (result != -1);

  result = ACE_Thread_Manager::instance ()->spawn 
    (handler, 0, THR_DETACHED);
  ACE_ASSERT (result != -1);

  result = ACE_Thread_Manager::instance ()->spawn 
    (worker, 0, THR_DETACHED);
  ACE_ASSERT (result != -1);
  
  // Wait for all threads to finish.
  result = ACE_Thread_Manager::instance ()->wait ();
  ACE_ASSERT (result != -1);
}

// This function runs the child process in a separate worker thread
// for the <ACE_TEST_MULTIPLE_CHILDREN> test.

static void *
multiple_children_worker_parent (void *)
{
  ACE_Process_Options options;

  // We're going to create a new process that runs this program
  // <ACE_MAX_PROCESSES> number of times, so we need to indicate that
  // these processes are the childen.
  options.command_line (ACE_TEXT (".")
                        ACE_DIRECTORY_SEPARATOR_STR
                        ACE_TEXT ("Process_Manager_Test")
                        ACE_PLATFORM_EXE_SUFFIX
                        ACE_TEXT (" -c 2"));
  int result =
    ACE_Process_Manager::instance ()->spawn_n (ACE_MAX_PROCESSES,
                                               options,
                                               child_pids);
  ACE_ASSERT (result != -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) child pids =\n")));

  for (size_t i = 0; i < ACE_MAX_PROCESSES; i++)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("[%d]\n"),
                child_pids[i]));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\n")));

  // Perform a barrier wait until all our child processes have exited.
  // Note that this <wait> doesn't require any signal handling by the
  // parent, i.e., it just uses one of the underlying <ACE_OS::wait>
  // methods that's portable to all versions of UNIX, as well as
  // Windows NT.
  result = ACE_Process_Manager::instance ()->wait ();
  ACE_ASSERT (result != -1);

  // Signal to the <sig_wait> thread to shut_down.
  shut_down = 1;

  // Send ourselves a signal to shut down the <sigwait> thread!
  result = ACE_OS::kill (ACE_OS::getpid (),
                         SIGINT);
  ACE_ASSERT (result != -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) test multiple children done\n")));
  return 0;
}

// This function runs the parent process in a separate worker thread
// for the <ACE_TEST_ONE_CHILDR> test.

static void *
one_child_worker_parent (void *)
{
  ACE_Process_Options options;

  // We're going to create a new process that runs this program again,
  // so we need to indicate that it's the child.
  options.command_line (ACE_TEXT (".")
                        ACE_DIRECTORY_SEPARATOR_STR
                        ACE_TEXT ("Process_Manager_Test")
                        ACE_PLATFORM_EXE_SUFFIX
                        ACE_TEXT (" -c 1"));
  child_pids[0] = ACE_Process_Manager::instance ()->spawn (options);

  ACE_ASSERT (child_pids[0] != -1);

  // Perform a barrier wait until all our child processes have exited.
  // Note that this <wait> requires the parent to use the signal
  // handling mechanism in <one_child_sig_handler> to keep track of
  // the counting of remaining processes.  Therefore, this <wait>
  // method is less portable than the version that's shown below in
  // the <multiple_children_worker_parent> function.
  int result = ACE_Process_Manager::instance ()->wait 
    ((ACE_Time_Value *) 0);
  ACE_ASSERT (result != -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) test single child done\n")));
  return 0;
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:c:h");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      n_iterations = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'c':
      {
        int value = ACE_OS::atoi (get_opt.optarg);
        if (value == 1)
          child_test = ACE_TEST_ONE_CHILD;
        else if (value == 2)
          child_test = ACE_TEST_MULTIPLE_CHILDREN;
        else
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%P|%t) unexpected child test %d\n"),
                      value));
        break;
      }
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
      ACE_APPEND_LOG (ASYS_TEXT ("Process_Manager_Test-children"));
      parse_args (argc, argv);

      if (child_test == ACE_TEST_ONE_CHILD)
        run_test (one_child_sig_handler,
                  one_child_worker_child);

      else if (child_test == ACE_TEST_MULTIPLE_CHILDREN)
        run_test (multiple_children_sig_handler,
                  multiple_children_worker_child);

      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ASYS_TEXT ("Process_Manager_Test"));
      ACE_INIT_LOG (ASYS_TEXT ("Process_Manager_Test-children"));

      // Run the parent logic for the <ACE_TEST_ONE_CHILD> test.
      run_test (one_child_sig_handler, 
                one_child_worker_parent);

      // Run the parent logic for the <ACE_TEST_MULTIPLE_CHILDREN>
      // test.
      run_test (multiple_children_sig_handler,
                multiple_children_worker_parent);

      ACE_END_TEST;
      return 0;
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int 
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Process_Manager_Test"));
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("The ACE Process Manager is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* !ACE_LACKS_FORK && ACE_HAS_THREADS */
