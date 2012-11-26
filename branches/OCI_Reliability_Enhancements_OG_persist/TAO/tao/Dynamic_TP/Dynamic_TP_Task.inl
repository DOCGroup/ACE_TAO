// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Dynamic_TP_Task::TAO_Dynamic_TP_Task()
  : work_available_(this->lock_),
    active_workers_(this->lock_),
    accepting_requests_(false),
    shutdown_initiated_(false),
    deferred_shutdown_initiated_(false),
    opened_(false),
    num_threads_(0),
    activated_threads_ ((size_t)0)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
