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
#include "ace/Process_Mutex.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, Process_Mutex_Test, "$Id$")

#if !defined (ACE_LACKS_FORK)
static int release_mutex = 1;
static int child_process = 0;
static const char *mutex_name = ACE_DEFAULT_MUTEX;
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
              ACE_TEXT ("usage: %n [-d (don't release mutex)] ")
              ACE_TEXT ("[-c (child process)] [-n mutex name]\n")));
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
      mutex_name = get_opt.opt_arg ();
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

static void
acquire_release (void)
{
  ACE_Process_Mutex mutex (ACE_TEXT_CHAR_TO_TCHAR (mutex_name));

  // Make sure the constructor succeeded
  ACE_ASSERT (ACE_LOG_MSG->op_status () == 0);

  // To see if we really are the only holder of the mutex below,
  // we'll try to create a file with exclusive access. If the file
  // already exists, we're not the only one holding the mutex.
  char mutex_check[MAXNAMLEN];
  ACE_OS::strcpy (mutex_check, mutex_name);
  ACE_OS::strcat (mutex_check, ACE_TEXT ("_checker"));

  // Grab the lock
  ACE_ASSERT (mutex.acquire () == 0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Mutex acquired %s\n"),
              mutex_name));

  ACE_HANDLE checker_handle = ACE_OS::open (mutex_check, O_CREAT | O_EXCL);
  if (checker_handle == ACE_INVALID_HANDLE)
    {
      ACE_ASSERT (errno != EEXIST);
      ACE_DEBUG ((LM_WARNING, ACE_TEXT ("(%P): %p\n"),
                  ACE_TEXT ("checker file open")));
    }
  else
    ACE_OS::close (checker_handle);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Working....\n")));

  // Do some "work", i.e., just sleep for a couple of seconds.
  ACE_OS::sleep (2);

  // Free up the check file for the next acquirer.
  ACE_OS::unlink (mutex_check);

  // Check if we need to release the mutex
  if (release_mutex == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) Releasing the mutex %s\n"),
                  mutex_name));
      ACE_ASSERT (mutex.release () == 0);
    }
}
#endif /* ! ACE_LACKS_FORK */

int
main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Process_Mutex_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("fork is not supported on this platform\n")));
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
      ACE_START_TEST (ACE_TEXT ("Process_Mutex_Test"));
      ACE_INIT_LOG ("Process_Mutex_Test-children");

      ACE_Process_Options options;
      if (release_mutex == 0)
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
                              ACE_TEXT (" -c -n %s -d"),
                              ACE_TEXT_CHAR_TO_TCHAR (mutex_name));
      else
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
                              ACE_TEXT (" -c -n %s"),
                              ACE_TEXT_CHAR_TO_TCHAR (mutex_name));

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
                      ACE_TEXT ("Parent spawned child process with pid = %d.\n"),
                      children[i].getpid ()));

          // Give the newly spawned child process a chance to start...
          ACE_OS::sleep (1);
        }

      for (i = 0; i < n_processes; i++)
        {
          ACE_exitcode child_status;
          // Wait for the child processes we created to exit.
          ACE_ASSERT (children[i].wait (&child_status) != -1);
          if (child_status == 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Child %d finished ok\n"),
                        children[i].getpid ()));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Child %d finished with status %d\n"),
                        children[i].getpid (), child_status));
        }

      ACE_END_TEST;
    }
#endif /* ! ACE_LACKS_FORK */

  return 0;
}
