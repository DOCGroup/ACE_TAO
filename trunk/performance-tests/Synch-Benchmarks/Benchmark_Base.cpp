// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

// Initialize the static variables.
/* static */

Benchmark_Base::Benchmark_Base (void)
  : benchmark_type_ (Benchmark_Base::BENCHMARK_BASE)
{
}

int
Benchmark_Base::benchmark_type (void)
{
  return this->benchmark_type_;
}

int
Benchmark_Base::thr_id (void)
{
#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
  // This invokes the thread-specific storage smart pointer.
  return this->id_->thr_id ();
#else
  return ACE_Thread::self ();
#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
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
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Thr_ID>
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
#endif /* ACE_HAS_THREADS */
