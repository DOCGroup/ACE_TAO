/* -*- C++ -*- */
/**
 *  @file  DSRT_Schedulers.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef DSRT_SCHEDULERS_H
#define DSRT_SCHEDULERS_H
#include "ace/pre.h"
#include "ace/OS.h"

#include "Kokyu.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Kokyu
{
  class DSRT_Scheduler_Impl
  {
  public:
    DSRT_Scheduler_Impl (ACE_Sched_Params::Policy sched_policy, int sched_scope);
    int schedule (guid_t, const QoSDescriptor&);

  private:
    virtual Priority_t schedule_i (guid_t, const QoSDescriptor&) = 0;

  protected:
    ACE_Sched_Params::Policy sched_policy_;
    int sched_scope_;
    Priority_t min_prio_;
    Priority_t max_prio_;
 };

  class MIF_Scheduler_Impl : public DSRT_Scheduler_Impl
  {
  public:
    MIF_Scheduler_Impl (ACE_Sched_Params::Policy sched_policy, int sched_scope);

  private:
    virtual Priority_t schedule_i (guid_t, const QoSDescriptor&);
  };
}

#if defined (__ACE_INLINE__)
//#include "DSRT_Schedulers.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* DSRT_SCHEDULERS_H */
