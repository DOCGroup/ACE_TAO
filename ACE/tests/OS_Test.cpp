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
#include "ace/ACE.h"
//FUZZ: disable check_for_include_OS_h
#include "ace/OS.h"
//FUZZ: enable check_for_include_OS_h

#include "ace/OS_NS_math.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_netdb.h"

#include <clocale>
#include <cmath>
#include <limits>
#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
#define THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(X) \
  ((X)                                                          \
   ? static_cast<void>(0)                                       \
   : ACE_VERSIONED_NAMESPACE_NAME::__ace_assert(__FILE__, __LINE__, ACE_TEXT_CHAR_TO_TCHAR (#X)))

// Test ACE_OS::access() to be sure a file's existence is correctly noted.
int
access_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing access method\n")));

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
  ACE_OS::fwrite (ACE_TEXT ("this is a test\n"), sizeof (ACE_TCHAR), 15, f);
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

#if defined (ACE_WIN32) && defined (ACE_LACKS_WIN32_MOVEFILEEX) || defined (ACE_HAS_WINCE)
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
#endif /* ACE_VXWORKS */
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

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::memchr (static_cast<const void *> (0),
                                'a',
                                0) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::memchr (memchr1, 'a', sizeof (memchr1)) != 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::memchr (memchr1, '1', sizeof (memchr1)) == 0);

    // ========================================================================
    // Test strchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr\n")));

    const char *strchr1 = "abcdefghijkabcdefghijk";

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (*ACE_OS::strchr (strchr1, 'h') == 'h');
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strchr (strchr1, 'h') == strchr1 + 7);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strchr (strchr1, '1') == 0);

    // ========================================================================
    // Test strrchr
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr\n")));

    const char *strrchr1 = "abcdefghijkabcdefghijk";

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (*ACE_OS::strrchr (strrchr1, 'h') == 'h');
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strrchr (strrchr1, 'h') == strrchr1 + 18);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strrchr (strrchr1, '1') == 0);

    // ========================================================================
    // Test strcspn
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcspn\n")));

    const char *strcspn1 = "abcdefghijkabcdefghijk";

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcspn (strcspn1, "d") == 3);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcspn (strcspn1, "abcdefghijk") == 0);

    // ========================================================================
    // Test strcasecmp
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp\n")));

    const char *strcasecmp1 = "stringf";
    const char *strcasecmp2 = "stringfe";  // An extra character
    const char *strcasecmp3 = "stringg";   // The last letter is higher
    const char *strcasecmp4 = "STRINGF";   // Different case
    const char *strcasecmp5 = "stringe";   // The last letter is lower

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    // ========================================================================
    // Test strtok_r
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strtok_r\n")));

    char strtok_r1[] = "A string of tokens";
    char *strtok_r2;

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok_r (strtok_r1,
                                                  " ",
                                                  &strtok_r2),
                                "A") == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "string") == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "of") == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok_r (0,
                                                  " ",
                                                  &strtok_r2),
                                "tokens") == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strtok_r (0, " ", &strtok_r2) == 0);

    // ========================================================================
    // Test itoa
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa\n")));

    char itoa1[33];

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 2),
                                "101010") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 3),
                                "1120") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itoa1, 16),
                                "2a") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (8, itoa1, 10),
                                "8") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (-8, itoa1, 10),
                                "-8") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (20345, itoa1, 10),
                                "20345") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (-20345, itoa1, 10),
                                "-20345") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (4566733, itoa1, 10),
                                "4566733") == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (-4566733, itoa1, 10),
                                "-4566733") == 0);
  }

