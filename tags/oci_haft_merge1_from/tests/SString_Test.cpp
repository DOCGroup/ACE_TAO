// $Id$

// ============================================================================
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
//    Prashant Jain <pjain@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

ACE_RCSID(tests, SString_Test, "$Id$")

  int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SString_Test"));

  {

    /* Set #1 */
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

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #1:\n"));}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != -1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s3.strstr (s1) != -1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.substring (4, 10).length () != 1){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s3.find (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s3.find (s1, 1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.find (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #1: \n"));}

    // Assignment. Error if they are not equal
    ACE_CString s6;
    s6 = s0;
    if (s6 != s0){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    s6 = s4;
    if (s4 != s6){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
    s6 = s5;
    if (s6 != s5){ACE_ERROR((LM_ERROR,"Set #1: \n"));}
  }

  {
    /* Set #2 */
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

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != -1){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s3.strstr (s1) != -1){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s3.find (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s3.find (s1, 1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.find (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Assignment. Error if they are not equal
    ACE_CString s6;
    s6 = s0;
    if (s6 != s0){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    s6 = s4;
    if (s4 != s6){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
    s6 = s5;
    if (s6 != s5){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Clear. Error if they are not equal
    s0.clear();
    if (s0.length() != 0){ACE_ERROR((LM_ERROR,"Set #2: \n"));}

    // Rep. Error if they are not equal
    ACE_Auto_Basic_Array_Ptr<char> s (s1.rep ());
    if (ACE_OS::strlen (s.get ()) != s1.length ())
      {
        ACE_ERROR((LM_ERROR,"Auto_ptr s: \n"));
      };

    ACE_CString s7 (s.get ());
    if (s1 != s7){ACE_ERROR((LM_ERROR,"Set #2: \n"));}
  }

  {
    /* Set #3 */
    ACE_NS_WString s0 ("hello");
    ACE_NS_WString s1 ("hello");
    ACE_NS_WString s2 ("world");
    ACE_NS_WString s3 ("ll");
    ACE_NS_WString s4 ("ello");
    ACE_NS_WString s5 = s1 + " " + s2;
    ACE_NS_WString s6 = ("hella"); // Same length as s1, off by one char.

    ACE_WCHAR_T single_character = 'z';
    ACE_NS_WString single_character_string (single_character);

    ACE_NS_WString empty_string;
    ACE_NS_WString zero_size_string (s1.c_str (), 0, 0);

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1 == s6){ACE_ERROR((LM_ERROR,"Set #3: off-by-one failed\n"));}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != -1){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s3.strstr (s1) != -1){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s3.find (s1) != ACE_WString::npos){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s3.find (s1, 1) != ACE_WString::npos){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.find (s2) != ACE_WString::npos){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Assignment. Error if they are not equal
    ACE_NS_WString s7;
    s7 = s0;
    if (s7 != s0){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    s7 = s4;
    if (s4 != s7){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
    s7 = s5;
    if (s7 != s5){ACE_ERROR((LM_ERROR,"Set #3: \n"));}

    // Clear. Error if they are not equal
    s0.clear();
    if (s0.length() != 0){ACE_ERROR((LM_ERROR,"Set #3: \n"));}
  }

  {
    /* Set #4 */
    ACE_CString s1("dog");
    ACE_CString s2("d");

    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if (!(s1 > s2)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if (s1 < s2){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

    ACE_CString s3 ("dog");
    ACE_CString s4 ("dogbert");

    if (s3 == s4){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if (!(s3 < s4)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if (s3 > s4){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

    ACE_CString s5 ("dogbert",3);
    ACE_CString s6 ("dogbert",5);

    if(s5 == s6){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(!(s5 < s6)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(s5 > s6){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

    ACE_CString s7 ("dogbert",4);
    ACE_CString s8 ("dogbert",2);

    if(s7 == s8){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(!(s7 > s8)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(s7 < s8){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

    ACE_CString s9 ("dogbert",3);
    ACE_CString s10 ("dogbert");

    if(s9 == s10){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(!(s9 < s10)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(s9 > s10){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

    ACE_CString s11 ("dogbert",5);
    ACE_CString s12 ("dog");

    if(s11 == s12){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(!(s11 > s12)){ACE_ERROR((LM_ERROR,"Set #4: \n"));}
    if(s11 < s12){ACE_ERROR((LM_ERROR,"Set #4: \n"));}

  }
  ACE_END_TEST;
  return 0;
}
