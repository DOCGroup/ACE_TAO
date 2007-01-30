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

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_errno.h"

ACE_RCSID(tests, OS_Test, "$Id$")

// Test ACE_OS::access() to be sure a file's existence is correctly noted.
int
access_test (void)
{
  int test_status = 0;

  int status = ACE_OS::access (ACE_TEXT ("missing_file.txt"), F_OK);
  if (status == -1)
    {
      if (errno == ENOTSUP)
        ACE_ERROR_RETURN ((LM_INFO,
                           ACE_TEXT ("ACE_OS::access() not supported\n")),
                          0);
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Missing file noted as present.\n")));
      test_status = -1;
    }

  return test_status;
}

// Test ACE_OS::rename to be sure the files come and go as expected.
int
rename_test (void)
{
#if defined (ACE_LACKS_RENAME) || defined (ACE_VXWORKS)
  // On VxWorks only some filesystem drivers support rename
  // and as we do not know which is used, skip the test here
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

#if defined (ACE_WIN32) && defined (ACE_LACKS_WIN32_MOVEFILEEX)
  // Can't rename if new_file exists already.
  ACE_OS::unlink (new_file);
#endif

  if (ACE_OS::rename (old_file, new_file) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("rename test 1")));
      ACE_OS::unlink (old_file);
      ACE_OS::unlink (new_file);
      return -1;
    }
  // Verify that the old file was really renamed.
  ACE_stat checking;
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
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Rename test 3 should bomb, and did.\n")));
  else
    {
      result = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Rename expected fail, but succeeded\n")));
    }

  return result;
#endif /* ACE_LACKS_RENAME */
}

//
int
string_emulation_test (void)
{
  {
    // ========================================================================
    // Test memchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing memchr\n")));

    const char *memchr1 = "abcdefghijklmnopqrstuvwxyz";

    ACE_ASSERT (ACE_OS::memchr (static_cast<const void *> (NULL),
                                'a',
                                0) == NULL);
    ACE_ASSERT (ACE_OS::memchr (memchr1, 'a', sizeof (memchr1)) != NULL);
    ACE_ASSERT (ACE_OS::memchr (memchr1, '1', sizeof (memchr1)) == NULL);

    // ========================================================================
    // Test strchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr\n")));

    const char *strchr1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (*ACE_OS::strchr (strchr1, 'h') == 'h');
    ACE_ASSERT (ACE_OS::strchr (strchr1, 'h') == strchr1 + 7);
    ACE_ASSERT (ACE_OS::strchr (strchr1, '1') == NULL);

    // ========================================================================
    // Test strrchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr\n")));

    const char *strrchr1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (*ACE_OS::strrchr (strrchr1, 'h') == 'h');
    ACE_ASSERT (ACE_OS::strrchr (strrchr1, 'h') == strrchr1 + 18);
    ACE_ASSERT (ACE_OS::strrchr (strrchr1, '1') == NULL);

    // ========================================================================
    // Test strcspn
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcspn\n")));

    const char *strcspn1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (ACE_OS::strcspn (strcspn1, "d") == 3);
    ACE_ASSERT (ACE_OS::strcspn (strcspn1, "abcdefghijk") == 0);

    // ========================================================================
    // Test strcasecmp
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp\n")));

    const char *strcasecmp1 = "stringf";
    const char *strcasecmp2 = "stringfe";  // An extra character
    const char *strcasecmp3 = "stringg";   // The last letter is higher
    const char *strcasecmp4 = "STRINGF";   // Different case
    const char *strcasecmp5 = "stringe";   // The last letter is lower

    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    // ========================================================================
    // Test strtok_r
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strtok_r\n")));

    char strtok_r1[] = "A string of tokens";
    char *strtok_r2;

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok_r (strtok_r1,
                                                  " ",
                                                  &strtok_r2),
                                "A") == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "string") == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "of") == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "tokens") == 0);
    ACE_ASSERT (ACE_OS::strtok_r (0, " ", &strtok_r2) == 0);

    // ========================================================================
    // Test itoa
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa\n")));

    char itoa1[33];

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 2),
                                "101010") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 3),
                                "1120") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 16),
                                "2a") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (8, itoa1, 10),
                                "8") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (-8, itoa1, 10),
                                "-8") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (20345, itoa1, 10),
                                "20345") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (-20345, itoa1, 10),
                                "-20345") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (4566733, itoa1, 10),
                                "4566733") == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (-4566733, itoa1, 10),
                                "-4566733") == 0);
  }

