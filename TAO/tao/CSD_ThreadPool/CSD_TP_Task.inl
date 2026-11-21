// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Task::TP_Task()
  : work_available_(this->lock_),
    active_workers_(this->lock_)
{
  this->activated_threads_.reserve (MAX_THREADPOOL_TASK_WORKER_THREADS);
}

TAO_END_VERSIONED_NAMESPACE_DECL
