// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Benchmark_Performance.h"
#include "ace/Basic_Types.h"

#if defined (ACE_HAS_THREADS)

// Global variables (used by the dynamically linked services).
ACE_Svc_Export int synch_count;
int buffer;

// Initialize the static variables.
/* static */
sig_atomic_t Benchmark_Performance::done_ = 0;

Benchmark_Performance_Test_Base::Benchmark_Performance_Test_Base (void)
  : Benchmark_Base (Benchmark_Base::PERFORMANCE)
{
}

sig_atomic_t
Benchmark_Performance::done (void)
{
  return Benchmark_Performance::done_;
}

void
Benchmark_Performance::done (sig_atomic_t d)
{
  Benchmark_Performance::done_ = d;
}

int
Benchmark_Performance::init (int, ACE_TCHAR **)
{
  return 1;
}

int
Benchmark_Performance::info (ACE_TCHAR **, size_t) const
{
  return -1;
}

int
Benchmark_Performance::fini (void)
{
  return -1;
}

void *
Benchmark_Performance::svc_run (Benchmark_Performance *bp)
{
  return (void *) (bp->svc () == -1 ? (intptr_t) -1 : (intptr_t) 0);
}

#endif /* ACE_HAS_THREADS */
