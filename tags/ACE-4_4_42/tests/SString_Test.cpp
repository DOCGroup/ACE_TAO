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

int 
main (int, char *[])
{
  ACE_START_TEST ("SString_Test");

  {
    ACE_CString s1 ("hello");
    ACE_CString s2 ("world");
    ACE_CString s3 ("el");
    
    ACE_ASSERT (s1 != s2);
    ACE_ASSERT (s1.strstr (s2) == -1);
    ACE_ASSERT (s1.strstr (s3));
    
    ACE_ASSERT (s1.find (s3) == 1);
    ACE_ASSERT (s1.find (s3, 2) == ACE_CString::npos);
    ACE_ASSERT (s1.find (s2) == ACE_CString::npos);
    ACE_ASSERT (s1.find ('o') == 4);
    ACE_ASSERT (s1.rfind ('l') == 3);
    ACE_ASSERT (s1.rfind ('l', 3) == 2);
  }

  {
    ACE_WString s1 ("hello");
    ACE_WString s2 ("world");
    ACE_WString s3 ("el");
    
    ACE_ASSERT (s1 != s2);
    ACE_ASSERT (s1.strstr (s2) == -1);
    ACE_ASSERT (s1.strstr (s3));
    
    ACE_ASSERT (s1.find (s3) == 1);
    ACE_ASSERT (s1.find (s3, 2) == ACE_WString::npos);
    ACE_ASSERT (s1.find (s2) == ACE_WString::npos);
    ACE_ASSERT (s1.find ('o') == 4);
    ACE_ASSERT (s1.rfind ('l') == 3);
    ACE_ASSERT (s1.rfind ('l', 3) == 2);
  }

  ACE_END_TEST;
  return 0;
}
