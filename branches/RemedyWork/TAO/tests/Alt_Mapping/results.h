
//=============================================================================
/**
 *  @file    results.h
 *
 *  $Id$
 *
 *  Printing the results
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef RESULTS_H
#define RESULTS_H

#include "tao/Basic_Types.h"

#include "ace/Profile_Timer.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class Environment;
}
TAO_END_VERSIONED_NAMESPACE_DECL

class Results
{
public:
  /// ctor
  Results (void);

  /// destructor
  ~Results (void);

  /// prints the time stats
  void print_stats (void);

  /// print the exception
  void print_exception (const char *call_name);

  /// start timing
  void start_timer (void);

  /// stop timing
  void stop_timer (void);

  /// return call count
  CORBA::ULong call_count (void);

  /// set the call count
  void call_count (CORBA::ULong);

  /// return the error count
  CORBA::ULong error_count (void);

  /// set error count
  void error_count (CORBA::ULong);

  /// set the number of times the test will be run
  void iterations (CORBA::ULong iters);

private:
  /// # of calls made to functions
  CORBA::ULong call_count_;

  /// # of errors incurred in the lifetime of the application.
  CORBA::ULong error_count_;

  /// for timing the test
  ACE_Profile_Timer timer_;

  /// holds the elapsed time for each iteration
  ACE_Profile_Timer::ACE_Elapsed_Time *elapsed_time_;
};

#endif /* RESULTS_H */
