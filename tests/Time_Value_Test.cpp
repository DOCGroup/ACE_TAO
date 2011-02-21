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
//      This is a simple test of ACE_Time_Value.  No command line arguments
//      are needed to run the test.  It also tests the ACE_U_LongLong class.
//      Because ACE_U_LongLong is used for ACE_hrtime_t on some platforms,
//      this seems like a reasonable place to test it.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> and David Levine <levine@cs.wustl.edu>
//
// ============================================================================

// Note, for this test the config.h file *must* come first!
#include "ace/config-all.h"



#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Time_Value.h"

#include "ace/Numeric_Limits.h"

#ifdef ACE_HAS_CPP98_IOSTREAMS
#include <sstream>
#endif

int
run_main (int, ACE_TCHAR *[])
{
  int ret = 0;

  ACE_START_TEST (ACE_TEXT ("Time_Value_Test"));

  ACE_Time_Value tv1;
  ACE_Time_Value tv2 (2);
  ACE_Time_Value tv3 (100U);
  ACE_Time_Value tv4 (1, 1000000);
  ACE_Time_Value tv5 (2UL);
  ACE_Time_Value tv6 (1, -1000000);
  ACE_Time_Value tv7 (static_cast<long> (2.0));

  // Beware!  2.5 gets truncated to 2!
  // NOTE:  this is intended to show what happens with
  // ACE_Time_Value (2.5).  Some compilers, such as g++ 2.7.2.3,
  // actually warn about it without the case.
  ACE_Time_Value tv8 (static_cast <long> (2.5));

  // Test assignment operator, tv9 and tv6 must be the same after this
  ACE_Time_Value tv9;
  tv9 = tv6;

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
  ACE_ASSERT (tv7 == tv8); // That's right!  See above . . .
  ACE_ASSERT (tv9 == tv6);

  ACE_Time_Value tv10 (1);

  ACE_ASSERT (tv10.sec() == 1);

  // test multiplication by double
  // test simple multiplication
  tv1.set (1, 1);
  tv2.set (2, 2);
  tv1 *= 2.0;
  ACE_ASSERT (tv1 == tv2);
  tv1.set (1, 1);
  tv2.set (static_cast<time_t> (-2), static_cast<suseconds_t> (-2));
  tv1 *= -2.0;
  ACE_ASSERT (tv1 == tv2);

  // test usec shift
  tv1.set (1, 999999);
  tv2.set (19, 999990);
  tv1 *= 10.0;
  ACE_ASSERT ( tv1 == tv2);
  tv1.set (1, 999999);
  tv2.set (static_cast<time_t> (-19), -999990);
  tv1 *= -10.0;
  ACE_ASSERT (tv1 == tv2);

  const time_t max_time_t = ACE_Numeric_Limits<time_t>::max ();
  const time_t min_time_t = ACE_Numeric_Limits<time_t>::min ();

  // test protection against overflows
  // ACE_ASSERT( ACE_Time_Value(max_time_t,ACE_ONE_SECOND_IN_USECS) != ACE_Time_Value(ACE_Numeric_Limits<time_t>::min()) );

  // test saturated result
  tv1.set (max_time_t - 1, 499999);
  tv2.set (max_time_t, 999999);  // ACE_Time_Value::max_time
  tv1 *= 10.0;
  ACE_ASSERT (tv1 == tv2);
  tv1.set (max_time_t - 1, 499999);
  tv2.set (min_time_t, -999999);
  tv1 *= -10.0;
  ACE_ASSERT (tv1 == tv2);

  // test results near limits
  tv1.set ((max_time_t >> 1), 499999);
  tv2.set ((-(max_time_t >> 1) << 1), -999998);
  tv1 *= -2.0;
  ACE_ASSERT (tv1 == tv2);
  tv1.set (max_time_t >> 1, 499999);
  tv2.set (((max_time_t >> 1) << 1), 999998);
  tv1 *= 2.0;
  ACE_ASSERT (tv1 == tv2);

  // Test correct msec() convert; also checks for compile error reported in
  // Bugzilla 3336.
  ACE_Time_Value msec_test (42, 555000);
  ACE_UINT64 ms = 0;
  msec_test.msec (ms);
  if (ms != 42555)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("msec test failed: %Q should be 42555\n"),
                ms));
  ms = 0;
  ms = msec_test.get_msec ();
  if (ms != 42555)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("get_msec test failed: %Q should be 42555\n"),
                ms));
  ACE_Time_Value const msec_test2 (42, 555000);
  ms = 0;
  msec_test2.msec (ms);
  if (ms != 42555)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("msec const test failed: %Q should be 42555\n"),
                ms));

  // Test setting from ACE_UINT64
  ms = 42555;
  ACE_Time_Value msec_test3;
  msec_test3.set_msec (ms);
  if (msec_test3.sec () != 42)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("set_msec test failed: %d secs should be 42\n"),
                msec_test3.sec ()));
  if (msec_test3.usec () != 555000)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("set_msec test failed: %d usecs should be 555000\n"),
                msec_test3.usec ()));

#ifdef ACE_HAS_CPP98_IOSTREAMS
  std::ostringstream ost;
  ost << ACE_Time_Value(1);
  ACE_ASSERT( ost.str() == "1" );
  ost.str("");
  ost << ACE_Time_Value(1,1);
  ACE_ASSERT( ost.str() == "1.000001" );
  ost.str("");
  ost << ACE_Time_Value(static_cast<time_t>(-1),-1);
  ACE_ASSERT( ost.str() == "-1.000001" );
  ost.str("");
  ost << ACE_Time_Value(0,1);
  ACE_ASSERT( ost.str() == "0.000001" );
  ost.str("");
  ost << ACE_Time_Value(0,-1);
  ACE_ASSERT( ost.str() == "-0.000001" );
  ost.str("");
  ost << ACE_Time_Value();
  ACE_ASSERT( ost.str() == "0" );
#endif

  ACE_END_TEST;

  return ret;
}
