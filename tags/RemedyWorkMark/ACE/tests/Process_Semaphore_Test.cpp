// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Semaphore_Test.cpp
//
// = DESCRIPTION
//    Tests an ACE Semaphore shared between multiple child processes.
//
// = AUTHOR
//    Martin Corino <mcorino@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/Mutex.h"
#include "ace/Process.h"
#if defined (ACE_WIN32) || \
      defined (ACE_USES_FIFO_SEM) || \
      (defined (ACE_HAS_POSIX_SEM) && !defined (ACE_LACKS_NAMED_POSIX_SEM))
# include "ace/Time_Value.h"
# include "ace/OS_NS_sys_time.h"
# include "ace/Semaphore.h"
#else
# include "ace/Process_Semaphore.h"
#endif
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_dirent.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/SString.h"



#if !defined (ACE_LACKS_FORK)
static int iterations = 10;
static int child_process = 0;
static const char *sema_ping_name = "ACE_Ping_Semaphore";
static const char *sema_pong_name = "ACE_Pong_Semaphore";

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-i #iterations] [-c (child process)]\n")));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:c"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'i':
      iterations = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'c':
      child_process = 1;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

static void
acquire_release (void)
{
#if defined (ACE_WIN32) || \
      defined (ACE_USES_FIFO_SEM) || \
      (defined (ACE_HAS_POSIX_SEM) && !defined (ACE_LACKS_NAMED_POSIX_SEM))
  ACE_Semaphore sema_ping (0, USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (sema_ping_name));
  ACE_Semaphore sema_pong (0, USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (sema_pong_name));
#else
  ACE_Process_Semaphore sema_ping (0, ACE_TEXT_CHAR_TO_TCHAR (sema_ping_name));
  ACE_Process_Semaphore sema_pong (0, ACE_TEXT_CHAR_TO_TCHAR (sema_pong_name));
#endif

  // Make sure the constructor succeeded
  ACE_TEST_ASSERT (ACE_LOG_MSG->op_status () == 0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Begin ping-pong\n")));

  if (child_process)
  {
    for (int i=0; i<iterations ;++i)
    {
      sema_ping.release ();

      if (sema_pong.acquire ())
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Failed acquiring pong\n")));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P) Pong\n")));
    }

#if defined (ACE_WIN32) || \
      defined (ACE_USES_FIFO_SEM) || \
      (defined (ACE_HAS_POSIX_SEM) && defined (ACE_HAS_POSIX_SEM_TIMEOUT) && \
       !defined (ACE_LACKS_NAMED_POSIX_SEM))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Testing timeouts\n")));

    // test timed wait
    ACE_Time_Value wait = ACE_OS::gettimeofday ();
    wait.sec (wait.sec () + 3); // timeout in 3 secs

    if (sema_pong.acquire (wait))
        ACE_TEST_ASSERT(errno == ETIME);
    else
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Acquired pong without release()\n")));

    sema_ping.release ();   // release waiting parent before timeout
#endif
  }
  else
  {
    for (int i=0; i<iterations ;++i)
    {
      if (sema_ping.acquire ())
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Failed acquiring ping\n")));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P) Ping\n")));

      sema_pong.release ();
    }

#if defined (ACE_WIN32) || \
      defined (ACE_USES_FIFO_SEM) || \
      (defined (ACE_HAS_POSIX_SEM) && defined (ACE_HAS_POSIX_SEM_TIMEOUT) && \
       !defined (ACE_LACKS_NAMED_POSIX_SEM))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Testing timeouts\n")));

    // test timed wait
    ACE_Time_Value wait = ACE_OS::gettimeofday ();
    wait.sec (wait.sec () + 10); // timeout in 10 secs

    if (sema_ping.acquire (wait))
    {
        ACE_TEST_ASSERT(errno == ETIME);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Acquiring pong timed out\n")));
    }
#endif
  }
}
#endif /* ! ACE_LACKS_FORK */

int
run_main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Process_Semaphore_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("fork is not supported on this platform\n")));
  ACE_END_TEST;
#else

  parse_args (argc, argv);

  // Child process code.
  if (child_process)
    {
      ACE_START_TEST (ACE_TEXT ("Process_Semaphore_Test-child"));
      acquire_release ();
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("Process_Semaphore_Test"));

      ACE_TString exe_sub_dir;
      const char *subdir_env = ACE_OS::getenv ("ACE_EXE_SUB_DIR");
      if (subdir_env)
        {
          exe_sub_dir = ACE_TEXT_CHAR_TO_TCHAR (subdir_env);
          exe_sub_dir += ACE_DIRECTORY_SEPARATOR_STR;
        }

      ACE_Process_Options options;
      options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                            ACE_TEXT ("%sProcess_Semaphore_Test")
                            ACE_PLATFORM_EXE_SUFFIX
                            ACE_TEXT (" -c -i %d"),
                            exe_sub_dir.c_str(),
                            iterations);

      // Spawn a child process that will contend for the
      // lock.
      ACE_Process child;

      // Spawn the child process.
      int result = child.spawn (options);
      ACE_TEST_ASSERT (result != -1);
      ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Parent spawned child process with pid = %d.\n"),
                child.getpid ()));

      // start test
      acquire_release ();

      ACE_exitcode child_status;
      // Wait for the child processes we created to exit.
      ACE_TEST_ASSERT (child.wait (&child_status) != -1);
      if (child_status == 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Child %d finished ok\n"),
                    child.getpid ()));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Child %d finished with status %d\n"),
                    child.getpid (), child_status));

      ACE_END_TEST;
    }
#endif /* ! ACE_LACKS_FORK */

  return 0;
}
