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
#include "ace/Mutex.h"
#include "ace/Process.h"
#include "ace/Process_Mutex.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/os_include/os_dirent.h"

ACE_RCSID(tests, Process_Mutex_Test, "$Id$")

static int release_mutex = 1;
static int child_process = 0;
static const ACE_TCHAR *mutex_name = ACE_DEFAULT_MUTEX;
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
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dcn:"));

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
  ACE_Process_Mutex mutex (mutex_name);

  // Make sure the constructor succeeded
  ACE_ASSERT (ACE_LOG_MSG->op_status () == 0);

  // To see if we really are the only holder of the mutex below,
  // we'll try to create a file with exclusive access. If the file
  // already exists, we're not the only one holding the mutex.
  ACE_TCHAR mutex_check[MAXPATHLEN+1];
  ACE_OS::strncpy (mutex_check, mutex_name, MAXPATHLEN);
  ACE_OS::strncat (mutex_check, ACE_TEXT ("_checker"), MAXPATHLEN);

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

int
run_main (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  // Child process code.
  if (child_process)
    {
      ACE_TCHAR lognm[MAXPATHLEN];
      int mypid (ACE_OS::getpid ());
      ACE_OS::sprintf(lognm, ACE_TEXT ("Process_Mutex_Test-child-%d"), mypid);

      ACE_START_TEST (lognm);
      acquire_release ();
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("Process_Mutex_Test"));
#     if !defined( ACE_HAS_SYSV_IPC) || defined( ACE_USES_MUTEX_FOR_PROCESS_MUTEX )
      // When Process_Mutex is pthreads based, then the owner of mutex destroys it
      // in destructor. This may disturb the other processes which still uses the
      // mutex. It is safer then to hold the mutex in main process, and destroy it after
      // children finish. This is temporary solution, and in future pthread base
      // Process_Mutex shall control the destruction of mutex better.
      ACE_Process_Mutex mutex( mutex_name );
#     endif

      ACE_Process_Options options;
      if (release_mutex == 0)
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
#if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
                              ACE_TEXT (" -c -n %ls -d"),
#else
                              ACE_TEXT (" -c -n %s -d"),
#endif /* !ACE_WIN32 && ACE_USES_WCHAR */
                              mutex_name);
      else
        options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                              ACE_TEXT ("Process_Mutex_Test")
                              ACE_PLATFORM_EXE_SUFFIX
#if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
                              ACE_TEXT (" -c -n %ls"),
#else
                              ACE_TEXT (" -c -n %s"),
#endif /* !ACE_WIN32 && ACE_USES_WCHAR */
                              mutex_name);

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
          // David Levine thinks this sleep() is required because
          // calling ::waitpid () before a fork'ed child has actually
          // been created may be a problem on some platforms.  It's
          // not enough for fork() to have returned to the parent.
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

  return 0;
}
