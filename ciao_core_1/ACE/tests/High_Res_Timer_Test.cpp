// $Id$
//============================================================================
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
//    David L. Levine <levine@cs.wustl.edu>
//
//============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Truncate.h"

ACE_RCSID(tests, High_Res_Timer_Test, "High_Res_Timer_Test.cpp,v 4.6 2000/04/23 04:43:58 brunsch Exp")

static
u_int
check (const time_t interval, const time_t measured)
{
  const u_int threshold = 25 /* percent */;

  const time_t difference =
    interval > measured  ?  interval - measured  :  measured - interval;

  const u_int percentage_difference = (u_int) (difference * 100 / interval);

  if (percentage_difference < threshold)
    return 0;
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("The measured time of %u differs from ")
                       ACE_TEXT ("the interval of %u by %u percent.\n"),
                       (unsigned int) measured,
                       (unsigned int) interval,
                       percentage_difference),
                      1);
}

// Does a sanity check of the microseconds vs nanoseconds. They're supposed
// to represent the same interval.
static u_int
check_micro_nano (ACE_hrtime_t microinterval, ACE_hrtime_t nanointerval)
{
  const u_int threshold = 8 /* promille */;

  microinterval *= 1000u;
  ACE_hrtime_t hr_difference = (microinterval > nanointerval  ?
                                microinterval - nanointerval  :
                                nanointerval - microinterval    );
  const u_int difference = ACE_HRTIME_CONVERSION (hr_difference);
  if (nanointerval == 0)
    nanointerval = 1;      // Prevent divide-by-zero
  const u_int promille_difference = difference * 1000 /
                                          ACE_HRTIME_CONVERSION(nanointerval);

  if ((promille_difference < threshold) || (difference < 1500))
    return 0;
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("The microseconds * 1000 of %u differs from ")
                       ACE_TEXT ("the nanoseconds of %u by %u promille\n"),
                       ACE_HRTIME_CONVERSION (microinterval),
                       ACE_HRTIME_CONVERSION (nanointerval),
                       promille_difference),
                      1);
}

static
ACE_Time_Value
time_interval (const ACE_Time_Value &interval,
               ACE_hrtime_t& nanoseconds,
               ACE_hrtime_t& microseconds)
{
  ACE_High_Res_Timer timer;

  timer.start ();
  ACE_OS::sleep (interval);
  timer.stop ();

  ACE_Time_Value measured;
  timer.elapsed_time (measured);
  timer.elapsed_time (nanoseconds);
  timer.elapsed_microseconds (microseconds);
  return measured;
}


static
u_int
intervals [] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 4000000}; /*usec*/

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("High_Res_Timer_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("The ACE_High_Res_Timer scale factor is %u ")
              ACE_TEXT ("1/microsecond\n"),
              ACE_High_Res_Timer::global_scale_factor ()));

  u_int errors = 0;

  u_int iterations = 1;

  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("i:"));
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (getopt.opt_arg ());
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
          ACE_hrtime_t nanoseconds;
          ACE_hrtime_t microseconds;
          const ACE_Time_Value measured = time_interval (interval,
                                                         nanoseconds,
                                                         microseconds);
          u_int interval_usec =
            ACE_Utils::Truncate<u_int> (interval.sec () * 1000000 + interval.usec ());
          u_int measured_usec =
            ACE_Utils::Truncate<u_int> (measured.sec () * 1000000 + measured.usec ());
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("interval: %u usec, measured: %u usec%s\n"),
                      interval_usec,
                      measured_usec,
                      intervals[i] <= TIMER_RESOLUTION  ?
                      ACE_TEXT (" (interval and measured may differ)")  :
                      ACE_TEXT ("")));

          if (intervals[i] > TIMER_RESOLUTION)
            {
              errors += check (interval.sec () * 1000000 + interval.usec (),
                               measured.sec () * 1000000 + measured.usec ());
              // Don't check for error for intervals below 10 msec.
            }
          // Check the ACE_Timer_Value-calculated microseconds against
          // the ACE_High_Res_Timer-calculated nanoseconds.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ACE_Time_Value usec: %u, ACE_HR nsec: %u\n"),
                      measured.sec () * 1000000 + measured.usec (),
                      ACE_HRTIME_CONVERSION (nanoseconds)));
          // This gives problems -> should be fixed
          errors += check_micro_nano (measured.sec () * 1000000 +
                                      measured.usec (),
                                      nanoseconds);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ACE_High_Res_Timer usec: %u, nsec: %u\n"),
                      ACE_HRTIME_CONVERSION (microseconds),
                      ACE_HRTIME_CONVERSION (nanoseconds)));
          // Now check the ACE_High_Res_Timer-calculated values against
          // each other.
          errors += check_micro_nano (microseconds, nanoseconds);
        }
    }

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
