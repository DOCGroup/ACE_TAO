// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Manual_Event_Test.cpp
//
// = DESCRIPTION
//    This test verifies the functionality of the <ACE_Manual_Event>
//    processshared implementation.
//
// = AUTHOR
//    Martin Corino <mcorino@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process.h"
#include "ace/Manual_Event.h"
#include "ace/Time_Value.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/os_include/os_dirent.h"

#if !defined (ACE_LACKS_FORK) && \
  (defined (ACE_WIN32) || \
   (defined (ACE_HAS_PTHREADS) && defined (_POSIX_THREAD_PROCESS_SHARED)) || \
   defined (ACE_USES_FIFO_SEM) || \
   (defined (ACE_HAS_POSIX_SEM) && defined (ACE_HAS_POSIX_SEM_TIMEOUT) && !defined (ACE_LACKS_NAMED_POSIX_SEM)))
static int iterations = 10;
static int child_process = 0;
static const char *event_ping_name = "ACE_Ping_Event";
static const char *event_pong_name = "ACE_Pong_Event";

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
  ACE_Manual_Event event_ping (0, USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (event_ping_name));
  ACE_Manual_Event event_pong (0, USYNC_PROCESS, ACE_TEXT_CHAR_TO_TCHAR (event_pong_name));

  // Make sure the constructor succeeded
  ACE_ASSERT (ACE_LOG_MSG->op_status () == 0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Begin ping-pong\n")));

  if (child_process)
  {
    for (int i=0; i<iterations ;++i)
    {
      event_ping.signal ();

      if (event_pong.wait ())
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Failed acquiring pong (%p)\n")));
      else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P) Pong\n")));
        event_pong.reset ();
      }
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Testing timeouts\n")));

    // test timed wait
    ACE_Time_Value wait = ACE_OS::gettimeofday ();
    wait.sec (wait.sec () + 3); // timeout in 3 secs

    if (event_pong.wait (&wait))
        ACE_ASSERT(errno == ETIME);
    else
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Acquired pong without release()\n")));

    event_ping.signal ();   // release waiting parent before timeout
  }
  else
  {
    for (int i=0; i<iterations ;++i)
    {
      if (event_ping.wait ())
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Failed acquiring ping (%p)\n")));
      else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P) Ping\n")));
        event_ping.reset ();
      }

      event_pong.signal ();
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Testing timeouts\n")));

    // test timed wait
    ACE_Time_Value wait = ACE_OS::gettimeofday ();
    wait.sec (wait.sec () + 10); // timeout in 10 secs

    if (event_ping.wait (&wait))
    {
      ACE_ASSERT(errno == ETIME);
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P) Acquiring pong timed out\n")));
    }
  }
}
#endif /* ! ACE_LACKS_FORK */

int
run_main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Process_Manual_Event_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("fork is not supported on this platform\n")));
  ACE_END_TEST;
#elif defined (ACE_WIN32) || \
       (defined (ACE_HAS_PTHREADS) && defined (_POSIX_THREAD_PROCESS_SHARED)) || \
       defined (ACE_USES_FIFO_SEM) || \
       (defined (ACE_HAS_POSIX_SEM) && defined (ACE_HAS_POSIX_SEM_TIMEOUT) && !defined (ACE_LACKS_NAMED_POSIX_SEM))

  parse_args (argc, argv);

  // Child process code.
  if (child_process)
    {
      ACE_START_TEST (ACE_TEXT ("Process_Manual_Event_Test-child"));
      acquire_release ();
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("Process_Manual_Event_Test"));

      ACE_Process_Options options;
      options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                            ACE_TEXT ("Process_Manual_Event_Test")
                            ACE_PLATFORM_EXE_SUFFIX
                            ACE_TEXT (" -c -i %d"),
                            iterations);

      // Spawn a child process that will contend for the
      // lock.
      ACE_Process child;

      // Spawn the child process.
      int result = child.spawn (options);
      ACE_ASSERT (result != -1);
      ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Parent spawned child process with pid = %d.\n"),
                child.getpid ()));

      // start test
      acquire_release ();

      ACE_exitcode child_status;
      // Wait for the child processes we created to exit.
      ACE_ASSERT (child.wait (&child_status) != -1);
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
#else /* !ACE_LACKS_FORK */
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Process_Manual_Event_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Process shared events are not supported on this platform\n")));
  ACE_END_TEST;
#endif /* ! ACE_LACKS_FORK  */

  return 0;
}
