/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    High_Res_Timer.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_HIGH_RES_TIMER_H
#define ACE_HIGH_RES_TIMER_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_High_Res_Timer
{
  // = TITLE
  //     A high resolution timer class wrapper that encapsulates
  //     OS-specific high-resolution timers, such as those found on
  //     Solaris, AIX, Win32/Pentium, and VxWorks.
  //
  // = DESCRIPTION
  //     Most of the member functions don't return values.  The only
  //     reason that one would fail is if high-resolution time isn't
  //     supported on the platform.  To avoid impacting performance and
  //     complicating the interface, in that case, ACE_OS::gettimeofday ()
  //     is used instead.
  //
  //     The global scale factor is required for platforms that have
  //     high-resolution timers that return units other than
  //     microseconds, such as clock ticks.  It is represented as a
  //     static u_long, can only be accessed through static methods,
  //     and is used by all instances of High Res Timer.  The member
  //     functions that return or print times use the global scale
  //     factor.  They divide the "time" that they get from
  //     ACE_OS::gethrtime () by global_scale_factor_ to obtain the
  //     time in microseconds.  Its units are therefore 1/microsecond.
  //     On Solaris, a scale factor of 1000 should be used because its
  //     high-resolution timer returns nanoseconds.  However, on Intel
  //     platforms, we use RDTSC which returns the number of clock
  //     ticks since system boot.  For a 200MHz cpu, each clock tick
  //     is 1/200 of a microsecond; the global_scale_factor_ should
  //     therefore be 200.
  //
  //     NOTE:  the elapsed time calculations in the print methods use
  //     ACE_hrtime_t values.  Those methods do _not_ check for overflow!
  //
  //     NOTE:  Gabe <begeddov@proaxis.com> raises this issue
  //     regarding ACE_OS::gethrtime ():  on multi-processors, the
  //     processor that you query for your timer.stop () value might
  //     not be the one you queried for timer.start ().  Its not clear
  //     how much divergence there would be, if any.
  //
  //     This issue is not mentioned in the Solaris 2.5.1 gethrtime
  //     man page.
public:
  // = Initialization method.

  static void global_scale_factor (ACE_UINT32 gsf);
  // global_scale_factor_ is set to <gsf>.  All High_Res_Timers use
  // global_scale_factor_.  This allows applications to set the scale
  // factor just once for all High_Res_Timers.  Check
  // High_Res_Timer.cpp for the default global_scale_factors for
  // several platforms.  For many platforms (e.g., Solaris), the
  // global_scale_factor_ is set to 1000 so that <scale_factor> need
  // not be set.  Careful, a <scale_factor> of 0 will cause division
  // by zero exceptions.

  static ACE_UINT32 global_scale_factor ();
  // Accesses the global_scale_factor.

  static int get_env_global_scale_factor (const char *env = "ACE_SCALE_FACTOR");
  // Sets the global_scale_factor to the value in the <env>
  // environment variable.  Returns 0 on success, -1 on failure.  Note
  // if <env> points to string "0" (value zero), this call will fail.
  // This is basically a no-op on CE because there is no concept of
  // environment variable on CE.

  static ACE_UINT32 calibrate (const ACE_UINT32 usec = 500000,
                               const u_int iterations = 10);
  // Set (and return, for info) the global scale factor by sleeping
  // for <usec> and counting the number of intervening clock cycles.
  // Average over <iterations> of <usec> each.  On some platforms,
  // such as Pentiums, this is called automatically during the first
  // ACE_High_Res_Timer construction with the default parameter
  // values.  An application can override that by calling calibrate
  // with any desired parameter values _prior_ to constructing the
  // first ACE_High_Res_Timer instance.

  ACE_High_Res_Timer (void);
  // Initialize the timer.

  ~ACE_High_Res_Timer (void);
  // dtor.

  void reset (void);
  // Reinitialize the timer.

  void start (const ACE_OS::ACE_HRTimer_Op = ACE_OS::ACE_HRTIMER_GETTIME);
  // Start timing.

  void stop (const ACE_OS::ACE_HRTimer_Op = ACE_OS::ACE_HRTIMER_GETTIME);
  // Stop timing.

  void elapsed_time (ACE_Time_Value &tv) const;
  // Set <tv> to the number of microseconds elapsed.

  void elapsed_time (ACE_hrtime_t &nanoseconds) const;
  // Set <nanoseconds> to the number of nanoseconds elapsed.

#if defined (ACE_HAS_POSIX_TIME)
  void elapsed_time (struct timespec &) const;
  // Returns the elapsed (stop - start) time in a struct timespec
  // (sec, nsec).
#endif /* ACE_HAS_POSIX_TIME */

  void elapsed_microseconds (ACE_hrtime_t &usecs) const;
  // Sets <usecs> to the elapsed (stop - start) time in microseconds.

  void start_incr (const ACE_OS::ACE_HRTimer_Op = ACE_OS::ACE_HRTIMER_GETTIME);
  // Start incremental timing.

  void stop_incr (const ACE_OS::ACE_HRTimer_Op = ACE_OS::ACE_HRTIMER_GETTIME);
  // Stop incremental timing.

  void elapsed_time_incr (ACE_Time_Value &tv) const;
  // Set <tv> to the number of microseconds elapsed between all
  // calls to start_incr and stop_incr.

  void elapsed_time_incr (ACE_hrtime_t &nanoseconds) const;
  // Set <nsec> to the number of nanoseconds elapsed between all
  // calls to start_incr and stop_incr.

#if !defined (ACE_HAS_WINCE)
  // @@ These two functions are currently not supported on Windows CE.
  //    However, we should probably use the handle and ACE_Log_Msg to
  //    print out the result.
  void print_total (const char *message,
                    const int iterations = 1,
                    ACE_HANDLE handle = ACE_STDOUT) const;
  // Print total time.  NOTE:  only use print_total ()
  // if incremental timings had been used!

  void print_ave (const char *message,
                  const int iterations = 1,
                  ACE_HANDLE handle = ACE_STDOUT) const;
  // Print average time.
#endif /* !ACE_HAS_WINCE */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  static ACE_Time_Value gettimeofday (const ACE_OS::ACE_HRTimer_Op =
                                        ACE_OS::ACE_HRTIMER_GETTIME);
  // THIS FUNCTION IS DEPRECATED.  PLEASE USE ACE_OS::gettimeofday ()
  // INSTEAD!
  // Calls ACE_High_Res_Timer::hrtime_to_tv passing ACE_OS::gethrtime.
  // This function can be used to parameterize objects such as
  // ACE_Timer_Queue::gettimeofday.  If global_scale_factor_ is not
  // set, and we're on a platform that requires global_scale_factor_
  // (e.g., Win32), ACE_OS::gettimeofday will be used instead of
  // ACE_OS::gethrtime.  This allows applications on Intel to use
  // High_Res_Timer even when global_scale_factor is not set.
  // However, setting the global_scale_factor_ appropriately will
  // result in the finest resolution possible.

private:
  static void hrtime_to_tv (ACE_Time_Value &tv,
                            const ACE_hrtime_t hrt);
  // Converts an <hrt> to <tv> using global_scale_factor_.

  static ACE_hrtime_t gettime (const ACE_OS::ACE_HRTimer_Op =
                                 ACE_OS::ACE_HRTIMER_GETTIME);
  // For internal use:  gets the high-resolution time using
  // ACE_OS::gethrtime ().  Except on platforms that require
  // that the global_scale_factor_ be set, such as ACE_WIN32,
  // uses the low-resolution clock if the global_scale_factor_
  // has not been set.

  ACE_hrtime_t start_;
  // Starting time.

  ACE_hrtime_t end_;
  // Ending time.

  ACE_hrtime_t total_;
  // Total elapsed time.

  ACE_hrtime_t start_incr_;
  // Start time of incremental timing.

  static ACE_UINT32 global_scale_factor_;
  // Converts ticks to microseconds.  That is, ticks /
  // global_scale_factor_ == microseconds.
};

#if defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HIGH_RES_TIMER_H */