#if defined (ACE_HAS_WCHAR)
  {
    // ========================================================================
    // Test itoa (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa (wchar_t version)\n")));

    wchar_t itow1[33];

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 2),
                                ACE_TEXT_WIDE ("101010")) == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 3),
                                ACE_TEXT_WIDE ("1120")) == 0);

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 16),
                                ACE_TEXT_WIDE ("2a")) == 0);


    // ========================================================================
    // Test strcmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcmp (wchar_t version)\n")));

    const wchar_t *strcmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS::strcmp (strcmp1, strcmp1) == 0);
    ACE_ASSERT (ACE_OS::strcmp (strcmp1, strcmp2) < 0);
    ACE_ASSERT (ACE_OS::strcmp (strcmp1, strcmp3) < 0);
    ACE_ASSERT (ACE_OS::strcmp (strcmp1, strcmp4) != 0);
    ACE_ASSERT (ACE_OS::strcmp (strcmp1, strcmp5) > 0);

    // ========================================================================
    // Test strcpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcpy (wchar_t version)\n")));

    const wchar_t *strcpy1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcpy2[27];

    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strcpy (strcpy2, strcpy1),
                       strcpy1) == 0);
    ACE_ASSERT (ACE_OS::strcmp (strcpy2, strcpy1) == 0);

    // ========================================================================
    // Test strcat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcat (wchar_t version)\n")));

    const wchar_t *strcat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strcat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyz");

    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strcat (strcat2, strcat3),
                       strcat1) == 0);
    ACE_ASSERT (ACE_OS::strcmp (strcat2, strcat1) == 0);

    // ========================================================================
    // Test strncat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncat (wchar_t version)\n")));

    const wchar_t *strncat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strncat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strncat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyzabc");

    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strncat (strncat2, strncat3, 14),
                       strncat1) == 0);
    ACE_ASSERT (ACE_OS::strcmp (strncat2, strncat1) == 0);

    // ========================================================================
    // Test strspn (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strspn (wchar_t version)\n")));

    const wchar_t *strspn1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS::strspn (strspn1,
                                ACE_TEXT_WIDE ("abcdf")) == 4);
    ACE_ASSERT (ACE_OS::strspn (strspn1,
                                ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr (wchar_t version)\n")));

    const wchar_t *strchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (*ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                == ACE_TEXT_WIDE ('h'));
    ACE_ASSERT (ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                == strchr1 + 7);
    ACE_ASSERT (ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('1')) == NULL);

    // ========================================================================
    // Test strstr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strstr (wchar_t version)\n")));

    const wchar_t *strstr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS::strncmp (
                  ACE_OS::strstr (strstr1, ACE_TEXT_WIDE ("def")),
                  ACE_TEXT_WIDE ("def"),
                  3)
                  == 0);
    ACE_ASSERT (ACE_OS::strstr (strstr1,
                                ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strlen (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strlen (wchar_t version)\n")));

    const wchar_t *strlen1 = ACE_TEXT_WIDE ("");
    const wchar_t *strlen2 = ACE_TEXT_WIDE ("12345");

    ACE_ASSERT (ACE_OS::strlen (strlen1) == 0);
    ACE_ASSERT (ACE_OS::strlen (strlen2) == 5);

    // ========================================================================
    // Test strpbrk (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strpbrk (wchar_t version)\n")));

    const wchar_t *strpbrk1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS::strpbrk (strpbrk1,  ACE_TEXT_WIDE ("ijkb"))
                  == strpbrk1 + 1);
    ACE_ASSERT (ACE_OS::strpbrk (strpbrk1,
                                        ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strrchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr (wchar_t version)\n")));

    const wchar_t *strrchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (*ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == ACE_TEXT_WIDE ('h'));
    ACE_ASSERT (ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == strrchr1 + 18);
    ACE_ASSERT (ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('1'))
                  == NULL);

    // ========================================================================
    // Test strcasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp (wchar_t version)\n")));

    const wchar_t *strcasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcasecmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    ACE_ASSERT (ACE_OS::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    // ========================================================================
    // Test strncasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncasecmp (wchar_t version)\n")));

    const wchar_t *strncasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncasecmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp2, 7) == 0);
    ACE_ASSERT
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp2, 8) < 0);
    ACE_ASSERT
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp3, 7) < 0);
    ACE_ASSERT
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp4, 7) == 0);
    ACE_ASSERT
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp5, 7) > 0);

    // ========================================================================
    // Test strncmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncmp (wchar_t version)\n")));

    const wchar_t *strncmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS::strncmp (strncmp1, strncmp2, 7) == 0);
    ACE_ASSERT (ACE_OS::strncmp (strncmp1, strncmp2, 8) < 0);
    ACE_ASSERT (ACE_OS::strncmp (strncmp1, strncmp3, 7) < 0);
    ACE_ASSERT (ACE_OS::strncmp (strncmp1, strncmp4, 7) != 0);
    ACE_ASSERT (ACE_OS::strncmp (strncmp1, strncmp5, 7) > 0);

    // ========================================================================
    // Test strncpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncpy (wchar_t version)\n")));

    wchar_t strncpy1[] = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyzabc");
    wchar_t strncpy2[27];

    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strncpy (strncpy2,
                                         strncpy1,
                                         26),
                        strncpy1,
                        26) == 0);

    strncpy1[26] = 0;
    strncpy2[26] = 0;
    ACE_ASSERT (ACE_OS::strcmp (strncpy2, strncpy1) == 0);

    // ========================================================================
    // Test strtok (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strtok (wchar_t version)\n")));
    wchar_t strtok_r1[] = ACE_TEXT_WIDE ("A string of tokens");

    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok (strtok_r1,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("A")) == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("string") ) == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("of") ) == 0);
    ACE_ASSERT (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("tokens") ) == 0);
    ACE_ASSERT (ACE_OS::strtok (0, ACE_TEXT_WIDE (" ")) == 0);


  }
