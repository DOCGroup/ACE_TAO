// $Id$

#ifndef DSRT_DIRECT_DISPATCHER_IMPL_T_CPP
#define DSRT_DIRECT_DISPATCHER_IMPL_T_CPP

#include "DSRT_Direct_Dispatcher_Impl_T.h"

#if !defined (__ACE_INLINE__)
//#include "DSRT_Direct_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{

/*
//@@VS: This is somehow not being recognized by MSVC, which results
//in a link error. For now, the definition has been moved to the .h
//file. Needs further investigation.

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
*/
template <class DSRT_Scheduler_Traits>
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
DSRT_Direct_Dispatcher_Impl (ACE_Sched_Params::Policy sched_policy,
                         int sched_scope)
  :DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>(sched_policy, sched_scope),
   sched_queue_modified_ (0),
   sched_queue_modified_cond_ (sched_queue_modified_cond_lock_)
{
  //Run scheduler thread at highest priority
  if (this->activate (this->rt_thr_flags_, 1, 0, this->executive_prio_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%t|%T) cannot activate scheduler thread in RT mode."
                  "Trying in non RT mode\n"));
      if (this->activate (this->non_rt_thr_flags_) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t|%T) cannot activate scheduler thread\n"));
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
  ACE_hthread_t scheduler_thr_handle;
  ACE_Thread::self (scheduler_thr_handle);

#ifdef KOKYU_DSRT_LOGGING
  int prio;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("max prio=%d\n")
              ACE_TEXT ("min prio=%d\n")
              ACE_TEXT ("active prio=%d\n")
              ACE_TEXT ("inactive prio=%d\n"),
              max_prio_,
              min_prio_,
              active_prio_,
              inactive_prio_));

  if (ACE_OS::thr_getprio (scheduler_thr_handle, prio) == -1)
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
#endif /*DSRT_LOGGING*/

  while(1)
    {
      ACE_GUARD_RETURN (cond_lock_t,
                        mon,
                        sched_queue_modified_cond_lock_,
                        0);

      if (this->shutdown_flagged_)
        break;

      while (!sched_queue_modified_)
        {
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG ((LM_DEBUG,
                      "(%t|%T): sched thread about to wait on cv\n"));
#endif
          sched_queue_modified_cond_.wait ();
        }

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t|%T): sched thread done waiting on cv\n"));
#endif

      sched_queue_modified_ = 0;

      ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                        synch_lock_mon,
                        this->synch_lock_,
                        0);

      if (this->ready_queue_.current_size () <= 0)
        continue;

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):Sched Queue contents===>\n"));
      this->ready_queue_.dump ();
#endif
      DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> item_var;
      this->ready_queue_.most_eligible (item_var);

      ACE_hthread_t most_eligible_thr_handle = item_var->thread_handle ();

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T):curr scheduled thr handle = %d\n",
                  this->curr_scheduled_thr_handle_));
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T):most eligible thr handle = %d\n",
                  most_eligible_thr_handle));
#endif

      if (this->curr_scheduled_thr_handle_ != most_eligible_thr_handle)
        {
          if (this->curr_scheduled_thr_handle_ != 0)
            {
              if (ACE_OS::thr_setprio (this->curr_scheduled_thr_handle_,
                                       this->inactive_prio_,
                                       this->sched_policy_) == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("%p\n"),
                              ACE_TEXT ("thr_setprio on curr_scheduled_thr_handle_ failed.")));
                  ACE_DEBUG ((LM_DEBUG, "thr_handle = %d, prio = %d\n",
                              this->curr_scheduled_thr_handle_,
                              this->inactive_prio_));
                }
            }

          if (ACE_OS::thr_setprio (most_eligible_thr_handle,
                                   this->active_prio_, this->sched_policy_) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("thr_setprio on most_eligible_thr_handle failed")));
            }

          this->curr_scheduled_thr_handle_ = most_eligible_thr_handle;
          this->curr_scheduled_guid_ = item_var->guid ();
        }
          /*change all threads in blocked_prio_ to inactive_prio_*/
          this->ready_queue_.change_prio(this->blocked_prio_, this->inactive_prio_,this->sched_policy_);
    }

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t): sched thread exiting\n"));
#endif

  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
schedule_i (Guid_t id, const DSRT_QoSDescriptor& qos)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i enter\n"));
#endif

  DSRT_Dispatch_Item<DSRT_Scheduler_Traits>* item;
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  ACE_NEW_RETURN (item,
                  DSRT_Dispatch_Item<DSRT_Scheduler_Traits> (id, qos),
                  -1);
  item->thread_handle (thr_handle);

  if (this->ready_queue_.insert (item) == -1)
    return -1;

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i after ready_q.insert\n"));
#endif

  if (ACE_OS::thr_setprio (thr_handle,
                           this->blocked_prio_,
                           this->sched_policy_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("thr_setprio failed")), -1);
    }

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i after thr_setprio\n"));
#endif

  //ready_queue_.dump ();

  /*first release ready_queue_ lock. Otherwise if the scheduler gets the
  sched_queue_modified_cond_lock first, then try to get the ready_queue_ lock
  just when one thread who gets the ready_queue_ lock first, then try to get
  sched_queue_modified_cond_lock. Deadlock happens.
  */
  guard.release ();

  //@@ Perhaps the lock could be moved further down just before
  //setting the condition variable?
  ACE_GUARD_RETURN (cond_lock_t,
                    mon, this->sched_queue_modified_cond_lock_, 0);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i after acquiring cond lock\n"));
#endif

  this->sched_queue_modified_ = 1;
  this->sched_queue_modified_cond_.signal ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i exit\n"));
#endif

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
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block entered\n"));
#endif

  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> dispatch_item;
  ACE_hthread_t thr_handle;

  int found = this->ready_queue_.find (guid, dispatch_item);
  if (found == 0 && flag == BLOCK)
    {
      thr_handle = dispatch_item->thread_handle ();
      if (ACE_OS::thr_setprio (thr_handle,
                               this->blocked_prio_,
                               this->sched_policy_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("thr_setprio failed")));
        }

      //monitor released because cancel_schedule would acquire the
      //lock. Using recursive mutex creates lock up.
      //
      //@@ Need to investigate this further. Also we can consider
      //using the Thread-Safe interface pattern.
      //mon.release ();
      int rc = this->cancel_schedule (guid);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block done\n"));
#endif

      return rc;
    }

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block done\n"));
#endif

  return -1;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
cancel_schedule_i (Guid_t guid)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t): about to remove guid\n"));
#endif

  this->ready_queue_.remove (guid);

#ifdef KOKYU_DSRT_LOGGING
  this->ready_queue_.dump ();
#endif

  if (this->curr_scheduled_guid_ == guid)
    {
      this->curr_scheduled_guid_ = 0;
      this->curr_scheduled_thr_handle_ = 0;
    }

  //release ready_queue_ lock first before getting another lock
  guard.release ();

  ACE_GUARD_RETURN (cond_lock_t,
                    mon, this->sched_queue_modified_cond_lock_, 0);
  this->sched_queue_modified_ = 1;
  this->sched_queue_modified_cond_.signal ();
  return 0;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
shutdown_i ()
{
  this->shutdown_flagged_ = 1;

  ACE_GUARD_RETURN (cond_lock_t, mon, this->sched_queue_modified_cond_lock_, 0);
  this->sched_queue_modified_ = 1;
  this->sched_queue_modified_cond_.signal ();
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
