// -*- C++ -*-
//
// $Id$

namespace { enum { MAX_THREADPOOL_TASK_WORKER_THREADS = 50 }; }
namespace { const ACE_thread_t default_thread_id = 0; }

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Task::TP_Task()
  : work_available_(this->lock_),
    active_workers_(this->lock_),
    accepting_requests_(false),
    shutdown_initiated_(false),
    opened_(false),
    num_threads_(0),
    activated_threads_ ((size_t)MAX_THREADPOOL_TASK_WORKER_THREADS, default_thread_id)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
