//=============================================================================
/**
 *  @file    NDDS_Timer_Test.cpp
 *
 *  $Id$
 *
 *  Simple test of NDDS_Timer. Copied from NDDS_Timer_Test.
 *
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include <ndds/ndds_cpp.h>
#include <ndds/clock/clock_highResolution.h>

static
u_int
check (const time_t interval, const time_t measured)
{
  time_t const threshold = 25 /* percent */;

  time_t const difference =
    interval > measured  ?  interval - measured  :  measured - interval;

  time_t const percentage_difference = difference * 100 / interval;

  if (percentage_difference < threshold)
    return 0;
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("The measured time of %Q differs from ")
                       ACE_TEXT ("the interval of %Q by %Q percent.\n"),
                       measured,
                       interval,
                       percentage_difference),
                      1);
}

// Does a sanity check of the microseconds vs nanoseconds. They're supposed
// to represent the same interval.
static u_int
check_micro_nano (unsigned long long microinterval, unsigned long long nanointerval)
{
  unsigned long long const threshold = 8 /* promille */;

  microinterval *= 1000u;
  unsigned long long const difference = (microinterval > nanointerval
                                   ? (microinterval - nanointerval)
                                   : (nanointerval - microinterval));
  if (nanointerval == 0)
    {
      nanointerval = 1;      // Prevent divide-by-zero
    }
  unsigned long long const promille_difference =
    difference * 1000 / nanointerval;

  if ((promille_difference < threshold) || (difference < 1500))
    {
      return 0;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("The microseconds * 1000 of %Q ")
                       ACE_TEXT ("differs from the nanoseconds of %Q ")
                       ACE_TEXT (" by %Q promille\n"),
                       microinterval,
                       nanointerval,
                       promille_difference),
                      1);
}

static
void
time_interval (RTIClock *timer,
               const ACE_Time_Value & interval,
               RTINtpTime& duration)
{
  struct RTINtpTime start_time;
  struct RTINtpTime finish_time;

  RTINtpTime_setZero(&start_time);
  RTINtpTime_setZero(&finish_time);

  // Initialize
  timer->reset(timer);

  // Run the test
  timer->getTime(timer, &start_time);
  ACE_OS::sleep (interval);
  timer->getTime(timer, &finish_time);

  // Measure.
  RTINtpTime_subtract(duration, finish_time, start_time);
}


static u_int const intervals[] =
  {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 4000000}; /*usec*/

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("NDDS_Timer_Test"));

  RTIClock *timer = RTIHighResolutionClock_new();

  u_int errors = 0;

  u_int iterations = 1;

  ACE_Get_Opt getoptarg (argc, argv, ACE_TEXT ("i:"));
  for (int c; (c = getoptarg ()) != -1; )
    {
      switch (c)
        {
        case 'i':
          iterations = ACE_OS::atoi (getoptarg.opt_arg ());
          break;
        }
    }

  // We don't check for errors if the interval is shorter than this
  // value because the OS has a finite resolution anyway.
  static u_int const TIMER_RESOLUTION = 10000;

  for (u_int i = 0; i < sizeof intervals / sizeof (u_int); ++i)
    {
      for (u_int j = 0; j < iterations; ++j)
        {
          ACE_Time_Value const interval (0, intervals[i]);

          RTINtpTime duration;
          time_interval (timer, interval, duration);

          unsigned long long microseconds, nanoseconds;
          int sec;
          RTINtpTime_unpackToMicrosec (sec, microseconds, duration);
          RTINtpTime_unpackToNanosec (sec, nanoseconds, duration);

          time_t const interval_usec =
            interval.sec () * ACE_ONE_SECOND_IN_USECS + interval.usec ();
          time_t const measured_usec =
            sec * ACE_ONE_SECOND_IN_USECS + microseconds;


          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("interval: %: usec, measured: %: usec %s\n"),
                      interval_usec,
                      measured_usec,
                      (intervals[i] <= TIMER_RESOLUTION
                       ? ACE_TEXT (" (interval and measured may differ)")
                       : ACE_TEXT (""))));

          if (intervals[i] > TIMER_RESOLUTION)
            {
              errors += check (interval.sec () * ACE_ONE_SECOND_IN_USECS
                               + interval.usec (),
                               sec * ACE_ONE_SECOND_IN_USECS + microseconds);
              // Don't check for error for intervals below 10 msec.
            }
          errors += check_micro_nano (microseconds, nanoseconds);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("NDDS_Timer usec: %Q, nsec: %Q\n"),
                      microseconds,
                      nanoseconds));
        }
    }

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
