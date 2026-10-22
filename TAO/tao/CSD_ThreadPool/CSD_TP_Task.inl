// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Task::TP_Task()
  : work_available_(this->lock_),
    active_workers_(this->lock_),
    activated_threads_ ((size_t)MAX_THREADPOOL_TASK_WORKER_THREADS)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
