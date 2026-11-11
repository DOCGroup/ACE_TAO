// -*- C++ -*-
#ifndef ACE_BASELINE_TEST_H
#define ACE_BASELINE_TEST_H

#include <cstddef>

#include "Synch_Lib/Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Profile_Timer.h"
#include "ace/svc_export.h"
#include "ace/Barrier.h"

constexpr std::size_t DEFAULT_ITERATIONS = 1000000;

enum class Baseline_Yield_Method
{
  sleep_zero,
  thread_yield
};

class ACE_Svc_Export Baseline_Test_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Performance Test class.
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

  int init (int argc, ACE_TCHAR *argv[]) override;
  // Standard initializing method for Baseline Test.

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parsing the svc.conf file arguments.

  virtual int acquire () = 0;
  virtual int release () = 0;
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release () = 0;
  virtual int test_try_lock () = 0;
  // Real test methods.

  virtual Baseline_Yield_Method yield_method () const;
  // Query the yield method used.

  virtual void yield () const;
  // Yield to other thread.

  std::size_t iteration () const;
  // Access methods.

protected:
  Baseline_Test_Base ();

  Baseline_Yield_Method yield_method_ {Baseline_Yield_Method::sleep_zero};
  // Should we your thr_yield or sleep (0).

  std::size_t iteration_ {DEFAULT_ITERATIONS};
  // Total number of operations.  <iterations_>

  Lock_Type what_ {Lock_Type::lock};
  // What test should be performed?
};

class ACE_Svc_Export Baseline_Test_Options
{
  // = TITLE
  //    This class holds the global settings for Baseline Test.
public:
  friend class Baseline_Test;

  Baseline_Test_Options (Baseline_Test_Options const &) = delete;
  Baseline_Test_Options (Baseline_Test_Options &&) = delete;
  Baseline_Test_Options &operator= (Baseline_Test_Options const &) = delete;
  Baseline_Test_Options &operator= (Baseline_Test_Options &&) = delete;

  Baseline_Test_Options () = default;

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse and set the Baseline_Test options and flags.

  void reset_params (std::size_t iteration, Baseline_Yield_Method yield);
  // Reset test parameters for next round.

  bool test_try_lock () const;
  // Return test configuration.

  void start_inc_timer ();
  void stop_inc_timer ();
  // Start/stop measuring time.

  bool inc_loop_counter ();

  void print_result ();
  // Print out the result.

private:
  bool test_try_lock_ {};
  // A flag indicates whether we are testing try_lock or lock and
  // release.

  bool verbose_ {};
  // Print out the result in verbose mode.

  Baseline_Yield_Method current_yield_method_ {Baseline_Yield_Method::sleep_zero};
  // yield or sleep.

  std::size_t current_iteration_ {};
  // Number of iteration.

  std::size_t total_iteration_ {DEFAULT_ITERATIONS};
  // Total number of target iteration.

  ACE_High_Res_Timer timer;
  // Profile timer result.
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

  static ACE_THR_FUNC_RETURN hold_lock (void *arg);
  // This method runs in a separate thread, and is used to hold the lock while
  // we test the performance of try lock.

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