#if defined (ACE_HAS_WCHAR)
  {
    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test itoa (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing itoa (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    wchar_t itow1[33];

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 2),
                                ACE_TEXT_WIDE ("101010")) == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 3),
                                ACE_TEXT_WIDE ("1120")) == 0);

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::itoa (42, itow1, 16),
                                ACE_TEXT_WIDE ("2a")) == 0);


    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strcmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcmp (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strcmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcmp5 = ACE_TEXT_WIDE ("stringe");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcmp1, strcmp1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcmp1, strcmp2) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcmp1, strcmp3) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcmp1, strcmp4) != 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcmp1, strcmp5) > 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strcpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcpy (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strcpy1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcpy2[27];

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strcpy (strcpy2, strcpy1),
                       strcpy1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcpy2, strcpy1) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strcat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcat (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strcat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strcat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strcat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyz");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strcat (strcat2, strcat3),
                       strcat1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strcat2, strcat1) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strncat (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncat (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strncat1 = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyz");
    wchar_t strncat2[27] = ACE_TEXT_WIDE ("abcdefghijkl");
    const wchar_t *strncat3 = ACE_TEXT_WIDE ("mnopqrstuvwxyzabc");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strncat (strncat2, strncat3, 14),
                       strncat1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strncat2, strncat1) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strspn (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strspn (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strspn1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strspn (strspn1,
                                ACE_TEXT_WIDE ("abcdf")) == 4);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strspn (strspn1,
                                ACE_TEXT_WIDE ("mno")) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strchr (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (*ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                == ACE_TEXT_WIDE ('h'));
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('h'))
                == strchr1 + 7);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strchr (strchr1, ACE_TEXT_WIDE ('1')) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strstr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strstr (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strstr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (
                  ACE_OS::strstr (strstr1, ACE_TEXT_WIDE ("def")),
                  ACE_TEXT_WIDE ("def"),
                  3)
                  == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strstr (strstr1,
                                ACE_TEXT_WIDE ("mno")) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strlen (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strlen (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strlen1 = ACE_TEXT_WIDE ("");
    const wchar_t *strlen2 = ACE_TEXT_WIDE ("12345");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strlen (strlen1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strlen (strlen2) == 5);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strpbrk (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strpbrk (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strpbrk1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strpbrk (strpbrk1,  ACE_TEXT_WIDE ("ijkb"))
                  == strpbrk1 + 1);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strpbrk (strpbrk1,
                                        ACE_TEXT_WIDE ("mno")) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strrchr (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strrchr (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strrchr1 = ACE_TEXT_WIDE ("abcdefghijkabcdefghijk");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (*ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == ACE_TEXT_WIDE ('h'));
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('h'))
                  == strrchr1 + 18);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strrchr (strrchr1, ACE_TEXT_WIDE ('1'))
                  == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strcasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strcasecmp (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strcasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strcasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strcasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strcasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strcasecmp5 = ACE_TEXT_WIDE ("stringe");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp1) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp2) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp3) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp4) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcasecmp (strcasecmp1, strcasecmp5) > 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strncasecmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncasecmp (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strncasecmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncasecmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncasecmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncasecmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncasecmp5 = ACE_TEXT_WIDE ("stringe");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp2, 7) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp2, 8) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp3, 7) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp4, 7) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncasecmp (strncasecmp1, strncasecmp5, 7) > 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strncmp (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncmp (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    const wchar_t *strncmp1 = ACE_TEXT_WIDE ("stringf");
    const wchar_t *strncmp2 = ACE_TEXT_WIDE ("stringfe");
    const wchar_t *strncmp3 = ACE_TEXT_WIDE ("stringg");
    const wchar_t *strncmp4 = ACE_TEXT_WIDE ("STRINGF");
    const wchar_t *strncmp5 = ACE_TEXT_WIDE ("stringe");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (strncmp1, strncmp2, 7) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (strncmp1, strncmp2, 8) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (strncmp1, strncmp3, 7) < 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (strncmp1, strncmp4, 7) != 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strncmp (strncmp1, strncmp5, 7) > 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strncpy (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strncpy (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    wchar_t strncpy1[] = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyzabc");
    wchar_t strncpy2[27];

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strncpy (strncpy2,
                                         strncpy1,
                                         26),
                        strncpy1,
                        26) == 0);

    strncpy1[26] = 0;
    strncpy2[26] = 0;
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strncpy2, strncpy1) == 0);

    //FUZZ: disable check_for_lack_ACE_OS
    // ========================================================================
    // Test strtok (wchar_t version)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing strtok (wchar_t version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    wchar_t strtok_r1[] = ACE_TEXT_WIDE ("A string of tokens");

    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok (strtok_r1,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("A")) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("string") ) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("of") ) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (ACE_OS::strtok (0,
                                                ACE_TEXT_WIDE (" ")),
                                ACE_TEXT_WIDE ("tokens") ) == 0);
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strtok (0, ACE_TEXT_WIDE (" ")) == 0);


  }
#endif /* ACE_HAS_WCHAR */

  return 0;
}

// Test ACE_OS::snprintf
typedef int (*SNPrintF_t) (char *buf, size_t maxlen, const char *format, ...);

int
snprintf_test (SNPrintF_t fn)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing snprintf\n")));

  int error_count = 0;
  const int BUFFER_SIZE = 4;
  char buf[2*BUFFER_SIZE];
  int retval;

  ACE_OS::memset(buf, 0xab, 2*BUFFER_SIZE);
  retval = fn (buf, BUFFER_SIZE, "%d", 123);
  if (retval != 3)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[1] ACE_OS::snprintf() returns %d, should be 3\n"),
                  retval));
      ++error_count;
    }

  ACE_OS::memset(buf, 0xab, 2*BUFFER_SIZE);
  retval = fn (buf, BUFFER_SIZE, "%d", 1234);

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
  retval = fn (buf, BUFFER_SIZE, "%d", 12345);
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
getpwnam_r_test (void)
{
  int result = 0;

#if !defined (ACE_LACKS_PWD_FUNCTIONS)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing getpwnam_r\n")));

  struct passwd pwd;
  struct passwd *pwd_ptr;
  char buf[1024];

  const char* login = getlogin ();
  if (login == 0)
    login = "root";

  if (ACE_OS::getpwnam_r (login,
                          &pwd,
                          buf,
                          sizeof (buf),
                          &pwd_ptr) != 0)
    {
      result = 1;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("getpwnam_r() failed\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" User '%s' has uid=%d and gid=%d\n"),
                  pwd_ptr->pw_name, pwd_ptr->pw_uid, pwd_ptr->pw_gid));
    }
