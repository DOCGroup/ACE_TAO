/* -*- C++ -*- */
/* Defines the class used to dynamically link in the benchmark tests */

#ifndef ACE_BENCHMARK_PERFORMANCE_H
#define ACE_BENCHMARK_PERFORMANCE_H

#include "Synch_Lib/Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

#include "ace/svc_export.h"

extern int buffer;
extern ACE_Svc_Export int synch_count;

/// Base class for performance benchmark tests.
class ACE_Svc_Export Benchmark_Performance_Test_Base : public Benchmark_Base
{
protected:
  Benchmark_Performance_Test_Base ();

  Benchmark_Performance_Test_Base (Benchmark_Performance_Test_Base const &) = delete;
  Benchmark_Performance_Test_Base (Benchmark_Performance_Test_Base &&) = delete;
  Benchmark_Performance_Test_Base &operator= (Benchmark_Performance_Test_Base const &) = delete;
  Benchmark_Performance_Test_Base &operator= (Benchmark_Performance_Test_Base &&) = delete;
};

/// Base class for timing tests.
class ACE_Svc_Export Benchmark_Performance : public Benchmark_Performance_Test_Base
{
public:
  /// @name Hooks inherited from ACE_Service_Object
  /// @{
  virtual int svc () = 0;
  int init (int, ACE_TCHAR *[]) override;
  int info (ACE_TCHAR **, size_t) const override;
  int fini () override;
  /// @}

  static void *svc_run (Benchmark_Performance *bp);

  /// Sets or gets the flag that controls graceful test shutdown.
  static void done (sig_atomic_t);
  static sig_atomic_t done ();

protected:
  /// Tracks whether the test has finished.
  static sig_atomic_t done_;
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_PERFORMANCE_H */
