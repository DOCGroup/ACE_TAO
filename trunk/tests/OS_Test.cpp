// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This simple test exercises and illustrates use of OS wrapper functions.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "ace/OS.h"
#include "test_config.h"

ACE_RCSID(tests, OS_Test, "$Id$")

// Test ACE_OS::rename to be sure the files come and go as expected.
int
rename_test (void)
{
#if defined (ACE_LACKS_RENAME)
  ACE_ERROR_RETURN ((LM_INFO,
                     ACE_TEXT ("rename not supported on this platform\n")),
                    0);
#else
  ACE_TCHAR old_file[MAXPATHLEN];
  ACE_TCHAR new_file[MAXPATHLEN];
  ACE_OS::strcpy (old_file, ACE_TEXT ("rename_test_old"));
  ACE_OS::strcpy (new_file, ACE_TEXT ("rename_test_new"));

  // Test 1. Rename old to new when new already exists.
  // To set up, create two files, old and new. Both get opened and truncated
  // in case they're left over from a previous run. The first one (old) gets
  // something written in it so it's non-zero length - this is how the rename
  // is verified.
  FILE *f = ACE_OS::fopen (old_file, ACE_TEXT ("w+"));
  if (f == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s: %p\n"),
                       old_file,
                       ACE_TEXT ("fopen")),
                      -1);
  // Write something in the old_file so it has non-zero length
  ACE_OS::fputs (ACE_TEXT ("this is a test\n"), f);
  ACE_OS::fclose (f);
  f = ACE_OS::fopen (new_file, ACE_TEXT ("w+"));
  if (f == 0)
    {
      ACE_OS::unlink (old_file);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s: %p\n"),
                         new_file,
                         ACE_TEXT ("fopen")),
                        -1);
    }
  ACE_OS::fclose (f);

  if (ACE_OS::rename (old_file, new_file) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("rename test 1")));
      ACE_OS::unlink (old_file);
      ACE_OS::unlink (new_file);
      return -1;
    }
  // Verify that the old file was really renamed.
  struct stat checking;
  int result = 0;
  if (ACE_OS::stat (new_file, &checking) == -1 || checking.st_size == 0)
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename test 1: new_file not correct\n")));
    }
  if (ACE_OS::stat (old_file, &checking) == 0)
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename test 1: old_file still there\n")));
    }
  if (result == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Rename when dest. exists: success\n")));

  // Now test 2 - rename when the new file does not exist. If test 1 worked,
  // the old_file is now new_file and there is no old_file.
  if (ACE_OS::rename (new_file, old_file) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("rename test 2")));
      ACE_OS::unlink (old_file);
      ACE_OS::unlink (new_file);
      return -1;
    }
  if (ACE_OS::stat (old_file, &checking) == -1 || checking.st_size == 0)
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename test 2: new_file not correct\n")));
    }
  else if (ACE_OS::stat (new_file, &checking) == 0)
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename test 2: old_file still there\n")));
    }
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Rename when dest. gone: success\n")));

  ACE_OS::unlink (new_file);
  ACE_OS::unlink (old_file);

  // Test 3: It should fail... there are no files.
  if (ACE_OS::rename (old_file, new_file) == -1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%p\n"), ACE_TEXT ("Rename should bomb, and did")));
  else
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename expected fail, but succeeded\n")));
    }

  return result;
#endif /* ACE_LACKS_RENAME */
}


int 
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("OS_Test"));

  int status = 0;
  int result;

  if ((result = rename_test ()) != 0)
    status = result;

  ACE_END_TEST;
  return status;
}
