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

int
Benchmark::thr_id (void)
{
#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
  // This invokes the thread-specific storage smart pointer.
  return this->id_->thr_id ();
#else
  return ACE_Thread::self ();
#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
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
  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());
  return (void *) (bp->svc () == -1 ? -1 : 0);
}

#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
/* static */
MT_INT Thr_ID::thread_id_ (0);

Thr_ID::Thr_ID (void)
  : thr_id_ (++Thr_ID::thread_id_)
{
}

int
Thr_ID::thr_id (void)
{
  return this->thr_id_;
}

void
Thr_ID::thr_id (int i)
{
  this->thr_id_ = i;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Thr_ID>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Thr_ID>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
#endif /* ACE_HAS_THREADS */
