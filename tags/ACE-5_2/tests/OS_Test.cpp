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

#if defined (ACE_WIN32) && defined (ACE_HAS_WINNT4) && ACE_HAS_WINNT4 == 0
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

    ACE_ASSERT (ACE_OS_String::memchr (ACE_static_cast (const void *, NULL),
                                       'a',
                                       0) == NULL);
    ACE_ASSERT (ACE_OS_String::memchr (memchr1, 'a', sizeof (memchr1)) != NULL);
    ACE_ASSERT (ACE_OS_String::memchr (memchr1, '1', sizeof (memchr1)) == NULL);

    // ========================================================================
    // Test strchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr\n")));

    const char *strchr1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (*ACE_OS_String::strchr (strchr1, 'h') == 'h');
    ACE_ASSERT (ACE_OS_String::strchr (strchr1, 'h') == strchr1 + 7);
    ACE_ASSERT (ACE_OS_String::strchr (strchr1, '1') == NULL);

    // ========================================================================
    // Test strrchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr\n")));

    const char *strrchr1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (*ACE_OS_String::strrchr (strrchr1, 'h') == 'h');
    ACE_ASSERT (ACE_OS_String::strrchr (strrchr1, 'h') == strrchr1 + 18);
    ACE_ASSERT (ACE_OS_String::strrchr (strrchr1, '1') == NULL);

    // ========================================================================
    // Test strcspn
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcspn\n")));

    const char *strcspn1 = "abcdefghijkabcdefghijk";

    ACE_ASSERT (ACE_OS_String::strcspn (strcspn1, "d") == 3);
    ACE_ASSERT (ACE_OS_String::strcspn (strcspn1, "abcdefghijk") == 0);

    // ========================================================================
    // Test strcasecmp
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp\n")));

    const char *strcasecmp1 = "stringf";
    const char *strcasecmp2 = "stringfe";  // An extra character
    const char *strcasecmp3 = "stringg";   // The last letter is higher
    const char *strcasecmp4 = "STRINGF";   // Different case
    const char *strcasecmp5 = "stringe";   // The last letter is lower

    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    // ========================================================================
    // Test strtok_r
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strtok_r\n")));

    char strtok_r1[] = "A string of tokens";
    char *strtok_r2;


    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::strtok_r (strtok_r1,
                                                                " ",
                                                                &strtok_r2),
                                       "A") == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::strtok_r (0,
                                                                " ",
                                                                &strtok_r2),
                                       "string") == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::strtok_r (0,
                                                                " ",
                                                                &strtok_r2),
                                       "of") == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::strtok_r (0,
                                                                " ",
                                                                &strtok_r2),
                                       "tokens") == 0);
    ACE_ASSERT (ACE_OS_String::strtok_r (0, " ", &strtok_r2) == 0);

    // ========================================================================
    // Test itoa
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa\n")));

    char itoa1[33];

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itoa1, 2),
                                       "101010") == 0);

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itoa1, 3),
                                       "1120") == 0);

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itoa1, 16),
                                       "2a") == 0);
  }

