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
  //     Solaris, AIX, Win32, and VxWorks.
  //
  // = DESCRIPTION
  //     Many of the member functions return -1 if the OS does not
  //     provide high-resolution time support.  On success, these functions
  //     return other than -1, but not any number in particular (they don't
  //     return 0 in order to maximize performance as much as possible,
  //     while still providing a -1 return value on unsupported platforms).
  //
  //     NOTE:  the elapsed time calculations in the print methods use
  //     hrtime_t values.  If hrtime_t is not a 64-bit type
  //     (ACE_HAS_LONGLONG_T), then those calculations are more susceptible
  //     to overflow.  Those methods do _not_ check for overflow!
{
public:
  // = Initialization method.

  ACE_High_Res_Timer (void);
  // Initialize the timer.

  void reset (void);
  // Reinitialize the timer.

  int start (void);
  // Start timing.  Returns -1 if high resolution timers are
  // not implemented on the platform.

  int stop (void);
  // Stop timing.  Returns -1 if high resolution timers are
  // not implemented on the platform.

  void elapsed_time (ACE_Time_Value &tv);
  // Set <tv> to the number of microseconds elapsed.

#if defined (ACE_HAS_POSIX_TIME)
  void elapsed_time (timespec_t &);
  // returns the elapsed (stop - start) time in a timespec_t (sec, nsec)
#endif /* ACE_HAS_POSIX_TIME */

  void elapsed_microseconds (hrtime_t &usecs) const;
  // Sets <usecs> to the elapsed (stop - start) time in microseconds.

  int start_incr (void);
  // Start incremental timing.  Returns -1 if high resolution timers are
  // not implemented on the platform.

  int stop_incr (void);
  // Stop incremental timing.  Returns -1 if high resolution timers are
  // not implemented on the platform.

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
  hrtime_t start_;
  // Starting time.

  hrtime_t end_;
  // Ending time.

  hrtime_t total_;
  // Total elapsed time.

  hrtime_t start_incr_;
  // Start time of incremental timing.
};

#if defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HIGH_RES_TIMER_H */
