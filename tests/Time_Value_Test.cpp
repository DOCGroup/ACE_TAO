// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Time_Value_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of ACE_Time_Value. No command line
//      arguments are needed to run the test. 
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#include "ace/ACE.h"

#include "test_config.h"

int
main (int, char *[])
{
  ACE_START_TEST ("Time_Value_Test");

  ACE_Time_Value tv1;
  ACE_Time_Value tv2 (2);
  ACE_Time_Value tv3 (100);
  ACE_Time_Value tv4 (1, 1000000);
  ACE_Time_Value tv5 (2);
  ACE_Time_Value tv6 (1, -1000000);
  ACE_Time_Value tv7 ((long) 2.0);

  ACE_ASSERT (tv1 == ACE_Time_Value (0));
  ACE_ASSERT (tv2 < tv3);
  ACE_ASSERT (tv2 <= tv2);
  ACE_ASSERT (tv2 >= tv4);
  ACE_ASSERT (tv5 >= tv6);
  ACE_ASSERT (tv2 == ACE_Time_Value (1, 1000000));
  ACE_ASSERT (tv5 == tv4);
  ACE_ASSERT (tv2 == tv4);
  ACE_ASSERT (tv1 != tv2);
  ACE_ASSERT (tv6 == tv1);
  ACE_ASSERT (tv5 == tv7);

  ACE_END_TEST;
  return 0;
}
