// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark
//
// = FILENAME
//    marshal_results.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _CORBA_MARSHAL_RESULTS_H_
#define _CORBA_MARSHAL_RESULTS_H_

#include "ace/Profile_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class CORBA_Marshal_Results
{
public:
  CORBA_Marshal_Results (void);
  // Construction.

  ~CORBA_Marshal_Results (void);
  // destructor.

  void clear (void);
  // Clear all results.

  ACE_Profile_Timer::ACE_Elapsed_Time & elapsed_time (void);
  // Get the elapsed time.

  ACE_Profile_Timer::Rusage elapsed_rusage (void);
  // compute the rusage

  int start_timer (void);
  // starts a profile timer.

  int stop_timer (void);
  // stops the profile timer.

  void filename (const char *fname);
  // set the name of the file to store results

  int log_results (void);
  // log results

  int print_results (void);
  // prints the results

private:

  // = Used for performance statistics
  ACE_Profile_Timer timer_;
  ACE_Profile_Timer::ACE_Elapsed_Time et_;
  ACE_Profile_Timer::Rusage rusage_;

  double elapsed_time_;
  // Real elapsed time in seconds.

  const char *filename_;
  // filename
};

#if defined (__ACE_INLINE__)
#include "benchmark/marshal_results.i"
#endif /* __ACE_INLINE__ */

#endif
