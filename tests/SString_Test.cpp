// ============================================================================
// @(#)SString_Test.cpp	1.1	10/18/96

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

#include "ace/SString.h"
#include "test_config.h"

int 
main (int argc, char *argv[])
{
  ACE_START_TEST;

  ACE_CString s1 ("hello");
  ACE_CString s2 ("world");
  ACE_CString s3 ("el");
  ACE_WString s4 ("hello");
  ACE_WString s5 ("world");
  ACE_WString s6 ("el");
  
  ACE_ASSERT (s1 != s2);
  ACE_ASSERT (s1.strstr (s2) == -1);
  ACE_ASSERT (s1.strstr (s2) == -1);
  ACE_ASSERT (s1.strstr (s3));
  ACE_ASSERT (s4.strstr (s5) == -1);
  ACE_ASSERT (s5.strstr (s6));

  ACE_END_TEST;
  return 0;
}