#endif /* ACE_HAS_WCHAR */

  return 0;
}

// Test ACE_OS::snprintf
int
snprintf_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing snprintf\n")));

  int error_count = 0;
  const int BUFFER_SIZE = 4;
  char buf[2*BUFFER_SIZE];
  int retval;

  ACE_OS::memset(buf, 0xab, 2*BUFFER_SIZE);
  retval = ACE_OS::snprintf (buf, BUFFER_SIZE, "%d", 123);
  if (retval != 3)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[1] ACE_OS::snprintf() returns %d, should be 3\n"),
                  retval));
      ++error_count;
    }

  ACE_OS::memset(buf, 0xab, 2*BUFFER_SIZE);
  retval = ACE_OS::snprintf (buf, BUFFER_SIZE, "%d", 1234);

  // HP-UX has broken vsnprintf
#if !defined (HPUX)
  if (retval != 4)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[2] ACE_OS::snprintf() returns %d, should be 4\n"),
                  retval));
      ++error_count;
    }
#endif /* !HPUX */

  if (buf[3] != 0)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[3] ACE_OS::snprintf() doesn't terminate string correctly\n")));
      ++error_count;
    }
  else if (ACE_OS::strcmp(buf, "123") != 0)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[4] ACE_OS::snprintf() incorrect output\n")));
      ++error_count;
    }

  ACE_OS::memset(buf, 0xab, 2*BUFFER_SIZE);
  retval = ACE_OS::snprintf (buf, BUFFER_SIZE, "%d", 12345);
  if (retval != 5)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[5] ACE_OS::snprintf() returns %d, should be 5\n"),
                  retval));
      ++error_count;
    }
  else if (buf[3] != 0)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[6] ACE_OS::snprintf() doesn't terminate string correctly\n")));
      ++error_count;
    }
  else if (ACE_OS::strcmp(buf, "123") != 0)
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_TEXT ("[6] ACE_OS::snprintf() incorrect output\n")));
      ++error_count;
    }

  return error_count;
}

