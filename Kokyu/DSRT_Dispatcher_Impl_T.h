/* -*- C++ -*- */
/**
 *  @file   DSRT_Dispatcher_Impl_T.h
 *
 *  $Id$
 *
 */

#ifndef DSRT_DISPATCHER_IMPL_H
#define DSRT_DISPATCHER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/Synch_Traits.h"
#if defined (ACE_HAS_THREADS)
#  include "ace/Recursive_Thread_Mutex.h"
#else
#  include "ace/Null_Mutex.h"
#endif /* ACE_HAS_THREADS */

#include "Kokyu_dsrt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



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
         const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item2)
      {
        int rc = qos_comparator_ (item1->qos (), item2->qos ());

#ifdef KOKYU_DSRT_LOGGING
        ACE_DEBUG ((LM_DEBUG,
                    "(%t|%T): MoreEligible:: qos_comparator returned %d\n",
                    rc));
#endif

        //more eligible
        if (rc == 1)
          return rc;

        //if equally eligible, then resolve tie with the creation time of
        //the item
        if (rc == 0 && item1->insertion_time () < item2->insertion_time ())
          return 1;

        return 0;
      }

    private:
      QoSComparator_t qos_comparator_;
    };

    /**
     * Facilitates return of the generated function object adapter.
     */
    typedef MoreEligible RET_FUNC;
  };

  /**
   * @class DSRT_Dispatcher
   *
   * @brief Base class for DSRT dispatcher implementations
   *
   * The responsibility of this class is to act as a common base class
   * for different DSRT dispatcher implementations. This is an
   * abstract base class and cannot be instantiated.
   */
  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatcher_Impl
  {
  public:
    typedef typename DSRT_Scheduler_Traits::Guid_t Guid_t;
    typedef typename DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

    DSRT_Dispatcher_Impl (ACE_Sched_Params::Policy sched_policy,
                          int sched_scope);

    /// Configure the DSRT dispatcher.
    int init (const DSRT_ConfigInfo&);

    /// Schedule a thread dynamically based on the qos info supplied.
    int schedule (Guid_t guid,
                  const DSRT_QoSDescriptor&);

    /// Update the schedule for a thread. This could alter the current
    /// schedule.
    int update_schedule (Guid_t guid,
                         const DSRT_QoSDescriptor&);

    /// Inform the scheduler that the caller thread is about to
    /// block. This could alter the current schedule.
    int update_schedule (Guid_t guid, Block_Flag_t flag);

    /// Cancel the schedule for a thread. This could alter the current
    /// schedule.
    int cancel_schedule (Guid_t guid);

    /// Shut down the dispatcher. The dispatcher will stop processing
    /// requests.
    int shutdown ();

    virtual ~DSRT_Dispatcher_Impl ();

  private:

    //following an idiom to avoid public virtual functions.
    //instead make them private and use the template method
    //pattern - "Virtually Yours" article in CUJ Experts Forum

    virtual int init_i (const DSRT_ConfigInfo&)=0;
    virtual int schedule_i (Guid_t guid,
                    const DSRT_QoSDescriptor&)=0;
    virtual int update_schedule_i (Guid_t guid,
                           const DSRT_QoSDescriptor&)=0;
    virtual int update_schedule_i (Guid_t guid, Block_Flag_t flag)=0;
    virtual int cancel_schedule_i (Guid_t guid)=0;
    virtual int shutdown_i ()=0;

  protected:
    /// Generate the QoSComparator adapter.
    typedef typename
    Comparator_Adapter_Generator<DSRT_Scheduler_Traits>::RET_FUNC
    Queue_Item_Comparator_t;

    typedef Sched_Ready_Queue<DSRT_Scheduler_Traits,
                              Queue_Item_Comparator_t,
                              ACE_SYNCH_NULL_MUTEX>
    DSRT_Sched_Queue_t;

    ACE_Sched_Params::Policy sched_policy_;
    int sched_scope_;

    Priority_t min_prio_;
    Priority_t max_prio_;
    Priority_t executive_prio_;
    Priority_t blocked_prio_;
    Priority_t inactive_prio_;
    Priority_t active_prio_;

    DSRT_Sched_Queue_t ready_queue_;
    int shutdown_flagged_;
    long non_rt_thr_flags_;
    long rt_thr_flags_;

    ACE_SYNCH_RECURSIVE_MUTEX synch_lock_;

    ACE_hthread_t curr_scheduled_thr_handle_;
    Guid_t curr_scheduled_guid_;
  };

} //end of namespace

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_Dispatcher_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_Dispatcher_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* DSRT_DISPATCHER_IMPL_H */