#endif

  return result;
}

static int
compiler_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing compiler methods\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using compiler %s with major version %d minor version %d beta version %d\n"),
    ACE::compiler_name(),
    ACE::compiler_major_version(),
    ACE::compiler_minor_version (),
    ACE::compiler_beta_version ()));

  return 0;
}

static int
version_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing version macros\n")));

  int code = ACE_MAKE_VERSION_CODE(ACE_MAJOR_VERSION, ACE_MINOR_VERSION, ACE_MICRO_VERSION);
  bool run_time_check = code == ACE_VERSION_CODE;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ACE release time version code: %d, runtime version code: %d, %s\n"),
              ACE_VERSION_CODE, code, run_time_check ? ACE_TEXT ("OK") : ACE_TEXT ("FAIL")));

  // Compile time check. Check we have ACE version 6.x
#if ACE_VERSION_CODE > ACE_MAKE_VERSION_CODE(5, 88, 99)
  bool compile_time_check = true;
#else
  bool compile_time_check = false;
#endif

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Compile time version check, %s\n"),
              compile_time_check ? ACE_TEXT ("OK") : ACE_TEXT ("FAIL")));

  if(run_time_check && compile_time_check)
    return 0;
  return 1;
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
    //FUZZ: disable check_for_lack_ACE_OS
    // Test strsncpy (char version)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing strsncpy (char version)\n")));
    //FUZZ: enable check_for_lack_ACE_OS

    char strsncpy1[] =  "abcdefghijklmnopqrstuvwxyzabc";
    char strsncpy2[36];

    // strsncpy() where the max. length doesn't matter
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // strsncpy() where the max length does matter
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          26),
                        strsncpy1,
                        25) == 0);

    // strsncpy1 and strsncpy2 are different size --> not equal
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strsncpy2, strsncpy1) != 0);

    // max. length == 2 --> 1 char available
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          2),
                        strsncpy1,
                        1) == 0);

    // max length == 1 --> empty string
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strlen (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         1)) == 0);

    // just preparation for the next assert
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // A tricky one, if the max. length == 0 --> do nothing
    // so the strsncpy2 shouldn't change
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         "test",
                                         0),
                       strsncpy1) == 0);

    // If src == dst --> truncate dst if needed!
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy2,
                                          10),
                        strsncpy1,
                        9) == 0);
    // size should be 9 (+ '\0' char)
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(ACE_OS::strlen(strsncpy2) == 9);

  }

#if defined (ACE_HAS_WCHAR)
  {
    //FUZZ: disable check_for_lack_ACE_OS
    // Test strsncpy (wchar_t version)
     ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Testing strsncpy (wchar_t version)\n")));
     //FUZZ: enable check_for_lack_ACE_OS

    wchar_t strsncpy1[] = ACE_TEXT_WIDE ("abcdefghijklmnopqrstuvwxyzabc");
    wchar_t strsncpy2[36];

    // strsncpy() where the max. length doesn't matter
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // strsncpy() where the max length does matter
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          26),
                        strsncpy1,
                        25) == 0);

    // strsncpy1 and strsncpy2 are different size --> not equal
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (ACE_OS::strcmp (strsncpy2, strsncpy1) != 0);

    // max. length == 2 --> 1 char available
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy1,
                                          2),
                        strsncpy1,
                        1) == 0);

    // max length == 1 --> empty string
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strlen (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         1)) == 0);

    // just preparation for the next assert
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         strsncpy1,
                                         36),
                       strsncpy1) == 0);

    // A tricky one, if the max. length == 0 --> do nothing
    // so the strsncpy2 shouldn't change
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strcmp (ACE_OS::strsncpy (strsncpy2,
                                         ACE_TEXT_WIDE
                                         ("test"),
                                         0),
                       strsncpy1) == 0);

    // If src == dst --> truncate dst if needed!
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
      (ACE_OS::strncmp (ACE_OS::strsncpy (strsncpy2,
                                          strsncpy2,
                                          10),
                        strsncpy1,
                        9) == 0);
    // size should be 9 (+ '\0' char)
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(ACE_OS::strlen(strsncpy2) == 9);
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

// Test ACE_OS::strsignal()
int
strsignal_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing strsignal method\n")));

  int test_status = 0;

  const char* result = 0;

  for (int i=-1; i < (ACE_NSIG + 1); ++i)
    {
      result = ACE_OS::strsignal (i);
      if (result == 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("strsignal returned null\n")));
          test_status = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" Sig #%d: %C\n"), i, result));
        }
    }

  return test_status;
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

  if ((number_processors_online != -1 && number_processors != -1) &&
      number_processors_online > number_processors)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%d online processors; should be <= %d\n"),
                       number_processors_online,
                       number_processors),
                      -1);

  return 0;
}

