// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    results.h
//
// = DESCRIPTION
//    Printing the results
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef RESULTS_H
#define RESULTS_H

#include "tao/corba.h"
#include "ace/Profile_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Results
{
public:
  Results (void);
  // ctor

  ~Results (void);
  // destructor

  void print_stats (void);
  // prints the time stats

  void print_exception (const char *call_name
                        ACE_ENV_ARG_DECL_NOT_USED);
  // print the exception

  void start_timer (void);
  // start timing

  void stop_timer (void);
  // stop timing

  CORBA::ULong call_count (void);
  // return call count

  void call_count (CORBA::ULong);
  // set the call count

  CORBA::ULong error_count (void);
  // return the error count

  void error_count (CORBA::ULong);
  // set error count

  void iterations (CORBA::ULong iters);
  // set the number of times the test will be run

private:
  CORBA::ULong call_count_;
  // # of calls made to functions

  CORBA::ULong error_count_;
  // # of errors incurred in the lifetime of the application.

  ACE_Profile_Timer timer_;
  // for timing the test

  ACE_Profile_Timer::ACE_Elapsed_Time *elapsed_time_;
  // holds the elapsed time for each iteration
};

#endif /* RESULTS_H */
