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

  void print_total (char *message, 
		    int iterations, 
		    ACE_HANDLE handle);
  // Print total time.

  void print_ave (char *message, 
		  int iterations, 
		  ACE_HANDLE handle);
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
};

#if defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_HI_RES_TIMER */
#endif /* ACE_HIGH_RES_TIMER_H */
