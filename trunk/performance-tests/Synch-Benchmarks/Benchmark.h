/* -*- C++ -*- */
// $Id$

/* Defines the class used to dynamically link in the benchmark tests */

#if !defined (ACE_BENCHMARK_H)
#define ACE_BENCHMARK_H

#include "Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

extern int buffer;
extern ACE_Svc_Export int synch_count;

class ACE_Svc_Export Benchmark : public Benchmark_Base
{
  // TITLE
  //   Base class for all the timing tests.
public:
  // = Hooks inherited from ACE_Service_Object.
  virtual int svc (void);
  virtual int init (int, char *[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);
  static void *svc_run (Benchmark *bp);

  // = Set/get flag that controls how the tests are shut down
  // gracefully.
  static void done (sig_atomic_t);
  static sig_atomic_t done (void);

protected:
  static sig_atomic_t done_;
  // Keeps track if we are finished or not.
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_H */
