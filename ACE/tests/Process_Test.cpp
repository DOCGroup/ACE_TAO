
//=============================================================================
/**
 *  @file    Process_Test.cpp
 *
 *  $Id$
 *
 *  Tests ACE_Process file handle inheritance for UNIX-like systems
 *
 *
 *  @author Christian Fromme <kaner@strace.org>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Dirent.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdlib.h"



// This will only work on Linux. Even UNIX-ish with /proc filesys lacks the
// 'self' level and link to the opened file name.
static const char *proc_self_fd = "/proc/self/fd/";

int
check_temp_file (const ACE_TString &tmpfilename)
{
  ACE_DIRENT *dir = 0;
  ACE_Dirent entr;
  ACE_stat stat;
  char filename[MAXPATHLEN + 1];

  ACE_OS::memset (&stat, 0, sizeof (stat));
  ACE_OS::memset (&entr, 0, sizeof (entr));

  // Loop through /proc/self/fs/
  if (entr.open (ACE_TEXT_CHAR_TO_TCHAR(proc_self_fd)) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Could not open dir %C\n"),
                         proc_self_fd),
                         -1);

  while ((dir = entr.read ()))
    {
      ACE_CString fullp = proc_self_fd;
#if defined (ACE_HAS_TCHAR_DIRENT)
      fullp += ACE_TEXT_ALWAYS_CHAR(dir->d_name);
#else
      fullp += dir->d_name;
#endif

      if ((ACE_OS::lstat (fullp.c_str (), &stat)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Stat failed for %C\n"),
                           fullp.c_str ()),
                          -1);

      if (S_ISLNK (stat.st_mode))
        {
          ssize_t size = 0;
          if ((size= ACE_OS::readlink (fullp.c_str (),
                                       filename,
                                       MAXPATHLEN + 1)) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Readlink failed for %C\n"),
                               fullp.c_str ()),
                              -1);
          filename[size] = '\0';
          if (tmpfilename == ACE_TString (ACE_TEXT_CHAR_TO_TCHAR (filename)))
            return 1;
        }
    }

  return 0;
}

void
run_parent (bool inherit_files)
{
  ACE_TCHAR t[] = ACE_TEXT ("ace_testXXXXXX");

  // Create tempfile. This will be tested for inheritance.
  ACE_TCHAR tempfile[MAXPATHLEN + 1];

  if (ACE::get_temp_dir (tempfile, MAXPATHLEN - sizeof (t)) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could not get temp dir\n")));

  ACE_OS::strcat (tempfile, t);

  ACE_HANDLE file_handle = ACE_OS::mkstemp (tempfile);
  if (file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could not get temp filename\n")));

  // Build child options
  ACE_TString exe_sub_dir;
  const char *subdir_env = ACE_OS::getenv ("ACE_EXE_SUB_DIR");
  if (subdir_env)
    {
      exe_sub_dir = ACE_TEXT_CHAR_TO_TCHAR (subdir_env);
      exe_sub_dir += ACE_DIRECTORY_SEPARATOR_STR;
    }

  ACE_Process_Options options;
  options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                        ACE_TEXT ("%sProcess_Test")
                        ACE_PLATFORM_EXE_SUFFIX
                        ACE_TEXT (" -c -h %d -f %s"),
                        exe_sub_dir.c_str(),
                        (int)inherit_files,
                        tempfile);
  options.handle_inheritance (inherit_files); /* ! */

  // Spawn child
  ACE_Process child;

  pid_t result = child.spawn (options);
  if (result == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Parent could NOT spawn child process\n")));
  else
    ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Parent spawned child process with pid = %d.\n"),
              child.getpid ()));

  ACE_exitcode child_status;
  result = child.wait (&child_status);
  if (result == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could NOT wait on child process\n")));
  else if (child_status == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Child %d finished ok\n"),
                child.getpid ()));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Child %d finished with status %d\n"),
                child.getpid (), child_status));
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  // This test relies on the ability to get a list of open files for a process
  // and examine each file descriptor to see which file is open, matching
  // against an expected opened file name. Although most systems provide some
  // mechanism to do this, the code in this test uses Linux-specific
  // techniques. Thus, although it is possible to add the code for the
  // checks on, for example, HP-UX (pstat_getproc, pstat_getpathname) and
  // AIX (/proc is available, but there's no self and the fds are not links
  // to the opened file names), the code isn't here at present.
#if defined (ACE_LACKS_FORK) || defined (ACE_LACKS_READLINK) || !defined(linux)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Process_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("This test is not supported on this platform\n")));
  ACE_END_TEST;
#else
  int c = 0;
  int handle_inherit = 0; /* Disable inheritance by default */
  bool ischild = false;
  ACE_TString temp_file_name;

  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("ch:f:"));

  while ((c = getopt ()) != -1)
      switch ((char) c)
        {
        case 'c':
          ischild = true;
          break;
        case 'h':
          handle_inherit = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'f':
          temp_file_name = getopt.opt_arg ();
          break;
        default:
          // Don't forgive bad options
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Bad option\n")),
                            -1);
          break;
        }

  if (ischild)
    {
      ACE_TCHAR lognm[MAXPATHLEN];
      int const mypid (ACE_OS::getpid ());
      ACE_OS::sprintf(lognm, ACE_TEXT ("Process_Test-child-%d"), mypid);

      ACE_START_TEST (lognm);

      int result = check_temp_file (temp_file_name);
      // Check descriptor inheritance
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Could not retrieve open files\n")),
                          -1);
      else if (result == handle_inherit)
        result = 0;
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Handle inheritance test failed with ")
                    ACE_TEXT ("%d, expected %d\n"), result, handle_inherit));
      ACE_END_LOG;
      return result;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("Process_Test"));

      // Test handle inheritance set to true
      run_parent (true);

      // ... and set to false
      run_parent (false);

      ACE_END_TEST;
    }
#endif /* ! ACE_LACKS_FORK */

  return 0;
}
