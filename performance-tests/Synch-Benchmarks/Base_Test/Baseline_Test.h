// -*- C++ -*-
// $Id$

#ifndef ACE_BASELINE_TEST_H
#define ACE_BASELINE_TEST_H

#include "Benchmark_Base.h"

class ACE_Svc_Export Benchmark_Baseline_Test_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Performance Test class.
public:
  virtual int init (int argc, char *argv[]);
  // Standard initializing method for Baseline Test.

  virtual int acquire () = 0;
  virtual int release () = 0;
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release () = 0;
  virtual int test_try_lock () = 0;
  // Real test methods.

protected:
  Benchmark_Baseline_Test_Base (void);
};

class ACE_Svc_Export Baseline_Options
{
public:
  Baseline_Options (void);
  // ctor.

  int parse_method_args (int argc, char *argv[]);
  // Parse and set the Benchmark_Baseline options and flags.

  int parse_test_args (int argc, char *argv[]);
  // Parse and set the

  int test_try_lock (void);
  // Return test configuration.

  int add_time (ACE_Profile_Timer::Elapsed_Time &et);
  // Add the time spent in the current iterations.

  int inc_loop_counter (void);
  // Added multiply_factor_ to total_iteration_.

  size_t multiply_factor (void);
  // Return <multiply_factor_>.

  size_t iteration (void);
  // Return <iteration_>.

  size_t print_result (void);
  // Print out the result.

private:
  int test_try_lock_;
  // A flag indicates whether we are testing try_lock or lock and
  // release.

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

extern Baseline_Options baseline_options;

class ACE_Svc_Export Benchmark_Baseline : public Benchmark_Method_Base
{
public:
  Benchmark_Baseline (void);
  int init (int argc, char **argv);
  virtual int pre_run_test (Benchmark_Base *bp);
  virtual int run_test (void);
  virtual int post_run_test (void);
  virtual int valid_test_object (Benchmark_Base *);

  virtual int svc (void);
  // This method is responsible to hold the lock if we are
  // benchmarking for try_lock method.

private:
  ACE_Barrier get_lock_;
  ACE_Barrier let_go_lock_;
};

ACE_SVC_FACTORY_DECLARE (Baseline_Test)

#if defined (__ACE_INLINE__)
#include "ace/Baseline_Test.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_BASELINE_TEST_H */
