// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    Test ACE_OS::strptime
//
// = AUTHOR
//    Johnny Willemsen
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"



int
strptime_test (void)
{
  // convert UTC time string to UTC ACE_Time_Value
  int error_count = 0;
  const char* original_time = "2008-06-21 23:45";
  tm lTime;
  ACE_OS::strptime(original_time, "%Y-%m-%d %H:%M", &lTime);
  lTime.tm_isdst = 0; // do not change due to daylight saving time
  time_t lNewTime = ACE_OS::mktime(&lTime);
  ACE_Time_Value lValue(lNewTime - ACE_OS::timezone(), 0); // do  not change because of timezone

  // convert UTC ACE_Time_Value to UTC time string
  char lBuffer[128];
  time_t time = lValue.sec();
  struct tm tm_time;
  ACE_OS::gmtime_r (&time, &tm_time);
  if (ACE_OS::strftime(lBuffer, 128, "%Y-%m-%d %H:%M", &tm_time) == 0 && errno == ENOTSUP)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("strftime is not supported on this platform\n")));
    }
  else if (ACE_OS::strcmp (lBuffer, original_time) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%C != %C\n"), lBuffer, original_time));
      ++error_count;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("strptime_test succeeded\n")));
    }

  return error_count;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3432_Regression_Test"));

  int status = 0;
  int result = 0;

  if ((result = strptime_test ()) != 0)
      status = result;

  ACE_END_TEST;
  return status;
}
