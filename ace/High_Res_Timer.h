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

#if defined (ACE_HAS_HI_RES_TIMER)

class ACE_Export ACE_High_Res_Timer
  // = TITLE
  //     A high resolution timer class wrapper that encapsulates
  //     Solaris timers. 
{
public:
  // = Initialization method.

  ACE_High_Res_Timer (void);
  // Initialize the timer.

  void reset (void);
  // Reinitialize the timer.

  void start (void);
  // Start timing.

  void stop (void);
  // Stop timing.

  void start_incr (void);
  // Start incremental timing.

  void stop_incr (void);
  // Stop incremental timing.

#if defined (ACE_HAS_POSIX_TIME)
  const timespec_t &elapsed_time (void);
  // returns the elapsed (stop - start) time in a timespec_t (sec, nsec)
#endif /* ACE_HAS_POSIX_TIME */

  unsigned long get_time () const;
  // returns the elapsed (stop - start) time in microseconds

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

  hrtime_t temp_;
  // Temp time used for incremental timing.

#if defined (ACE_HAS_POSIX_TIME)
  timespec_t elapsed_time_;
#endif /* ACE_HAS_POSIX_TIME */
};

#if defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_HI_RES_TIMER */
#endif /* ACE_HIGH_RES_TIMER_H */