#if defined (ACE_HAS_WCHAR)
  {
    // ========================================================================
    // Test itoa (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa (wchar_t version)\n")));

    wchar_t itow1[33];

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itow1, 2),
                                       ACE_TEXT_WIDE ("101010")) == 0);

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itow1, 3),
                                       ACE_TEXT_WIDE ("1120")) == 0);

    ACE_ASSERT (ACE_OS_String::strcmp (ACE_OS_String::itoa (42, itow1, 16),
                                       ACE_TEXT_WIDE ("2a")) == 0);


    // ========================================================================
    // Test strcmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcmp (wchar_t version)\n")));

    const wchar_t *strcmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS_String::strcmp (strcmp1, strcmp1) == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcmp1, strcmp2) < 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcmp1, strcmp3) < 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcmp1, strcmp4) != 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcmp1, strcmp5) > 0);

    // ========================================================================
    // Test strcpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcpy (wchar_t version)\n")));

    const wchar_t *strcpy1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcpy2[27];

    ACE_ASSERT
      (ACE_OS_String::strcmp (ACE_OS_String::strcpy (strcpy2, strcpy1),
                              strcpy1) == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcpy2, strcpy1) == 0);

    // ========================================================================
    // Test strcat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcat (wchar_t version)\n")));

    const wchar_t *strcat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strcat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyz");

    ACE_ASSERT
      (ACE_OS_String::strcmp (ACE_OS_String::strcat (strcat2, strcat3),
                              strcat1) == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strcat2, strcat1) == 0);

    // ========================================================================
    // Test strncat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncat (wchar_t version)\n")));

    const wchar_t *strncat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strncat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strncat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyzabc");

    ACE_ASSERT
      (ACE_OS_String::strcmp (ACE_OS_String::strncat (strncat2, strncat3, 14),
                              strncat1) == 0);
    ACE_ASSERT (ACE_OS_String::strcmp (strncat2, strncat1) == 0);

    // ========================================================================
    // Test strspn (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strspn (wchar_t version)\n")));

    const wchar_t *strspn1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS_String::strspn (strspn1,
                                       ACE_TEXT_WIDE ("abcdf")) == 4);
    ACE_ASSERT (ACE_OS_String::strspn (strspn1,
                                       ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr (wchar_t version)\n")));

    const wchar_t *strchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (*ACE_OS_String::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                  == ACE_TEXT_WIDE ('h'));
    ACE_ASSERT (ACE_OS_String::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                  == strchr1 + 7);
    ACE_ASSERT (ACE_OS_String::strchr (strchr1, ACE_TEXT_WIDE ('1')) == NULL);

    // ========================================================================
    // Test strstr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strstr (wchar_t version)\n")));

    const wchar_t *strstr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS_String::strncmp (
                  ACE_OS_String::strstr (strstr1, ACE_TEXT_WIDE ("def")),
                  ACE_TEXT_WIDE ("def"),
                  3)
                  == 0);
    ACE_ASSERT (ACE_OS_String::strstr (strstr1,
                                       ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strlen (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strlen (wchar_t version)\n")));

    const wchar_t *strlen1 = ACE_TEXT_WIDE ("");
    const wchar_t *strlen2 = ACE_TEXT_WIDE ("12345");

    ACE_ASSERT (ACE_OS_String::strlen (strlen1) == 0);
    ACE_ASSERT (ACE_OS_String::strlen (strlen2) == 5);

    // ========================================================================
    // Test strpbrk (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strpbrk (wchar_t version)\n")));

    const wchar_t *strpbrk1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (ACE_OS_String::strpbrk (strpbrk1,  ACE_TEXT_WIDE ("ijkb"))
                  == strpbrk1 + 1);
    ACE_ASSERT (ACE_OS_String::strpbrk (strpbrk1,
                                        ACE_TEXT_WIDE ("mno")) == 0);

    // ========================================================================
    // Test strrchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr (wchar_t version)\n")));

    const wchar_t *strrchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    ACE_ASSERT (*ACE_OS_String::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == ACE_TEXT_WIDE ('h'));
    ACE_ASSERT (ACE_OS_String::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == strrchr1 + 18);
    ACE_ASSERT (ACE_OS_String::strrchr (strrchr1, ACE_TEXT_WIDE ('1'))
                  == NULL);

    // ========================================================================
    // Test strcasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp (wchar_t version)\n")));

    const wchar_t *strcasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcasecmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    ACE_ASSERT (ACE_OS_String::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    // ========================================================================
    // Test strncasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncasecmp (wchar_t version)\n")));

    const wchar_t *strncasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncasecmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT
      (ACE_OS_String::strncasecmp (strncasecmp1, strncasecmp2, 7) == 0);
    ACE_ASSERT
      (ACE_OS_String::strncasecmp (strncasecmp1, strncasecmp2, 8) < 0);
    ACE_ASSERT
      (ACE_OS_String::strncasecmp (strncasecmp1, strncasecmp3, 7) < 0);
    ACE_ASSERT
      (ACE_OS_String::strncasecmp (strncasecmp1, strncasecmp4, 7) == 0);
    ACE_ASSERT
      (ACE_OS_String::strncasecmp (strncasecmp1, strncasecmp5, 7) > 0);

    // ========================================================================
    // Test strncmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncmp (wchar_t version)\n")));

    const wchar_t *strncmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncmp5 = ACE_TEXT_WIDE ("stringe");

    ACE_ASSERT (ACE_OS_String::strncmp (strncmp1, strncmp2, 7) == 0);
    ACE_ASSERT (ACE_OS_String::strncmp (strncmp1, strncmp2, 8) < 0);
    ACE_ASSERT (ACE_OS_String::strncmp (strncmp1, strncmp3, 7) < 0);
    ACE_ASSERT (ACE_OS_String::strncmp (strncmp1, strncmp4, 7) != 0);
    ACE_ASSERT (ACE_OS_String::strncmp (strncmp1, strncmp5, 7) > 0);

    // ========================================================================
    // Test strncpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncpy (wchar_t version)\n")));

    wchar_t strncpy1[] = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyzabc");
    wchar_t strncpy2[27];

    ACE_ASSERT
      (ACE_OS_String::strncmp (ACE_OS_String::strncpy (strncpy2,
                                                       strncpy1,
                                                       26),
                              strncpy1,
                              26) == 0);

    strncpy1[26] = 0;
    strncpy2[26] = 0;
    ACE_ASSERT (ACE_OS_String::strcmp (strncpy2, strncpy1) == 0);

  }
#endif /* ACE_HAS_WCHAR */

  return 0;
}

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("OS_Test"));

  int status = 0;
  int result;

  if ((result = rename_test ()) != 0)
    status = result;

  if ((result = string_emulation_test ()) != 0)
    status = result;

  ACE_END_TEST;
  return status;
}