static int
ctime_r_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing ctime_r\n")));

  int result = 0;

  // test 'normal' buffer
  ACE_TCHAR buf[27];
  buf[26] = 'Z';

  ACE_Time_Value cur_time =
    ACE_OS::gettimeofday ();

  time_t secs = cur_time.sec ();
  if (ACE_OS::ctime_r (&secs, buf, 26) == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("ctime_r with 26 bytes")));
      result = -1;
    }
  else if (buf[0] == '\0')
    {
      result = -1;

      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Truncated input buffer\n")));
    }
  else if (buf[26] != 'Z')
    {
      result = -1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Wrote past end of input buffer\n")));
    }

  // test small buffer - should not do anything unless 3rd arg is at least 26.
  if (result == 0)
    {
      ACE_TCHAR bufcheck[27];
      ACE_OS::strcpy (bufcheck, buf);
      if (ACE_OS::ctime_r (&secs, buf, 10) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ctime_r with short len returned %s\n"),
                      buf));
          result = -1;
        }
      else if (errno != ERANGE)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("ctime_r short; wrong error")));
          result = -1;
        }
      // Make sure it didn't scribble
      else if (ACE_OS::strcmp (buf, bufcheck) != 0)
      {
        result = -1;
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ctime_r short; got %s, expected %s\n"),
                    buf, bufcheck));
      }
    }

  return result;
}


int
string_strsncpy_test (void)
{
  {
    // Test strsncpy (char version)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing strsncpy (char version)\n")));

    char strsncpy1[] =  "abcdefghijklmnopqrstuvwxyzabc";
    char strsncpy2[36];

    // strsncpy() where the max. length doesn't matter
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // strsncpy() where the max length does matter
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          26),
                        strsncpy1,
                        25) == 0);

    // strsncpy1 and strsncpy2 are different size --> not equal
    ACE_ASSERT (ACE_OS::strcmp (strsncpy2, strsncpy1) != 0);

    // max. length == 2 --> 1 char available
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          2),
                        strsncpy1,
                        1) == 0);

    // max length == 1 --> empty string
    ACE_ASSERT
      (ACE_OS::strlen (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         1)) == 0);

    // just preparation for the next assert
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // A tricky one, if the max. length == 0 --> do nothing
    // so the strsncpy2 shouldn't change
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         "test",
                                         0),
                       strsncpy1) == 0);

    // If src == dst --> truncate dst if needed!
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy2,
                                          10),
                        strsncpy1,
                        9) == 0);
    // size should be 9 (+ '\0' char)
    ACE_ASSERT(ACE_OS::strlen(strsncpy2) == 9);

  }

#if defined (ACE_HAS_WCHAR)
  {
    // Test strsncpy (wchar_t version)
     ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing strsncpy (wchar_t version)\n")));

    wchar_t strsncpy1[] = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyzabc");
    wchar_t strsncpy2[36];

    // strsncpy() where the max. length doesn't matter
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // strsncpy() where the max length does matter
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          26),
                        strsncpy1,
                        25) == 0);

    // strsncpy1 and strsncpy2 are different size --> not equal
    ACE_ASSERT (ACE_OS::strcmp (strsncpy2, strsncpy1) != 0);

    // max. length == 2 --> 1 char available
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          2),
                        strsncpy1,
                        1) == 0);

    // max length == 1 --> empty string
    ACE_ASSERT
      (ACE_OS::strlen (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         1)) == 0);

    // just preparation for the next assert
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // A tricky one, if the max. length == 0 --> do nothing
    // so the strsncpy2 shouldn't change
    ACE_ASSERT
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         ACE_TEXT_WIDE
                                         ("test"),
                                         0),
                       strsncpy1) == 0);

    // If src == dst --> truncate dst if needed!
    ACE_ASSERT
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy2,
                                          10),
                        strsncpy1,
                        9) == 0);
    // size should be 9 (+ '\0' char)
    ACE_ASSERT(ACE_OS::strlen(strsncpy2) == 9);
  }
