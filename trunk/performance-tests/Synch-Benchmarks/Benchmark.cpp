// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

// Global variables (used by the dynamically linked services).
ACE_Svc_Export int synch_count;
int buffer;

// Initialize the static variables.
/* static */
sig_atomic_t Benchmark::done_ = 0;

sig_atomic_t
Benchmark::done (void)
{
  return Benchmark::done_;
}

void
Benchmark::done (sig_atomic_t d)
{
  Benchmark::done_ = d;
}

int
Benchmark::svc (void)
{
  return -1;
}

int
Benchmark::init (int, char **)
{
  return 1;
}

int
Benchmark::info (char **, size_t) const
{
  return -1;
}

int
Benchmark::fini (void)
{
  return -1;
}

void *
Benchmark::svc_run (Benchmark *bp)
{
  return (void *) (bp->svc () == -1 ? -1 : 0);
}

#endif /* ACE_HAS_THREADS */
