// -*- C++ -*-
// $Id$

#ifndef ACE_BASELINE_TEST_H
#define ACE_BASELINE_TEST_H

#include "ace/Profile_Timer.h"

#include "Synch_Lib/Benchmark_Base.h"

class ACE_Svc_Export Baseline_Test_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Performance Test class.
public:
  virtual int init (int argc, char *argv[]);
  // Standard initializing method for Baseline Test.

  int parse_args (int argc, char *argv[]);
  // Parsing the svc.conf file arguments.

  virtual int acquire () = 0;
  virtual int release () = 0;
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release () = 0;
  virtual int test_try_lock () = 0;
  // Real test methods.

  size_t multiply_factor (void);
  size_t iteration (void);
  // Access methods.

protected:
  Baseline_Test_Base (void);

  size_t multiply_factor_;
  // Number of operations before yielding to other threads.

  size_t iteration_;
  // Total number of operations.  <iterations_> <= <multiply_factor_> * n
};

class ACE_Svc_Export Baseline_Test_Options
{
  // = TITLE
  //    This class holds the global settings for Baseline Test.
public:
  friend class Baseline_Test;

  Baseline_Test_Options (void);
  // ctor.

  int parse_args (int argc, char *argv[]);
  // Parse and set the Baseline_Test options and flags.

  int reset_params (size_t mulply_factor, size_t iteration);
  // Reset test parameters for next round.

  int test_try_lock (void);
  // Return test configuration.

  int add_time (ACE_Profile_Timer::ACE_Elapsed_Time &et);
  // Add the time spent in the current iterations.

  int inc_loop_counter (void);
  // Added multiply_factor_ to total_iteration_.

  size_t multiply_factor (void);
  // Return <multiply_factor_>.

  size_t iteration (void);
  // Return <iteration_>.

  void print_result (void);
  // Print out the result.

private:
  int test_try_lock_;
  // A flag indicates whether we are testing try_lock or lock and
  // release.

  int verbose_;
  // Print out the result in verbose mode.

  size_t multiply_factor_;
  // Number loop before performing thread yield.

  size_t iteration_;
  // Number of iteration.

  size_t total_iteration_;
  // Real iteration.  After performing a test,
  // total_iteration_ < iteration_ + multiply_factor_.

  ACE_timer_t real_;
  ACE_timer_t system_;
  ACE_timer_t user_;
  // Profile timer result.
};

extern Baseline_Test_Options baseline_options;

class ACE_Svc_Export Baseline_Test : public Benchmark_Method_Base
{
public:
  Baseline_Test (void);
  virtual int init (int argc, char *argv[]);
  virtual int pre_run_test (Benchmark_Base *bp);
  virtual int run_test (void);
  virtual int post_run_test (void);
  virtual int valid_test_object (Benchmark_Base *);

  static void *hold_lock (void * arg);
  // This method runs in a separate thread, and is used to hold the lock while
  // we test the performance of try lock.

private:
  Baseline_Test_Base *current_test_;
  ACE_Barrier get_lock_;
  ACE_Barrier let_go_lock_;
};

ACE_SVC_FACTORY_DECLARE (Baseline_Test)

#if defined (__ACE_INLINE__)
#include "Baseline_Test.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_BASELINE_TEST_H */
