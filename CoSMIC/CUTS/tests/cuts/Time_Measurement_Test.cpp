// $Id$

#include "Time_Measurement_Test.h"
#include "cuts/Time_Measurement.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"

#include <algorithm>
#include <vector>
#include <numeric>

namespace CUTS_Test_Data
{
  CUTS_Time_Measurement tm_;
}

//=============================================================================
/*
 * CUTS_Time_Measurement_Test
 */
//=============================================================================

CUTS_TEST_EXPORT (CUTS_Time_Measurement_Test)

CUTS_TEST_IMPL_BEGIN (CUTS_Time_Measurement_Test, "Time_Measurement_Test")
  CUTS_ADD_UNIT_TEST (Time_Measurement_Constructor)
  CUTS_ADD_UNIT_TEST (Time_Measurement_Add_Time)
  CUTS_ADD_UNIT_TEST (Time_Measurement_Reset)
CUTS_TEST_IMPL_END (CUTS_Time_Measurement_Test)

//=============================================================================
/*
 * Time_Measurement_Constructor
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Time_Measurement_Constructor, "constructor");

CUTS_UNIT_TEST_RUN_IMPL (Time_Measurement_Constructor)
{
  using namespace CUTS_Test_Data;

  CUTS_VERIFY_TEST ((tm_.maximum () != ACE_Time_Value::zero ||
                     tm_.minimum () != ACE_Time_Value::zero ||
                     tm_.accumulation () != ACE_Time_Value::zero ||
                     tm_.count () != 0),
                     "default constructor failed");
  return 0;
}

//=============================================================================
/*
 * Time_Measurement_Add_Time
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Time_Measurement_Add_Time, "Add_Time")

CUTS_UNIT_TEST_RUN_IMPL (Time_Measurement_Add_Time)
{
  ACE_Time_Value tv;

  long msec = ACE_OS::rand () % 1000;
  tv.msec (msec);

  do
  {
    CUTS_Time_Measurement tm;
    tm += tv;

    CUTS_VERIFY_TEST (tm.count () != 1,
                      "expected count = 1");
    CUTS_VERIFY_TEST (tm.accumulation () != tv,
                      "accumulation value is incorrect");
    CUTS_VERIFY_TEST (tm.maximum () != tv,
                      "maximum value is incorrect");
    CUTS_VERIFY_TEST (tm.minimum () != tv,
                      "minimum value is incorrect");
  } while (0);

  // randomly generate 50-100 numbers and verify the time measurement
  // object handles them correctly.

  using namespace CUTS_Test_Data;
  size_t length = 50 + (ACE_OS::rand () % 50);

  typedef std::vector <long> Long_Vector;
  Long_Vector values (length);

  for (size_t i = 0; i < length; i ++)
  {
    msec = ACE_OS::rand () % 1000;
    tv.msec (msec);

    values[i] = msec;

    tm_ += tv;
  }

  Long_Vector::iterator min_iter =
    std::min_element (values.begin (), values.end ());
  Long_Vector::iterator max_iter =
    std::max_element (values.begin (), values.end ());
  long sum = std::accumulate (values.begin (), values.end (), 0);

  CUTS_VERIFY_TEST (tm_.count () != length,
                    "count () failed with multiple values");
  CUTS_VERIFY_TEST (tm_.minimum ().msec () != *min_iter,
                    "minimum () failed with multiple values");
  CUTS_VERIFY_TEST (tm_.maximum ().msec () != *max_iter,
                    "maximum () failed with multiple values");
  CUTS_VERIFY_TEST (tm_.accumulation ().msec () != sum,
                    "accumulation () failed with multiple values");
  return 0;
}

//=============================================================================
/*
 * Time_Measurement_Reset
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Time_Measurement_Reset, "Reset");

CUTS_UNIT_TEST_RUN_IMPL (Time_Measurement_Reset)
{
  using namespace CUTS_Test_Data;
  tm_.reset ();

  CUTS_VERIFY_TEST ((tm_.maximum () != ACE_Time_Value::zero ||
                     tm_.minimum () != ACE_Time_Value::zero ||
                     tm_.accumulation () != ACE_Time_Value::zero ||
                     tm_.count () != 0),
                     "reset operation failed");
  return 0;
}
