/* -*- C++ -*- */
/**
 *  @file  DSRT_CV_Dispatcher_Impl_T.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef DSRT_CV_DISPATCHER_IMPL_T_H
#define DSRT_CV_DISPATCHER_IMPL_T_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"
#include "ace/Copy_Disabled.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_dsrt.h"
#include "DSRT_Sched_Queue_T.h"
#include "DSRT_Dispatcher_Impl_T.h"
#include "ace/Hash_Map_Manager_T.h"

namespace Kokyu
{
  template<class DSRT_Scheduler_Traits>
  class DSRT_CV_Dispatcher_Impl :
    public DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>,
    public ACE_Copy_Disabled
  {
  public:
    typedef typename
    DSRT_Scheduler_Traits::Guid_t Guid_t;

    typedef typename
    DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

    DSRT_CV_Dispatcher_Impl (ACE_Sched_Params::Policy sched_policy,
                             int sched_scope);

    int init_i (const DSRT_ConfigInfo&);

    /// Schedule a thread dynamically based on the qos info supplied.
    int schedule_i (Guid_t, const DSRT_QoSDescriptor&);

    ///block a thread on the release_cond_, until the proper release time
    int release_guard_i (Guid_t, const DSRT_QoSDescriptor&);

    /// Update the schedule for a thread. This could alter the current
    /// schedule.
    int update_schedule_i (Guid_t, const DSRT_QoSDescriptor&);

    /// Inform the scheduler that the caller thread is about to
    /// block. This could alter the current schedule.
    int update_schedule_i (Guid_t, Block_Flag_t);

    /// Cancel the schedule for a thread. This could alter the current
    /// schedule.
    int cancel_schedule_i (Guid_t);

    /// Shut down the dispatcher. The dispatcher will stop processing
    /// requests.
    int shutdown_i ();

  private:
    class Guid_Hash
    {
    public:
      /// Returns hash value.
      u_long operator () (const typename DSRT_Scheduler_Traits::Guid_t &id)
      {
        typename DSRT_Scheduler_Traits::Guid_Hash guid_hash;
        return guid_hash(id);
      }
    };

//Maybe I need to change Guid_t to a task ID.
    typedef ACE_Hash_Map_Manager_Ex<int,
                                    TimeBase::TimeT,
                                    Guid_Hash,
                                    ACE_Equal_To<int>,
                                    ACE_SYNCH_NULL_MUTEX>
    Release_Time_Map;

    typedef ACE_SYNCH_MUTEX cond_lock_t;
    typedef ACE_SYNCH_CONDITION cond_t;

    cond_lock_t run_cond_lock_;
    cond_t run_cond_;
    cond_lock_t release_cond_lock_;
    cond_t release_cond_;
    Release_Time_Map release_map_;
  };

}

#if !defined (__ACE_INLINE__)
//#include "DSRT_CV_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_CV_Dispatcher_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_CV_Dispatcher_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* DSRT_DIRECT_DISPATCHER_IMPL_T_H */
