// $Id$
//
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    High_Res_Timer_Test.cpp
//
// = DESCRIPTION
//    Simple test of ACE_High_Res_Timer.
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, High_Res_Timer_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static
u_int
check (const u_int interval, const u_int measured)
{
  const u_int threshold = 25 /* percent */;

  const u_int difference =
    interval > measured  ?  interval - measured  :  measured - interval;

  const u_int percentage_difference = difference * 100 / interval;

  if (percentage_difference < threshold)
    {
      return 0;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("The measured time of %u differs from "
                             "the interval of %u by %u percent.\n"),
                  measured, interval, percentage_difference));
      return 1;
    }
}

static
ACE_Time_Value
time_interval (const ACE_Time_Value &interval)
{
  ACE_High_Res_Timer timer;

  timer.start ();
  ACE_OS::sleep (interval);
  timer.stop ();

  ACE_Time_Value measured;
  timer.elapsed_time (measured);
  return measured;
}

static
u_int
intervals [] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000}; /* usec */

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("High_Res_Timer_Test"));

  ACE_DEBUG ((LM_DEBUG, "The ACE_High_Res_Timer scale factor is %u "
                        "1/microsecond\n",
              ACE_High_Res_Timer::global_scale_factor ()));

  u_int errors = 0;

  u_int iterations = 1;

  ACE_Get_Opt getopt (argc, argv, ASYS_TEXT ("i:"));
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (getopt.optarg);
        break;
      }

  // We don't check for errors if the interval is shorter than this
  // value because the OS has a finite resolution anyway.
  const u_int TIMER_RESOLUTION = 10000;

  for (u_int i = 0; i < sizeof intervals / sizeof (u_int); ++i)
    {
      for (u_int j = 0; j < iterations; ++j)
        {
          const ACE_Time_Value interval (0, intervals[i]);
          const ACE_Time_Value measured = time_interval (interval);
          ACE_DEBUG ((LM_DEBUG, "interval: %u usec, measured: %u usec%s\n",
                      interval.sec () * 1000000 + interval.usec (),
                      measured.sec () * 1000000 + measured.usec (),
                      intervals[i] <= TIMER_RESOLUTION  ?
                      " (interval and measured may differ)"  :  ""));

          if (intervals[i] > TIMER_RESOLUTION)
            {
              errors += check (interval.sec () * 1000000 + interval.usec (),
                               measured.sec () * 1000000 + measured.usec ());
              // Don't check for error for intervals below 10 msec.
            }
        }
    }

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
