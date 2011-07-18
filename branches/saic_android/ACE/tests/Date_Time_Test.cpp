// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program verifies the ACE_Date_Time class.
//
// = AUTHOR
//    Steve Huston  <shuston@riverace.com>
//
// ============================================================================

#include "ace/Date_Time.h"
#include "test_config.h"

static ACE_Date_Time static_dt;   // Making sure it doesn't crash.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Date_Time_Test"));

  int error = 0;

  ACE_Date_Time dt;

  long month   = dt.month ();
  long day     = dt.day ();
  long year    = dt.year ();
  long hour    = dt.hour ();
  long minute  = dt.minute ();
  long seconds = dt.second ();
  long usec    = dt.microsec ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("ACE_Date_Time (m/d/y, h:m:s.u): %d/%d/%d, %d:%d:%d.%d\n"),
              month, day, year, hour, minute, seconds, usec));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ACE_Log thinks it is: %D\n")));

  if (month < 1 || month > 12)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Month (%d) out of range (1-12)\n"),
                  month));
      error = 1;
    }
  if (day < 1 || day > 31)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Day (%d) out of range (1-31)\n"),
                  day));
      error = 1;
    }
  if (year < 1900 || year > 2100)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Year (%d) out of range (1900-2100)\n"),
                  year));
      error = 1;
    }
  if (hour < 0 || hour > 23)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Hour (%d) out of range (0-23)\n"),
                  hour));
      error = 1;
    }
  if (minute < 0 || minute > 59)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Minute (%d) out of range (0-59)\n"),
                  minute));
      error = 1;
    }
  if (seconds < 0 || seconds > 59)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Seconds (%d) out of range (0-59)\n"),
                  seconds));
      error = 1;
    }
  if (usec < 0 || usec > 999999)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Microseconds (%d) out of range (0-999999)\n"),
                  usec));
      error = 1;
    }

  // The static ACE_Date_Time object is primarily to be sure it doesn't
  // crash; However, let's do some sanity checks on it to be sure it's
  // legit as well.
  if (static_dt.month () != month)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Static month (%d) doesn't match %d\n"),
                  static_dt.month (), month));
      error = 1;
    }
  if (static_dt.day () != day)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Static day (%d) doesn't match %d\n"),
                  static_dt.day (), day));
      error = 1;
    }
  if (static_dt.year () != year)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Static year (%d) doesn't match %d\n"),
                  static_dt.year (), year));
      error = 1;
    }
  if (static_dt.hour () != hour)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Static hour (%d) doesn't match %d\n"),
                  static_dt.hour (), hour));
      error = 1;
    }

  // There's a rare instance where the starting seconds is 59 and the
  // minute roles over during the test run.
  if (!(static_dt.minute () == minute ||
       (static_dt.minute () + 1 == minute && static_dt.second () > seconds)))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Static minute (%d) doesn't match %d\n"),
                  static_dt.minute (), minute));
      error = 1;
    }
  ACE_END_TEST;
  return error;
}