int
last_error_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing last_error method\n")));

  ACE_OS::last_error (ETIME);

  int const l_error = ACE_OS::last_error ();
  if (l_error != ETIME)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Last error returned %d instead of ETIME"),
                            l_error));
      return 1;
    }

  ACE_OS::last_error (0);

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
ace_ctype_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ace ctype methods\n")));

  int retval = 0;
  int result = ACE_OS::ace_isprint (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isprint should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isblank (ACE_TEXT('\t'));
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isblank should return != 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isblank (ACE_TEXT(' '));
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isblank should return != 0 for space ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isalpha (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isalpha should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isupper (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isupper should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_islower (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_islower should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isdigit (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isdigit should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isxdigit (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isxdigit should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isspace (ACE_TEXT('\t'));
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isspace should return != 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_ispunct (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_ispunct should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isalnum (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isalnum should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isgraph (ACE_TEXT('\t'));
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isgraph should return 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_iscntrl (ACE_TEXT('\t'));
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_iscntrl should return != 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }
  result = ACE_OS::ace_isascii (ACE_TEXT('\t'));
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, ace_isascii should return != 0 for tab ")
                            ACE_TEXT ("but it returned %d\n"), result));
      ++retval;
    }

  return 0;
}

int
ceilf_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ceilf method\n")));

  float values[]  = {-2.5, -1.5, 1.5, 2.5};
  float results[] = {-2.0, -1.0, 2.0, 3.0};
  float result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (float) ; i++)
    {
      result = ACE_OS::ceil (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ceilf error: input %.1F, output %1F, expected %1F\n"),
                      values [i],
                      result,
                      results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
floorf_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing floorf method\n")));

  float values[]  = {-2.5, -1.5, 1.5, 2.5};
  float results[] = {-3.0, -2.0, 1.0, 2.0};
  float result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (float) ; i++)
    {
      result = ACE_OS::floor (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("floorf error: input %.1F, output %1F, expected %1F\n"),
                      values [i], result, results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
ceil_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ceil method\n")));

  double values[]  = {-2.5, -1.5, 1.5, 2.5};
  double results[] = {-2.0, -1.0, 2.0, 3.0};
  double result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (double) ; i++)
    {
      result = ACE_OS::ceil (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ceil error: input %.1F, output %1F, expected %1F\n"),
                      values [i],
                      result,
                      results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
floor_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing floor method\n")));

  double values[]  = {-2.5, -1.5, 1.5, 2.5};
  double results[] = {-3.0, -2.0, 1.0, 2.0};
  double result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (double) ; i++)
    {
      result = ACE_OS::floor (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("floor error: input %.1F, output %1F, expected %1F\n"),
                      values [i],
                      result,
                      results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
ceill_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing ceill method\n")));

  long double values[]  = {-2.5, -1.5, 1.5, 2.5};
  long double results[] = {-2.0, -1.0, 2.0, 3.0};
  long double result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (long double) ; i++)
    {
      result = ACE_OS::ceil (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ceil error: input %.1F, output %1F, expected %1F\n"),
                      values [i],
                      result,
                      results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
floorl_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing floorl method\n")));

  long double values[]  = {-2.5, -1.5, 1.5, 2.5};
  long double results[] = {-3.0, -2.0, 1.0, 2.0};
  long double result = 0.0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (long double) ; i++)
    {
      result = ACE_OS::floor (values [i]);
      if (ACE::is_inequal(result, results[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("floor error: input %.1F, output %1F, expected %1F\n"),
                      values [i], result, results [i]));
          ++error_count;
        }
    }

  return error_count;
}

int
log2_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing log2 method\n")));

  double values[] = {1.0, 2.0, 4.0, 8.0, 1048576.0};
  int const results[] = {0, 1, 2, 3, 20};
  int result = 0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (double) ; i++)
    {
      result = static_cast<int> (ACE_OS::log2 (values [i]) + 0.5);
      if (result != results [i])
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Log2 error: input %.1F, output %d, expected %d\n"), values [i], result, results [i]));
          ++error_count;
        }
    }

  return error_count;
}

#if defined (ACE_HAS_VSNPRINTF_EMULATION)
int snprintf_emulation (char *buf, size_t maxlen, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  const int ret = ACE_OS::vsnprintf_emulation (buf, maxlen, format, ap);
  va_end (ap);
  return ret;
}

#define TEST_STR_EQUAL(LHS, RHS)                                              \
  if (ACE_OS::strcmp ((LHS), (RHS))) {                                        \
    failed = true;                                                            \
    ACE_ERROR ((LM_ERROR, "Test assertion FAILED {%C} != {%C} at line %l\n",  \
                (LHS), (RHS)));                                               \
  }

#define TEST_INT_EQUAL(LHS, RHS)                                              \
  if (!((LHS) == (RHS))) {                                                    \
    failed = true;                                                            \
    ACE_ERROR ((LM_ERROR, "Test assertion FAILED %d != %d at line %l\n",      \
                (LHS), (RHS)));                                               \
  }

#define EXPECTED_RESULT(STR)                          \
  TEST_INT_EQUAL (ACE_OS::strlen (STR), size_t (ret)) \
  TEST_STR_EQUAL (STR, buf)

#define EXPECTED_RESULTS(STR_A, STR_B)                                        \
  if (ACE_OS::strcmp ((STR_A), (buf)) && ACE_OS::strcmp ((STR_B), (buf))) {   \
    failed = true;                                                            \
    ACE_ERROR ((LM_ERROR, "Test assertion FAILED {%C} != {%C} and "           \
                          "{%C} != {%C} at line %l\n",                        \
                (STR_A), (buf), (STR_B), (buf)));                             \
  }

#define EXPECTED_RESULT_LEN(STR, LEN)         \
  TEST_INT_EQUAL (LEN, ret)                   \
  TEST_STR_EQUAL (STR, buf)

#define LOG_RESULT(STR)                                                     \
  ACE_DEBUG ((LM_DEBUG, "Locale-dependent snprintf could be {%C}, was {%C}" \
                        " at line %l\n", (STR), buf));

int snprintf_emulation_test ()
{
  bool failed = false;
  char buf[BUFSIZ];

  int ret = snprintf_emulation (buf, sizeof buf, "[%d]", 314); EXPECTED_RESULT ("[314]");
  ret = snprintf_emulation (buf, sizeof buf, "[%i] %% [%u]", -314, 314); EXPECTED_RESULT ("[-314] % [314]");
  ret = snprintf_emulation (buf, sizeof buf, "%5d", 414); EXPECTED_RESULT ("  414");
  ret = snprintf_emulation (buf, sizeof buf, "%05d", 414); EXPECTED_RESULT ("00414");
  ret = snprintf_emulation (buf, sizeof buf, "%*d", 5, 414); EXPECTED_RESULT ("  414");
  ret = snprintf_emulation (buf, sizeof buf, "%-*d", 5, 414); EXPECTED_RESULT ("414  ");
  ret = snprintf_emulation (buf, sizeof buf, "%5i", -414); EXPECTED_RESULT (" -414");
  ret = snprintf_emulation (buf, sizeof buf, "%05i", -414); EXPECTED_RESULT ("-0414");
  ret = snprintf_emulation (buf, sizeof buf, "%2d", -414); EXPECTED_RESULT ("-414");
  ret = snprintf_emulation (buf, sizeof buf, "%.4d", -414); EXPECTED_RESULT ("-0414");
  ret = snprintf_emulation (buf, sizeof buf, "%6.4d", -314); EXPECTED_RESULT (" -0314");
  ret = snprintf_emulation (buf, sizeof buf, "%.4i", 314); EXPECTED_RESULT ("0314");
  ret = snprintf_emulation (buf, sizeof buf, "%6.4u", 414); EXPECTED_RESULT ("  0414");
  ret = snprintf_emulation (buf, sizeof buf, "%.d", 0); EXPECTED_RESULT ("");
  ret = snprintf_emulation (buf, sizeof buf, "% .0d", 0); EXPECTED_RESULT (" ");
  ret = snprintf_emulation (buf, sizeof buf, "%d", 0); EXPECTED_RESULT ("0");
  ret = snprintf_emulation (buf, sizeof buf, "%+d", 0); EXPECTED_RESULT ("+0");
  ret = snprintf_emulation (buf, sizeof buf, "% d", 0); EXPECTED_RESULT (" 0");

  ret = snprintf_emulation (buf, sizeof buf, "%04o", 0755); EXPECTED_RESULT ("0755");
  ret = snprintf_emulation (buf, sizeof buf, "%#o", 0644); EXPECTED_RESULT ("0644");
  ret = snprintf_emulation (buf, sizeof buf, "%#.o", 0); EXPECTED_RESULT ("0");
  ret = snprintf_emulation (buf, sizeof buf, "%#.5o", 0644); EXPECTED_RESULT ("00644");

  ret = snprintf_emulation (buf, sizeof buf, "%x", 0x987abc); EXPECTED_RESULT ("987abc");
  ret = snprintf_emulation (buf, sizeof buf, "%X", 0x987abc); EXPECTED_RESULT ("987ABC");
  ret = snprintf_emulation (buf, sizeof buf, "%02x", 0); EXPECTED_RESULT ("00");
  ret = snprintf_emulation (buf, sizeof buf, "%-#10x", 0x987abc); EXPECTED_RESULT ("0x987abc  ");
  ret = snprintf_emulation (buf, sizeof buf, "%#10X", 0x987abc); EXPECTED_RESULT ("  0X987ABC");
  ret = snprintf_emulation (buf, sizeof buf, "%#X", 0); EXPECTED_RESULT ("0");
  ret = snprintf_emulation (buf, sizeof buf, "%#.X", 0); EXPECTED_RESULT ("");
  ret = snprintf_emulation (buf, sizeof buf, "%#05x", 20); EXPECTED_RESULT ("0x014");
  ret = snprintf_emulation (buf, sizeof buf, "%#.3X", 20); EXPECTED_RESULT ("0X014");
  ret = snprintf_emulation (buf, sizeof buf, "%#6.3X", 20); EXPECTED_RESULT (" 0X014");
  ret = snprintf_emulation (buf, sizeof buf, "%#-6.3X", 20); EXPECTED_RESULT ("0X014 ");

  ret = snprintf_emulation (buf, sizeof buf, "%c", 'a'); EXPECTED_RESULT ("a");
  ret = snprintf_emulation (buf, sizeof buf, "%2c", 'b'); EXPECTED_RESULT (" b");
  ret = snprintf_emulation (buf, sizeof buf, "%-2c", 'c'); EXPECTED_RESULT ("c ");
  ret = snprintf_emulation (buf, sizeof buf, "%-2s", "d"); EXPECTED_RESULT ("d ");
  ret = snprintf_emulation (buf, sizeof buf, "%2s", "e"); EXPECTED_RESULT (" e");
  ret = snprintf_emulation (buf, sizeof buf, "%2.1s", "fg"); EXPECTED_RESULT (" f");
  ret = snprintf_emulation (buf, sizeof buf, "%-2.1s", "gh"); EXPECTED_RESULT ("g ");
  ret = snprintf_emulation (buf, sizeof buf, "%.s", "x"); EXPECTED_RESULT ("");
  ret = snprintf_emulation (buf, sizeof buf, "%-4.9s", "hi"); EXPECTED_RESULT ("hi  ");
  ret = snprintf_emulation (buf, sizeof buf, "%.s", "x"); EXPECTED_RESULT ("");

  int n;
  ret = snprintf_emulation (buf, sizeof buf, "%-5.2s%n %s", "jkl", &n, "lmn"); EXPECTED_RESULT ("jk    lmn");
  ret = snprintf_emulation (buf, sizeof buf, "%0-3.2i", n); EXPECTED_RESULT ("05 ");

  ret = snprintf_emulation (buf, sizeof buf, "%p", reinterpret_cast<void *> (0x1234abc)); EXPECTED_RESULT ("0x1234abc");
  ret = snprintf_emulation (buf, sizeof buf, "%12p", reinterpret_cast<void *> (0x1234abc)); EXPECTED_RESULT ("   0x1234abc");
  ret = snprintf_emulation (buf, sizeof buf, "%-12p", reinterpret_cast<void *> (0x1234abc)); EXPECTED_RESULT ("0x1234abc   ");

  ret = snprintf_emulation (buf, sizeof buf, "%hhu", 0x101); EXPECTED_RESULT ("1");
  ret = snprintf_emulation (buf, sizeof buf, "%hu", 0x10002); EXPECTED_RESULT ("2");
  ret = snprintf_emulation (buf, sizeof buf, "%#lx", 0x87654321ul); EXPECTED_RESULT ("0x87654321");
  ret = snprintf_emulation (buf, sizeof buf, "%llu", 612578912487901265ull); EXPECTED_RESULT ("612578912487901265");

  ret = snprintf_emulation (buf, sizeof buf, "%-+010.7d", 98765); EXPECTED_RESULT ("+0098765  ");
  ret = snprintf_emulation (buf, 10, "%-+010.7d", 98765); EXPECTED_RESULT_LEN ("+0098765 ", 10);

  ret = snprintf_emulation (buf, sizeof buf, "%f", 3.14); EXPECTED_RESULT ("3.140000");
  ret = snprintf_emulation (buf, sizeof buf, "%10f", -3.14); EXPECTED_RESULT (" -3.140000");
  ret = snprintf_emulation (buf, sizeof buf, "%+-10F", 3.14); EXPECTED_RESULT ("+3.140000 ");
  ret = snprintf_emulation (buf, sizeof buf, "%010f", -3.14); EXPECTED_RESULT ("-03.140000");
  ret = snprintf_emulation (buf, sizeof buf, "% f", 3.14); EXPECTED_RESULT (" 3.140000");
  ret = snprintf_emulation (buf, sizeof buf, "% f", HUGE_VAL); EXPECTED_RESULT (" inf");
  ret = snprintf_emulation (buf, sizeof buf, "%f", -HUGE_VAL); EXPECTED_RESULT ("-inf");
  ret = snprintf_emulation (buf, sizeof buf, "%#F", HUGE_VAL); EXPECTED_RESULT ("INF");
  ret = snprintf_emulation (buf, sizeof buf, "%5F", -HUGE_VAL); EXPECTED_RESULT (" -INF");
#ifndef ACE_LYNXOS_MAJOR
  ret = snprintf_emulation (buf, sizeof buf, "%f", std::numeric_limits<double>::quiet_NaN ()); EXPECTED_RESULTS ("nan", "-nan");
  ret = snprintf_emulation (buf, sizeof buf, "%+F", std::numeric_limits<double>::quiet_NaN ()); EXPECTED_RESULTS ("+NAN", "-NAN");
#endif
  ret = snprintf_emulation (buf, sizeof buf, "%.f", 2.17); EXPECTED_RESULT ("2");
  ret = snprintf_emulation (buf, sizeof buf, "%#.f", 2.17); EXPECTED_RESULT ("2.");
  ret = snprintf_emulation (buf, sizeof buf, "%.1f", 18.); EXPECTED_RESULT ("18.0");
  ret = snprintf_emulation (buf, sizeof buf, "%.1f", .9); EXPECTED_RESULT ("0.9");
  ret = snprintf_emulation (buf, sizeof buf, "%.2f", .01); EXPECTED_RESULT ("0.01");

  ret = snprintf_emulation (buf, sizeof buf, "%.2e", .01); EXPECTED_RESULT ("1.00e-02");
  ret = snprintf_emulation (buf, sizeof buf, "%#.E", .01); EXPECTED_RESULT ("1.E-02");
  ret = snprintf_emulation (buf, sizeof buf, "%+.E", .01); EXPECTED_RESULT ("+1E-02");
  ret = snprintf_emulation (buf, sizeof buf, "%e", 3.14159265); EXPECTED_RESULT ("3.141592e+00");
  ret = snprintf_emulation (buf, sizeof buf, "% .e", 0.); EXPECTED_RESULT (" 0e+00");
  ret = snprintf_emulation (buf, sizeof buf, "% -8.e", 0.); EXPECTED_RESULT (" 0e+00  ");

#if !defined _MSC_VER || ACE_CC_MAJOR_VERSION > 7
  ret = snprintf_emulation (buf, sizeof buf, "% -11.2e", -0.); EXPECTED_RESULT ("-0.00e+00  ");
#endif

  ret = snprintf_emulation (buf, sizeof buf, "%.E", 9e101); EXPECTED_RESULTS ("9E+101", "8E+101"); // could be rounded

  ret = snprintf_emulation (buf, sizeof buf, "%g", 3.); EXPECTED_RESULT ("3");
  ret = snprintf_emulation (buf, sizeof buf, "%g", 3.000001); EXPECTED_RESULT ("3");
  ret = snprintf_emulation (buf, sizeof buf, "%.6g", 3.000001); EXPECTED_RESULT ("3");
  ret = snprintf_emulation (buf, sizeof buf, "%G", 3000000.1); EXPECTED_RESULT ("3E+06");
  ret = snprintf_emulation (buf, sizeof buf, "%+#g", 3000000.1); EXPECTED_RESULT ("+3.00000e+06");
  ret = snprintf_emulation (buf, sizeof buf, "%G", -3000010.); EXPECTED_RESULTS ("-3.00001E+06", "-3E+06");
  ret = snprintf_emulation (buf, sizeof buf, "%g", .0001); EXPECTED_RESULT ("0.0001");
  ret = snprintf_emulation (buf, sizeof buf, "%- g", .00001); EXPECTED_RESULT (" 1e-05");

  ret = snprintf_emulation (buf, sizeof buf, "%a", 4.); EXPECTED_RESULT ("0x8p-1");
  ret = snprintf_emulation (buf, sizeof buf, "%#a", 4.); EXPECTED_RESULT ("0x8.p-1");
  ret = snprintf_emulation (buf, sizeof buf, "%A", -3.125); EXPECTED_RESULT ("-0XC.8P-2");
  ret = snprintf_emulation (buf, sizeof buf, "%+a", 0.); EXPECTED_RESULT ("+0x0p+0");
  ret = snprintf_emulation (buf, sizeof buf, "%+-10.1a", 0.); EXPECTED_RESULT ("+0x0.0p+0 ");
  ret = snprintf_emulation (buf, sizeof buf, "%0+10.1a", 0.); EXPECTED_RESULT ("+0x00.0p+0");
  ret = snprintf_emulation (buf, sizeof buf, "% 09A", 16.625); EXPECTED_RESULT (" 0X8.5P+1");
  ret = snprintf_emulation (buf, sizeof buf, "% 010a", 16.625); EXPECTED_RESULT (" 0x08.5p+1");

  ret = snprintf_emulation (buf, sizeof buf, "%%%d%#x", 1, 2); EXPECTED_RESULT ("%10x2");
  ret = snprintf_emulation (buf, sizeof buf, "foo %%%% bar"); EXPECTED_RESULT ("foo %% bar");
  ret = snprintf_emulation (buf, sizeof buf, ""); EXPECTED_RESULT ("");
  ret = snprintf_emulation (buf, sizeof buf, "foo"); EXPECTED_RESULT ("foo");

  ret = snprintf_emulation (buf, sizeof buf, "%'d", 12345); LOG_RESULT ("12,345");
  ret = snprintf_emulation (buf, sizeof buf, "%'d", 123456); LOG_RESULT ("123,456");
  ret = snprintf_emulation (buf, sizeof buf, "%'d", 1234567890); LOG_RESULT ("1,234,567,890");
  ret = snprintf_emulation (buf, sizeof buf, "%'.f", 12345.); LOG_RESULT ("12,345");
  ret = snprintf_emulation (buf, sizeof buf, "%'.f", 1234567890.); LOG_RESULT ("1,234,567,890");
  ret = snprintf_emulation (buf, sizeof buf, "%'.6d", 12345); LOG_RESULT ("012,345");
  ret = snprintf_emulation (buf, sizeof buf, "%0'8d", 12345); LOG_RESULT ("0012,345");

  ret = snprintf_emulation (buf, sizeof buf, "a%%b%2$*3$.*1$s %%%%%%%4$*3$.*1$s", 3, "cdef", 4, "fgh"); EXPECTED_RESULT ("a%b cde %%% fgh");
  ret = snprintf_emulation (buf, sizeof buf, "%3$d %6$d %5$d %4$d %8$d %9$d %7$d %1$d %2$d %10$d %11$d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11); EXPECTED_RESULT ("3 6 5 4 8 9 7 1 2 10 11");
  ret = snprintf_emulation (buf, sizeof buf, "%2$0*1$d %3$.*1$f %1$.*2$d", 3, 2, 3.1); EXPECTED_RESULT ("002 3.100 03");
  ret = snprintf_emulation (buf, sizeof buf, "%2$#*1$x", -4, 10); EXPECTED_RESULT ("0xa ");

  return failed ? 1 : 0;
}
#endif // ACE_HAS_VSNPRINTF_EMULATION

int
swab_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing swab method\n")));

  int error_count = 0;
  char from[] =   "BADCFEHGJILKNMPORQTSVUXWZY";
  char to[] =     "..........................";
  char expect[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  ACE_OS::swab (from, to, sizeof (from));
  if (ACE_OS::strcmp (to, expect) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("swab error: %C, expected %C\n"),
                  to, expect));
      ++error_count;
    }

  return error_count;
}

int
gai_strerror_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing gai_strerror method\n")));

  const ACE_TCHAR* error_text = ACE_OS::gai_strerror (EAI_FAMILY);

  ACE_UNUSED_ARG (error_text);

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("OS_Test"));

  // Enable a locale that has digit grouping so that snprintf's %'d is
  // different than %d.  If the locale is not available the test won't
  // fail (log file needs to be examined to check formatting).
#ifdef ACE_WIN32
  std::setlocale(LC_NUMERIC, "en-US");
#elif defined ACE_LINUX
  std::setlocale(LC_NUMERIC, "en_US.utf8");
#endif

  int status = 0;
  int result;

  if ((result = access_test ()) != 0)
    status = result;

  if ((result = rename_test ()) != 0)
    status = result;

  if ((result = string_emulation_test ()) != 0)
    status = result;

  if ((result = snprintf_test (ACE_OS::snprintf)) != 0)
    status = result;

#if defined (ACE_HAS_VSNPRINTF_EMULATION)
  if ((result = snprintf_test (snprintf_emulation)) != 0)
    status = result;

  if ((result = snprintf_emulation_test ()) != 0)
    status = result;
#endif

  if ((result = getpwnam_r_test ()) != 0)
    status = result;

  if ((result = ctime_r_test ()) != 0)
    status = result;

  if ((result = string_strsncpy_test ()) != 0)
      status = result;

  if ((result = strsignal_test ()) != 0)
      status = result;

  if ((result = cpu_info_test ()) != 0)
      status = result;

  if ((result = pagesize_test ()) != 0)
      status = result;

  if ((result = ceilf_test ()) != 0)
      status = result;

  if ((result = floorf_test ()) != 0)
      status = result;

  if ((result = ceil_test ()) != 0)
      status = result;

  if ((result = floor_test ()) != 0)
      status = result;

  if ((result = ceill_test ()) != 0)
      status = result;

  if ((result = floorl_test ()) != 0)
      status = result;

  if ((result = log2_test ()) != 0)
      status = result;

  if ((result = last_error_test ()) != 0)
      status = result;

  if ((result = ace_ctype_test ()) != 0)
      status = result;

  if ((result = swab_test ()) != 0)
      status = result;

  if ((result = compiler_test ()) != 0)
      status = result;

  if ((result = version_test ()) != 0)
      status = result;

  if ((result = gai_strerror_test   ()) != 0)
      status = result;

  ACE_END_TEST;
  return status;
}
#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
