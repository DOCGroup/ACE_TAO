/* -*- C++ -*- */
/**
 *  @file  DSRT_Direct_Dispatcher_Impl_T.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef DSRT_DIRECT_DISPATCHER_IMPL_T_H
#define DSRT_DIRECT_DISPATCHER_IMPL_T_H
#include "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_dsrt.h"
#include "DSRT_Sched_Queue_T.h"
#include "DSRT_Dispatcher_Impl_T.h"

namespace Kokyu
{
  /**
   * @class Comparator_Adapter_Generator
   *
   * @brief Generates function object adapter that adapts the
   * QoSComparator function object to compare between two schedulable
   * items instead of QoSDescriptors.
   *
   * The QoSComparator function object that gets passed through the
   * <code> DSRT_Scheduler_Traits </code> takes two qos values and
   * determines the more eligible one. Since the INT_ID (key) for
   * RB_Tree needs to be of type <code> DSRT_Dispatch_Item_var
   * </code>, the QoSComparator needs to be adapted using an adapter
   * to compare two schedulable items. This adapter compares the two
   * using their qos values. Ties are resolved by giving preference to
   * items which arrived earlier. Note that this class serves the
   * purpose of a generator class, since it generates the adapter
   * class for a given qos comparator function object.
   */

  template <class DSRT_Scheduler_Traits>
  class Comparator_Adapter_Generator
  {
  public:
    typedef typename
    DSRT_Scheduler_Traits::QoSComparator_t QoSComparator_t;

    /**
     * @class More_Eligible
     *
     * @brief Actual function object that gets generated.
     */
    class MoreEligible
    {
    public:
      /**
       * Function call operator to do comparison between two
       * schedulable items. Returns 1 if item1 is more eligible than
       * item2, otherwise 0.
       */
      int operator ()
        (const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item1,
         const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item2);

    private:
      QoSComparator_t qos_comparator_;
    };

    /**
     * Facilitates return of the generated function object adapter.
     */
    typedef MoreEligible RET;
  };

  template<class DSRT_Scheduler_Traits>
  class DSRT_Direct_Dispatcher_Impl :
    public ACE_Task_Base,
    public DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>,
    public non_copyable
  {
  public:
    typedef typename
    DSRT_Scheduler_Traits::Guid_t Guid_t;

    typedef typename
    DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

    DSRT_Direct_Dispatcher_Impl ();

    int init_i (const DSRT_ConfigInfo&);

    /// Schedule a thread dynamically based on the qos info supplied.
    int schedule_i (Guid_t, const DSRT_QoSDescriptor&);

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

    /// Generate the QoSComparator adapter.
    typedef typename
    Comparator_Adapter_Generator<DSRT_Scheduler_Traits>::RET
    Queue_Item_Comparator_t;

    typedef Sched_Ready_Queue<DSRT_Scheduler_Traits,
                              Queue_Item_Comparator_t,
                              ACE_SYNCH_NULL_MUTEX>
    DSRT_Sched_Queue;

    typedef ACE_SYNCH_MUTEX cond_lock_t;
    typedef ACE_SYNCH_CONDITION cond_t;

    Priority_t min_prio_;
    Priority_t max_prio_;
    Priority_t executive_prio_;
    Priority_t blocked_prio_;
    Priority_t inactive_prio_;
    Priority_t active_prio_;
    ACE_hthread_t curr_scheduled_thr_handle_;
    Guid_t curr_scheduled_guid_;
    DSRT_Sched_Queue ready_queue_;
    u_int sched_queue_modified_;
    cond_lock_t sched_queue_modified_cond_lock_;
    cond_t sched_queue_modified_cond_;
    int shutdown_flagged_;

  private:
    int svc (void);
  };

}

#if !defined (__ACE_INLINE__)
//#include "DSRT_Direct_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_Direct_Dispatcher_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_Direct_Dispatcher_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* DSRT_DIRECT_DISPATCHER_IMPL_T_H */
