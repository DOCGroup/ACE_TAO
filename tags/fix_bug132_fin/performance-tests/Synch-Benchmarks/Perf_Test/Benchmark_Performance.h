/* -*- C++ -*- */
// $Id$

/* Defines the class used to dynamically link in the benchmark tests */

#ifndef ACE_BENCHMARK_PERFORMANCE_H
#define ACE_BENCHMARK_PERFORMANCE_H

#include "Synch_Lib/Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

extern int buffer;
extern ACE_Svc_Export int synch_count;

class ACE_Svc_Export Benchmark_Performance_Test_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Performance Test class.
protected:
  Benchmark_Performance_Test_Base (void);
};

class ACE_Svc_Export Benchmark_Performance : public Benchmark_Performance_Test_Base
{
  // = TITLE
  //   Base class for all the timing tests.
public:
  // = Hooks inherited from ACE_Service_Object.
  virtual int svc (void) = 0;
  virtual int init (int, char *[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);
  static void *svc_run (Benchmark_Performance *bp);

  // = Set/get flag that controls how the tests are shut down
  // gracefully.
  static void done (sig_atomic_t);
  static sig_atomic_t done (void);

protected:
  static sig_atomic_t done_;
  // Keeps track if we are finished or not.
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_PERFORMANCE_H */
