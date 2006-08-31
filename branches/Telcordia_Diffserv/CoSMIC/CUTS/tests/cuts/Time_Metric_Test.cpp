// $Id$

#include "Time_Metric_Test.h"
#include "cuts/Time_Metric.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"

CUTS_TEST_EXPORT (CUTS_Time_Metric_Test)

namespace CUTS_Testing
{
  CUTS_Time_Metric time_metric_;
}

//=============================================================================
/*
 * CUTS_Time_Metric_Test
 */
//=============================================================================

CUTS_TEST_IMPL_BEGIN (CUTS_Time_Metric_Test, "Time_Metric_Test")
  CUTS_ADD_UNIT_TEST (Unit_Test_TM_Time_Value)
  CUTS_ADD_UNIT_TEST (Unit_Test_TM_Operators)
  CUTS_ADD_UNIT_TEST (Unit_Test_TM_Reset)
CUTS_TEST_IMPL_END (CUTS_Time_Metric_Test)

//=============================================================================
/*
 * Unit_Test_TM_Time_Value
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_TM_Time_Value, "Time_Values");

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_TM_Time_Value)
{
  using namespace CUTS_Testing;

  // Verify the default values for the <time_metric_>.
  CUTS_VERIFY_TEST (time_metric_.count () != 0,
                    "expected count () = 0");
  CUTS_VERIFY_TEST (time_metric_.best_time () != 0,
                    "expected best_time () = 0");
  CUTS_VERIFY_TEST (time_metric_.average_time () != 0,
                    "expected average_time () = 0");
  CUTS_VERIFY_TEST (time_metric_.worse_time () != 0,
                    "expected worse_time () = 0");

  // Update the <time_metric_> with arbitrary metrics.
  int count = ACE_OS::rand () % 50;
  long min = ACE_OS::rand () % 1000;
  long avg = min + ACE_OS::rand () % 1000;
  long max = avg + ACE_OS::rand () % 1000;

  time_metric_.update (count, avg, min, max);

  CUTS_VERIFY_TEST (time_metric_.count () != count,
                    "expected count () = " << count);

  CUTS_VERIFY_TEST (time_metric_.best_time () != min,
                    "expected best_time () = " << min);

  CUTS_VERIFY_TEST (time_metric_.average_time () != avg,
                    "expected average_time () = " << avg);

  CUTS_VERIFY_TEST (time_metric_.worse_time () != max,
                    "expected worse_time () = " << max);

  // Check the <timestamp> operation.
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  time_metric_.timestamp (now);

  CUTS_VERIFY_TEST (time_metric_.timestamp () != now,
                    "set/get timestamp () is not equivalent");
  return 0;
}

//=============================================================================
/*
 * Unit_Test_TM_Operators
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_TM_Operators, "operators");

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_TM_Operators)
{
  using namespace CUTS_Testing;

  do
  {
    // testing the copy constructor.
    CUTS_Time_Metric tm (time_metric_);

    CUTS_VERIFY_TEST ((tm.count () != time_metric_.count () ||
                       tm.average_time () != time_metric_.average_time () ||
                       tm.best_time () != time_metric_.best_time () ||
                       tm.worse_time () != time_metric_.worse_time ()),
                      "copy constructor failed");
  } while (0);

  do
  {
    // testing the assignment operator.
    CUTS_Time_Metric tm;
    tm = time_metric_;

    CUTS_VERIFY_TEST ((tm.count () != time_metric_.count () ||
                       tm.average_time () != time_metric_.average_time () ||
                       tm.best_time () != time_metric_.best_time () ||
                       tm.worse_time () != time_metric_.worse_time ()),
                      "assignment operator failed");
  } while (0);

  do
  {
    // testing the self-assigning addition operator
    CUTS_Time_Metric tm;
    tm += time_metric_;

    CUTS_VERIFY_TEST ((tm.average_time () != time_metric_.average_time () ||
                       tm.best_time () != time_metric_.best_time () ||
                       tm.worse_time () != time_metric_.worse_time ()),
                      "self-assigning addition operation failed");
  } while (0);
  return 0;
}

//=============================================================================
/*
 * Unit_Test_TM_Operators
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_TM_Reset, "reset");

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_TM_Reset)
{
  using namespace CUTS_Testing;
  time_metric_.reset ();

  CUTS_VERIFY_TEST ((time_metric_.count () != 0 ||
                     time_metric_.average_time () != 0 ||
                     time_metric_.best_time () != 0 ||
                     time_metric_.worse_time () != 0),
                    "all values of time metric not 0");
  return 0;
}
