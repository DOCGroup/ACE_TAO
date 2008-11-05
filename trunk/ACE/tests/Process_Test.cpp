// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Test.cpp
//
// = DESCRIPTION
//    Tests ACE_Process file handle inheritance for UNIX-like systems
//
// = AUTHOR
//    Christian Fromme <kaner@strace.org>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Dirent.h"
#include "ace/Vector_T.h"
#include "ace/SString.h"
#include "ace/Process_Semaphore.h"

ACE_RCSID(tests, Process_Test, "Process_Test.cpp,v 4.11 1999/09/02 04:36:30 schmidt Exp")

// This will only work on UNIX-like with /proc filesys.
static const char *proc_self_fd = "/proc/self/fd/";

int
check_temp_file (const ACE_CString &tmpfilename)
{
  ACE_DIRENT *dir = 0;
  ACE_Dirent entr;
  ACE_stat stat;
  char filename[MAXPATHLEN + 1];

  ACE_OS::memset (&stat, 0, sizeof (stat));
  ACE_OS::memset (&entr, 0, sizeof (entr));

  // Loop through /proc/self/fs/
  if (entr.open (proc_self_fd) == -1) 
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Could not open dir %s\n"),
                         proc_self_fd),
                         -1);

  while ((dir = entr.read ())) 
    {
      ACE_CString fullp = ACE_CString (proc_self_fd) 
        + ACE_CString (dir->d_name);

      if ((ACE_OS::lstat (fullp.c_str (), &stat)) == -1) 
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Stat failed for %s\n"),
                           fullp.c_str ()),
                          -1);

      if (S_ISLNK (stat.st_mode)) 
        {
          ssize_t size = 0;
          if ((size= ACE_OS::readlink (fullp.c_str (), 
                                       filename, 
                                       MAXPATHLEN + 1)) == -1) 
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Readlink failed for %s\n"),
                               fullp.c_str ()),
                              -1);
          filename[size] = '\0';
          if (tmpfilename == ACE_CString (filename))
            return 1;
        }
    }

  return 0;
}

int
run_parent (bool inherit_files)
{
  ACE_TCHAR t[] = "ace_testXXXXXX";

  // Create tempfile. This will be tested for inheritance.
  ACE_TCHAR tempfile[MAXPATHLEN + 1];

  if (ACE::get_temp_dir (tempfile, MAXPATHLEN - sizeof (t)) == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT ("Could not get temp dir\n")), 
                      -1);

  ACE_OS::strcat (tempfile, t);

  int result = ACE_OS::mkstemp (tempfile);
  if (result == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT ("Could not get temp filename\n")), 
                      -1);

  ACE_HANDLE file_handle = ACE_OS::open (tempfile, 
                                         O_RDONLY|O_CREAT, 
                                         ACE_DEFAULT_FILE_PERMS);
  if (file_handle == ACE_INVALID_HANDLE) 
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Could not open temp file: %s\n"),
                       ACE_OS::strerror (ACE_OS::last_error ())),
                      -1);

  // Build child options
  ACE_Process_Options options;
  options.command_line (ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
                        ACE_TEXT ("Process_Test")
                        ACE_PLATFORM_EXE_SUFFIX
                        ACE_TEXT (" -c -h %d -f %s"),
                        (int)inherit_files,
                        tempfile);
  options.handle_inheritance (inherit_files); /* ! */
 
  // Spawn child
  ACE_Process child;

  result = child.spawn (options);
  ACE_ASSERT (result != -1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Parent spawned child process with pid = %d.\n"),
              child.getpid ()));

  ACE_exitcode child_status;
  ACE_ASSERT (child.wait (&child_status) != -1);
  if (child_status == 0) 
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Child %d finished ok\n"),
                child.getpid ()));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Child %d finished with status %d\n"),
                child.getpid (), child_status));
  return child_status;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_LACKS_FORK) 
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
  ACE_Vector<ACE_CString> ofiles;
  ACE_CString temp_file_name;

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
      int mypid (ACE_OS::getpid ());
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
                    ACE_TEXT ("Handle inheritance test failed\n")));
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

