// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
//
// = FILENAME
//    SString_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the use of ACE_CString
//      and ACE_WString. No command line arguments are needed to run
//      the test.
//
// = AUTHOR
//    Prashant Jain
//
// ============================================================================

#include "test_config.h"
#include "ace/SString.h"

ACE_RCSID(tests, SString_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("SString_Test"));

  {
    ACE_CString s0 ("hello");
    ACE_CString s1 ("hello");
    ACE_CString s2 ("world");
    ACE_CString s3 ("ll");
    ACE_CString s4 ("ello");
    ACE_CString s5 = s1 + " " + s2;

    char single_character = 'z';
    ACE_CString single_character_string (single_character);

    ACE_CString empty_string;
    ACE_CString zero_size_string (s1.c_str (), 0, 0, 1);

    // Not equal comparisons.
    ACE_ASSERT (s1 != s2);
    ACE_ASSERT (s1 != s5);

    // Equal comparisons.
    ACE_ASSERT (s1 == s1);
    ACE_ASSERT (s1 == s0);

    // Substring match.
    ACE_ASSERT (s1.strstr (s2) == -1);
    ACE_ASSERT (s1.strstr (s3) == 2);
    ACE_ASSERT (s3.strstr (s1) == -1);
    ACE_ASSERT (s1.strstr (s4) == 1);

    // Substring creation.
    ACE_ASSERT (s1.substring (0) == s1);
    ACE_ASSERT (s1.substring (1) == s4);
    ACE_ASSERT (s1.substring (2, 2) == s3);
    ACE_ASSERT (s1.substring (0, 0) == empty_string);

    // Forward search.
    ACE_ASSERT (s1.find (s3) == 2);
    ACE_ASSERT (s3.find (s1) == ACE_CString::npos);
    ACE_ASSERT (s1.find (s3, 2) == 0);
    ACE_ASSERT (s3.find (s1, 1) == ACE_CString::npos);
    ACE_ASSERT (s1.find (s2) == ACE_CString::npos);
    ACE_ASSERT (s1.find ('o') == 4);

    // Reverse search.
    ACE_ASSERT (s1.rfind ('l') == 3);
    ACE_ASSERT (s1.rfind ('l', 3) == 2);
  }

  {
    ACE_CString s0 ("hello", 0, 0);
    ACE_CString s1 ("hello", 0, 0);
    ACE_CString s2 ("world", 0, 0);
    ACE_CString s3 ("ll", 0, 0);
    ACE_CString s4 ("ello", 0, 0);
    ACE_CString s5 = s1 + " " + s2;

    char single_character = 'z';
    ACE_CString single_character_string (single_character);

    ACE_CString empty_string (0, 0, 0);
    ACE_CString zero_size_string (s1.c_str (), 0, 0, 0);

    // Not equal comparisons.
    ACE_ASSERT (s1 != s2);
    ACE_ASSERT (s1 != s5);

    // Equal comparisons.
    ACE_ASSERT (s1 == s1);
    ACE_ASSERT (s1 == s0);

    // Substring match.
    ACE_ASSERT (s1.strstr (s2) == -1);
    ACE_ASSERT (s1.strstr (s3) == 2);
    ACE_ASSERT (s3.strstr (s1) == -1);
    ACE_ASSERT (s1.strstr (s4) == 1);

    // Substring creation.
    ACE_ASSERT (s1.substring (0) == s1);
    ACE_ASSERT (s1.substring (1) == s4);
    ACE_ASSERT (s1.substring (2, 2) == s3);
    ACE_ASSERT (s1.substring (0, 0) == empty_string);

    // Forward search.
    ACE_ASSERT (s1.find (s3) == 2);
    ACE_ASSERT (s3.find (s1) == ACE_CString::npos);
    ACE_ASSERT (s1.find (s3, 2) == 0);
    ACE_ASSERT (s3.find (s1, 1) == ACE_CString::npos);
    ACE_ASSERT (s1.find (s2) == ACE_CString::npos);
    ACE_ASSERT (s1.find ('o') == 4);

    // Reverse search.
    ACE_ASSERT (s1.rfind ('l') == 3);
    ACE_ASSERT (s1.rfind ('l', 3) == 2);
  }

  {
    ACE_WString s0 ("hello");
    ACE_WString s1 ("hello");
    ACE_WString s2 ("world");
    ACE_WString s3 ("ll");
    ACE_WString s4 ("ello");
    ACE_WString s5 = s1 + " " + s2;

    ACE_USHORT16 single_character = 'z';
    ACE_WString single_character_string (single_character);
    
    ACE_WString empty_string;
    ACE_WString zero_size_string (s1.c_str (), 0, 0);

    // Not equal comparisons.
    ACE_ASSERT (s1 != s2);
    ACE_ASSERT (s1 != s5);

    // Equal comparisons.
    ACE_ASSERT (s1 == s1);
    ACE_ASSERT (s1 == s0);

    // Substring match.
    ACE_ASSERT (s1.strstr (s2) == -1);
    ACE_ASSERT (s1.strstr (s3) == 2);
    ACE_ASSERT (s3.strstr (s1) == -1);
    ACE_ASSERT (s1.strstr (s4) == 1);

    // Substring creation.
    ACE_ASSERT (s1.substring (0) == s1);
    ACE_ASSERT (s1.substring (1) == s4);
    ACE_ASSERT (s1.substring (2, 2) == s3);
    ACE_ASSERT (s1.substring (0, 0) == empty_string);

    // Forward search.
    ACE_ASSERT (s1.find (s3) == 2);
    ACE_ASSERT (s3.find (s1) == ACE_WString::npos);
    ACE_ASSERT (s1.find (s3, 2) == 0);
    ACE_ASSERT (s3.find (s1, 1) == ACE_WString::npos);
    ACE_ASSERT (s1.find (s2) == ACE_WString::npos);
    ACE_ASSERT (s1.find ('o') == 4);

    // Reverse search.
    ACE_ASSERT (s1.rfind ('l') == 3);
    ACE_ASSERT (s1.rfind ('l', 3) == 2);
  }

  ACE_END_TEST;
  return 0;
}
