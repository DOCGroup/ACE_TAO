
//=============================================================================
/**
 *  @file    Bug_2434_Regression_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test of ACE_Time_Value.  No command line arguments
 *    are needed to run the test.  It also tests the ACE_U_LongLong class.
 *    Because ACE_U_LongLong is used for ACE_hrtime_t on some platforms,
 *    this seems like a reasonable place to test it.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and David Levine <levine@cs.wustl.edu>
 */
//=============================================================================


// Note, for this test the config.h file *must* come first!
#include "ace/config-all.h"



#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Time_Value.h"
#include "ace/Numeric_Limits.h"

int
run_main (int, ACE_TCHAR *[])
{
  int ret = 0;

  ACE_START_TEST (ACE_TEXT ("Bug_2434_Regression_Test"));

  ACE_Time_Value tv1;
  ACE_Time_Value tv2;

  const time_t max_time_t = ACE_Numeric_Limits<time_t>::max ();
  const time_t min_time_t = ACE_Numeric_Limits<time_t>::min ();

  // test saturated result
  tv1.set (max_time_t - 1, 499999);
  tv2.set (max_time_t, 999999);  // ACE_Time_Value::max_time
  tv1 *= 10.0;
  ACE_TEST_ASSERT (tv1 == tv2);
  tv1.set (max_time_t - 1, 499999);
  tv2.set (min_time_t, -999999);
  tv1 *= -10.0;
  ACE_TEST_ASSERT (tv1 == tv2);

  // test results near limits
  tv1.set ((max_time_t >> 1), 499999);
  tv2.set ((-(max_time_t >> 1) << 1), -999998);
  tv1 *= -2.0;
  ACE_TEST_ASSERT (tv1 == tv2);
  tv1.set (max_time_t >> 1, 499999);
  tv2.set (((max_time_t >> 1) << 1), 999998);
  tv1 *= 2.0;
  ACE_TEST_ASSERT (tv1 == tv2);

  ACE_END_TEST;

  return ret;
}
