// $Id$

#ifndef DSRT_DIRECT_DISPATCHER_IMPL_T_CPP
#define DSRT_DIRECT_DISPATCHER_IMPL_T_CPP

#include "DSRT_Direct_Dispatcher_Impl_T.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
//#include "DSRT_Direct_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, DSRT_Direct_Dispatcher_Impl_T, "$Id$")

namespace Kokyu
{
template <class DSRT_Scheduler_Traits>
int Comparator_Adapter_Generator<DSRT_Scheduler_Traits>::MoreEligible::
operator ()(const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item1,
            const DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item2)
{
  int rc = qos_comparator_ (item1->qos (), item2->qos ());

  //more eligible
  if (rc == 1)
    return 1;

  //if equally eligible, then resolve tie with the creation time of
  //the item
  if (rc == 0 && item1->insertion_time () < item2->insertion_time ())
    return 1;

  return 0;
}

template <class DSRT_Scheduler_Traits>
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
DSRT_Direct_Dispatcher_Impl ()
  :min_prio_ (ACE_Sched_Params::priority_min
              (ACE_SCHED_FIFO,
               ACE_SCOPE_THREAD)),
   max_prio_ (ACE_Sched_Params::priority_max
              (ACE_SCHED_FIFO,
               ACE_SCOPE_THREAD)),
   executive_prio_ (max_prio_),
   blocked_prio_ (ACE_Sched_Params::previous_priority
                  (ACE_SCHED_FIFO,
                   max_prio_,
                   ACE_SCOPE_THREAD)),
   inactive_prio_ (min_prio_),
   active_prio_ (ACE_Sched_Params::next_priority
                 (ACE_SCHED_FIFO,
                  min_prio_)),
   curr_scheduled_thr_handle_ (0),
   sched_queue_modified_ (0),
   sched_queue_modified_cond_ (sched_queue_modified_cond_lock_),
   shutdown_flagged_ (0)
{

  long flags =
    THR_NEW_LWP |
    THR_JOINABLE |
    THR_BOUND |
    THR_SCHED_FIFO;

  //Run scheduler thread at highest priority
  if (this->activate (flags, 1, 0, executive_prio_) == -1)
    {
      flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
      if (this->activate (flags) == -1)
        ACE_ERROR ((LM_ERROR,
                    "EC (%P|%t) cannot activate scheduler thread\n"));
    }
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
init_i (const DSRT_ConfigInfo&)
{
  return 0;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::svc (void)
{
  int prio;
  ACE_hthread_t scheduler_thr_handle;
  ACE_Thread::self (scheduler_thr_handle);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("max prio=%d\n")
              ACE_TEXT ("min prio=%d\n")
              ACE_TEXT ("active prio=%d\n")
              ACE_TEXT ("inactive prio=%d\n"),
              max_prio_,
              min_prio_,
              active_prio_,
              inactive_prio_));

  if (ACE_Thread::getprio (scheduler_thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")
                     ));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t): Scheduler thread prio is %d\n", prio));

  while(1)
    {
      ACE_GUARD_RETURN (cond_lock_t,
                        mon, sched_queue_modified_cond_lock_, 0);

      if (shutdown_flagged_)
        break;

      while (!sched_queue_modified_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t): sched thread about to wait on cv\n"));
          sched_queue_modified_cond_.wait ();
        }
      ACE_DEBUG ((LM_DEBUG, "(%t): sched thread done waiting on cv\n"));

      sched_queue_modified_ = 0;

      if (ready_queue_.current_size () <= 0)
        continue;

      ready_queue_.dump ();
      DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> item_var;
      ready_queue_.most_eligible (item_var);

      ACE_hthread_t most_eligible_thread = item_var->thread_handle ();
      /*
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t): most eligible thread guid = %d\n"),
                  item_var->guid ()));
      */
      if (curr_scheduled_thr_handle_ != most_eligible_thread)
        {
          ACE_OS::thr_setprio (curr_scheduled_thr_handle_,
                               inactive_prio_);
          ACE_OS::thr_setprio (most_eligible_thread, active_prio_);
          curr_scheduled_thr_handle_ = most_eligible_thread;
          curr_scheduled_guid_ = item_var->guid ();
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t): sched thread exiting\n"));
  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
schedule_i (Guid_t id, const DSRT_QoSDescriptor& qos)
{
  //@@ Perhaps the lock could be moved further down just before
  //setting the condition variable?
  ACE_GUARD_RETURN (cond_lock_t,
                    mon, sched_queue_modified_cond_lock_, 0);
  if (ready_queue_.insert (id, qos) == -1)
    return -1;

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  if (ACE_OS::thr_setprio (thr_handle, inactive_prio_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("thr_setprio failed")),
                        -1);
    }

  ready_queue_.dump ();

  sched_queue_modified_ = 1;
  sched_queue_modified_cond_.signal ();
  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule_i (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  return this->schedule (guid, qos);
}

template <class DSRT_Scheduler_Traits>
int DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule_i (Guid_t guid, Block_Flag_t flag)
{
  ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block entered\n"));
  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> dispatch_item;
  ACE_hthread_t thr_handle;
  //@@ Perhaps the lock could be got rid of. It looks like the state
  //of this object is not getting modified here. It makes calls to
  //other methods, which already are thread-safe.
  ACE_Guard<cond_lock_t> mon(sched_queue_modified_cond_lock_);

  int found = this->ready_queue_.find (guid, dispatch_item);
  if (found == 0 && flag == BLOCK)
    {
      thr_handle = dispatch_item->thread_handle ();
      ACE_OS::thr_setprio (thr_handle, blocked_prio_);

      //monitor released because cancel_schedule would acquire the
      //lock. Using recursive mutex creates lock up.
      //
      //@@ Need to investigate this further. Also we can consider
      //using the Thread-Safe interface pattern.
      mon.release ();
      int rc = this->cancel_schedule (guid);
      ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block done\n"));
      return rc;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block done\n"));
  return -1;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
cancel_schedule_i (Guid_t guid)
{
  //@@ Perhaps the lock could be moved further down?
  ACE_GUARD_RETURN (cond_lock_t,
                    mon, sched_queue_modified_cond_lock_, 0);
  ready_queue_.dump ();
  ACE_DEBUG ((LM_DEBUG, "(%t): about to remove guid\n"));
  ready_queue_.remove (guid);
  ready_queue_.dump ();

  if (curr_scheduled_guid_ == guid)
    {
      curr_scheduled_guid_ = 0;
      curr_scheduled_thr_handle_ = 0;
    }

  sched_queue_modified_ = 1;
  sched_queue_modified_cond_.signal ();
  return 0;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
shutdown_i ()
{
  ACE_Guard<cond_lock_t> mon(sched_queue_modified_cond_lock_);
  shutdown_flagged_ = 1;
  sched_queue_modified_ = 1;
  sched_queue_modified_cond_.signal ();
  // We have to wait until the scheduler executive thread shuts
  // down. But we have acquired the lock and if we wait without
  // releasing it, the scheduler thread will try to acquire it after
  // it gets woken up by the above signal and it fails to acquire the
  // lock. This will lead to a deadlock. So release the lock before we
  // wait.
  mon.release ();
  this->wait ();
  return 0;
}

}

#endif /* DSRT_DIRECT_DISPATCHER_IMPL_T_CPP */