#endif /* ACE_HAS_WCHAR */

  return 0;
}


// Test conversion between narrow and wide chars.
int
string_convert_test (void)
{
#if defined (ACE_HAS_WCHAR)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing narrow/wide string conversion\n")));

  int result = 0;
  const char *test1_n = "abcdefg";
  const wchar_t *test1_w = ACE_TEXT_WIDE ("abcdefg");
  const char *test2_n = "\xe9\xe8\xe0\xf9\xea";
  const wchar_t *test2_w = ACE_TEXT_WIDE ("\xe9\xe8\xe0\xf9\xea");
  wchar_t str_w[10];
  char str_n[10];
  ACE_OS::strcpy (str_w, ACE_Ascii_To_Wide (test1_n).wchar_rep ());
  if (0 != ACE_OS::strcmp (test1_w, str_w))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Simple narrow->wide failed: ")
                  ACE_TEXT ("Expected \"%W\"; Got \"%W\"\n"), test1_w, str_w));
      result = 1;
    }
  ACE_OS::strcpy (str_n, ACE_Wide_To_Ascii (test1_w).char_rep ());
  if (0 != ACE_OS::strcmp (test1_n, str_n))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Simple wide->narrow failed: ")
                  ACE_TEXT ("Expected \"%C\"; Got \"%C\"\n"), test1_n, str_n));
      result = 1;
    }
  ACE_OS::strcpy (str_w, ACE_Ascii_To_Wide (test2_n).wchar_rep ());
  if (0 != ACE_OS::strcmp (test2_w, str_w))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Complex narrow->wide failed: ")
                  ACE_TEXT ("Expected \"%W\"; Got \"%W\"\n"), test2_w, str_w));
      result = 1;
    }
  ACE_OS::strcpy (str_n, ACE_Wide_To_Ascii (test2_w).char_rep ());
  if (0 != ACE_OS::strcmp (test2_n, str_n))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Complex wide->narrow failed: ")
                  ACE_TEXT ("Expected \"%C\"; Got \"%C\"\n"), test2_n, str_n));
      result = 1;
    }
  return result;
#else
  return 0;
#endif /* ACE_HAS_WCHAR */
}

// Test the methods for getting cpu info
int
cpu_info_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing cpu info methods\n")));

  long number_processors = ACE_OS::num_processors();
  long number_processors_online = ACE_OS::num_processors_online();

  if (number_processors == -1)
    {
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("number of processors not supported on ")
                  ACE_TEXT ("this platform\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("This system has %d processors\n"),
                  number_processors));
    }

  if (number_processors_online == -1)
    {
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("number of processors online not supported on ")
                  ACE_TEXT ("this platform\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("This system has %d processors online\n"),
                  number_processors_online));
    }

  return 0;
}

int
pagesize_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing getpagesize method\n")));

  long const pagesize = ACE_OS::getpagesize ();
  if (pagesize <= 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, pagesize should return a value bigger ")
                            ACE_TEXT ("then zero, it returned %d\n"), pagesize));
      return 1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Pagesize returned %d\n"),
                  pagesize));
    }
  return 0;
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("OS_Test"));

  int status = 0;
  int result;

  if ((result = access_test ()) != 0)
    status = result;

  if ((result = rename_test ()) != 0)
    status = result;

  if ((result = string_emulation_test ()) != 0)
    status = result;

#if !defined (ACE_LACKS_VSNPRINTF) || defined (ACE_HAS_TRIO)
  if ((result = snprintf_test ()) != 0)
    status = result;
#endif /* !ACE_LACKS_VSNPRINTF || ACE_HAS_TRIO */

  if ((result = ctime_r_test ()) != 0)
    status = result;

  if ((result = string_strsncpy_test ()) != 0)
      status = result;

  if ((result = cpu_info_test ()) != 0)
      status = result;

  if ((result = pagesize_test ()) != 0)
      status = result;

  ACE_END_TEST;
  return status;
}
