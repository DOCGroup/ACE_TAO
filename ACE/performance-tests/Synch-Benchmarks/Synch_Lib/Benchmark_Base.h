/* -*- C++ -*- */
/* Defines the base class used to dynamically link in the benchmark tests */

#ifndef ACE_BENCHMARK_BASE_H
# define ACE_BENCHMARK_BASE_H

# include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/Service_Repository.h"
# include "ace/Service_Types.h"
# include "ace/Atomic_Op.h"
# include "export_mac.h"
# include "ace/TSS_T.h"

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)

using MT_INT = ACE_Atomic_Op<ACE_Thread_Mutex, int>;

/// Provides a thread-specific value to compensate for POSIX Pthreads.
class Thr_ID
{
public:
  Thr_ID (Thr_ID const &) = delete;
  Thr_ID (Thr_ID &&) = delete;
  Thr_ID &operator= (Thr_ID const &) = delete;
  Thr_ID &operator= (Thr_ID &&) = delete;

  Thr_ID ();
  int thr_id () const;
  void thr_id (int const);

private:
  int thr_id_;
  static MT_INT thread_id_;
};
#   endif /* ACE_HAS_PTHREADS || VXWORKS */

/**
 * Base class for benchmarking objects.
 *
 * Provides RTTI information and common operations supported by benchmark
 * classes.
 */
class SYNCHLIB_Export Benchmark_Base : public ACE_Service_Object
{
public:
  enum {
    BENCHMARK_BASE,
    METHOD,
    BASELINE,
    PERFORMANCE
  };

  Benchmark_Base (Benchmark_Base const &) = delete;
  Benchmark_Base (Benchmark_Base &&) = delete;
  Benchmark_Base &operator= (Benchmark_Base const &) = delete;
  Benchmark_Base &operator= (Benchmark_Base &&) = delete;

  /// Returns the benchmark type.
  int benchmark_type () const;

  /// Returns the thread identifier.
  int thr_id () const;

protected:
  explicit Benchmark_Base (int const type = BENCHMARK_BASE);

  /// RTTI value identifying this benchmark module.
  int const benchmark_type_;

#   if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)
  /// Tracks this thread's virtual benchmark identifier.
  ACE_TSS<Thr_ID> id_;
#   endif /* ACE_HAS_PTHREADS || VXWORKS */
};

/// Identifies and configures a benchmark method.
class SYNCHLIB_Export Benchmark_Method_Base : public Benchmark_Base
{
public:
  Benchmark_Method_Base (Benchmark_Method_Base const &) = delete;
  Benchmark_Method_Base (Benchmark_Method_Base &&) = delete;
  Benchmark_Method_Base &operator= (Benchmark_Method_Base const &) = delete;
  Benchmark_Method_Base &operator= (Benchmark_Method_Base &&) = delete;

  int exec (ACE_Service_Repository_Iterator *sri);

  /// Prepares a test before it runs.
  virtual int pre_run_test (Benchmark_Base *bp) = 0;

  /// Runs the test.
  virtual int run_test () = 0;

  /// Cleans up after the test runs.
  virtual int post_run_test () = 0;

  /// Checks whether a benchmark object is valid for this test.
  virtual int valid_test_object (Benchmark_Base *) = 0;

protected:
  Benchmark_Method_Base ();
};

# endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_BASE_H */
