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

#if !defined (ACE_HIGH_RES_TIMER_H)
#define ACE_HIGH_RES_TIMER_H

#include "ace/ACE.h"

class ACE_Export ACE_High_Res_Timer
  // = TITLE
  //     A high resolution timer class wrapper that encapsulates
  //     OS-specific high-resolution timers, such as those found on
  //     Solaris, AIX, Win32/Pentium, and VxWorks.
  //
  // = DESCRIPTION
  //     Most of the member functions don't return values.  The only
  //     reason that one would fail is if high-resolution time isn't
  //     supported on the platform.  To avoid impacting performance and
  //     complicating the interface, the "supported ()" member function was
  //     added.  It returns 1 if high-resolution time (ACE_OS::gethrtime ())
  //     is supported on the platform, and 0 if not.
  //
  //     The optional scale factor is required for platforms that have
  //     high-resolution timers that return units other than nanoseconds,
  //     such as clock ticks.  The member functions that return or print
  //     times use this scale factor.  They divide the "time" that they
  //     get from ACE_OS::gethrtime () by it to obtain the time in nanoseconds.
  //     Its units are 1/second, possibly ticks/second.
  //
  //     NOTE:  the elapsed time calculations in the print methods use
  //     ACE_hrtime_t values.  If ACE_hrtime_t is not a 64-bit type
  //     (ACE_HAS_LONGLONG_T), then those calculations are more susceptible
  //     to overflow.  Those methods do _not_ check for overflow!
{
public:
  // = Initialization method.

  static int supported ();
  // Returns 1 if high-resolution time is supported on the platform, 0 if not.

  ACE_High_Res_Timer (u_long scale_factor = 0);
  // Initialize the timer.

  void reset (void);
  // Reinitialize the timer.

  void start (void);
  // Start timing.

  void stop (void);
  // Stop timing.

  void elapsed_time (ACE_Time_Value &tv);
  // Set <tv> to the number of microseconds elapsed.

#if defined (ACE_HAS_POSIX_TIME)
  void elapsed_time (struct timespec &);
  // returns the elapsed (stop - start) time in a struct timespec (sec, nsec)
#endif /* ACE_HAS_POSIX_TIME */

  void elapsed_microseconds (ACE_hrtime_t &usecs) const;
  // Sets <usecs> to the elapsed (stop - start) time in microseconds.

  void start_incr (void);
  // Start incremental timing.

  void stop_incr (void);
  // Stop incremental timing.

  void elapsed_time_incr (ACE_Time_Value &tv);
  // Set <tv> to the number of microseconds elapsed between all
  // calls to start_incr and stop_incr.

  void print_total (const char *message,
		    const int iterations = 1,
		    ACE_HANDLE handle = ACE_STDOUT);
  // Print total time.  NOTE:  only use print_total ()
  // if incremental timings had been used!

  void print_ave (const char *message,
		  const int iterations = 1,
		  ACE_HANDLE handle = ACE_STDOUT);
  // Print average time.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_hrtime_t start_;
  // Starting time.

  ACE_hrtime_t end_;
  // Ending time.

  ACE_hrtime_t total_;
  // Total elapsed time.

  ACE_hrtime_t start_incr_;
  // Start time of incremental timing.

  u_long scale_factor_;
};

#if defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HIGH_RES_TIMER_H */
