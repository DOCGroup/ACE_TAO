// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Mutex_Test.cpp
//
// = DESCRIPTION
//    Tests an <ACE_Process_Mutex> shared between multiple child processes.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, Process_Mutex_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if !defined (ACE_LACKS_FORK)
static int release_mutex = 1;
static int child_process = 0;
static const char *mutex_name = ACE_DEFAULT_MUTEX_A;
#if defined (__Lynx__)
static const u_int n_processes = 4;
#else  /* ! __Lynx__ */
static const u_int n_processes = ACE_MAX_PROCESSES;
#endif /* ! __Lynx__ */

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("usage: %n [-d (don't release mutex)] ")
              ASYS_TEXT ("[-c (child process)] [-n mutex name]\n")));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dcn:");

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'd':
      release_mutex = 0;
      break;
    case 'c':
      child_process = 1;
      break;
    case 'n':
      mutex_name = get_opt.optarg;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

static void
acquire_release (void)
{
  ACE_Process_Mutex mutex (ACE_WIDE_STRING (mutex_name));

  // Make sure the constructor succeeded
  ACE_ASSERT (ACE_LOG_MSG->op_status () == 0);

  // Grab the lock
  ACE_ASSERT (mutex.acquire () == 0);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P) Mutex acquired %s\n"),
              mutex_name));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P) Working....\n")));

  // Do some "work", i.e., just sleep for a couple of seconds.
  ACE_OS::sleep (2);

  // Check if we need to release the mutex
  if (release_mutex == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P) Releasing the mutex %s\n"),
                  mutex_name));
      ACE_ASSERT (mutex.release () == 0);
    }
}
#endif /* ! ACE_LACKS_FORK */

int
main (int argc, ASYS_TCHAR *argv[])
{
#if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ASYS_TEXT ("Process_Mutex_Test"));
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("fork is not supported on this platform\n")));
  ACE_END_TEST;
#else  /* ! ACE_LACKS_FORK */

  parse_args (argc, argv);

  // Child process code.
  if (child_process)
    {
      ACE_APPEND_LOG ("Process_Mutex_Test-children");
      acquire_release ();
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ASYS_TEXT ("Process_Mutex_Test"));
      ACE_INIT_LOG ("Process_Mutex_Test-children");

      ACE_Process_Options options;
      if (release_mutex == 0)
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
                              ACE_TEXT (" -c -n %s -d"),
                              ACE_WIDE_STRING (mutex_name));
      else
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
                              ACE_TEXT (" -c -n %s"),
                              ACE_WIDE_STRING (mutex_name));

      // Spawn <n_processes> child processes that will contend for the
      // lock.
      ACE_Process children[n_processes];
      size_t i;

      for (i = 0;
           i < n_processes;
           i++)
        {
          // Spawn the child process.
          int result = children[i].spawn (options);
          ACE_ASSERT (result != -1);
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("Parent spawned child process with pid = %d.\n"),
                      children[i].getpid ()));

          // Give the newly spawned child process a chance to start...
          ACE_OS::sleep (1);
        }

      for (i = 0; i < n_processes; i++)
        {
          // Wait for the child processes we created to exit.
          ACE_ASSERT (children[i].wait () != -1);
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("Parent %d finished\n"),
                      children[i].getpid ()));
        }

      ACE_END_TEST;
    }
#endif /* ! ACE_LACKS_FORK */

  return 0;
}
