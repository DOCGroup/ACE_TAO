// -*- C++ -*-
#ifndef ACE_BASELINE_TEST_H
#define ACE_BASELINE_TEST_H

#include <cstddef>

#include "Synch_Lib/Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Profile_Timer.h"
#include "ace/svc_export.h"
#include "ace/Barrier.h"

constexpr size_t DEFAULT_ITERATIONS = 1000000;

enum class Baseline_Yield_Method
{
  sleep_zero,
  thread_yield
};

/// Base class for baseline performance tests.
class ACE_Svc_Export Baseline_Test_Base : public Benchmark_Base
{
public:
  enum class Lock_Type
  {
    lock,
    read_lock,
    write_lock
  };

  Baseline_Test_Base (Baseline_Test_Base const &) = delete;
  Baseline_Test_Base (Baseline_Test_Base &&) = delete;
  Baseline_Test_Base &operator= (Baseline_Test_Base const &) = delete;
  Baseline_Test_Base &operator= (Baseline_Test_Base &&) = delete;

  /// Initializes the baseline test.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Parses arguments from the service configuration.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Acquires the lock type selected for the test.
  virtual int acquire () = 0;
  /// Releases the lock type selected for the test.
  virtual int release () = 0;

  /// Runs the acquire/release performance test.
  virtual int test_acquire_release () = 0;
  /// Runs the try-lock performance test.
  virtual int test_try_lock () = 0;

  /// Returns the yield method used by the test.
  virtual Baseline_Yield_Method yield_method () const;

  /// Yields execution to another thread.
  virtual void yield () const;

  /// Returns the number of iterations used by the test.
  size_t iteration () const;

protected:
  Baseline_Test_Base ();

  /// Selects whether to call thr_yield or sleep (0).
  Baseline_Yield_Method yield_method_ {Baseline_Yield_Method::sleep_zero};

  /// Total number of operations to run.
  size_t iteration_ {DEFAULT_ITERATIONS};

  /// Lock operation measured by the test.
  Lock_Type what_ {Lock_Type::lock};
};

/// Holds global settings for the baseline test.
class ACE_Svc_Export Baseline_Test_Options
{
public:
  friend class Baseline_Test;

  Baseline_Test_Options (Baseline_Test_Options const &) = delete;
  Baseline_Test_Options (Baseline_Test_Options &&) = delete;
  Baseline_Test_Options &operator= (Baseline_Test_Options const &) = delete;
  Baseline_Test_Options &operator= (Baseline_Test_Options &&) = delete;

  Baseline_Test_Options () = default;

  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Resets test parameters for the next round.
  void reset_params (size_t iteration, Baseline_Yield_Method yield);

  /// Returns whether the test measures try-lock performance.
  bool test_try_lock () const;

  /// Starts and stops timing each increment operation.
  void start_inc_timer ();
  void stop_inc_timer ();

  bool inc_loop_counter ();

  /// Prints the measured results.
  void print_result ();

private:
  /// Selects whether to measure try_lock or lock/release.
  bool test_try_lock_ {};

  /// Enables verbose result output.
  bool verbose_ {};

  /// Yield method used in the current round.
  Baseline_Yield_Method current_yield_method_ {Baseline_Yield_Method::sleep_zero};

  /// Number of iterations completed in the current round.
  size_t current_iteration_ {};

  /// Total number of iterations requested for the run.
  size_t total_iteration_ {DEFAULT_ITERATIONS};

  /// Measures elapsed time for the current run.
  ACE_High_Res_Timer timer;
};

extern Baseline_Test_Options baseline_options;

class ACE_Svc_Export Baseline_Test : public Benchmark_Method_Base
{
public:
  Baseline_Test (Baseline_Test const &) = delete;
  Baseline_Test (Baseline_Test &&) = delete;
  Baseline_Test &operator= (Baseline_Test const &) = delete;
  Baseline_Test &operator= (Baseline_Test &&) = delete;

  Baseline_Test ();
  int init (int argc, ACE_TCHAR *argv[]) override;
  int pre_run_test (Benchmark_Base *bp) override;
  int run_test () override;
  int post_run_test () override;
  int valid_test_object (Benchmark_Base *) override;

  /// Holds the lock from another thread during the try-lock test.
  static ACE_THR_FUNC_RETURN hold_lock (void *arg);

private:
  // The service repository owns the test object; this pointer is non-owning.
  Baseline_Test_Base *current_test_ {};
  ACE_Barrier get_lock_;
  ACE_Barrier let_go_lock_;
};

ACE_SVC_FACTORY_DECLARE (Baseline_Test)

#if defined (__ACE_INLINE__)
#include "Baseline_Test.inl"
#endif /* __ACE_INLINE__ */

#endif  /* ACE_HAS_THREADS */

#endif /* ACE_BASELINE_TEST_H */
