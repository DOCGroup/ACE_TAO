// $Id$

#ifndef DSRT_DIRECT_DISPATCHER_IMPL_T_CPP
#define DSRT_DIRECT_DISPATCHER_IMPL_T_CPP

#include "DSRT_Direct_Dispatcher_Impl_T.h"

#if !defined (__ACE_INLINE__)
//#include "DSRT_Direct_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>

ACE_RCSID(Kokyu, DSRT_Direct_Dispatcher_Impl_T, "$Id$")

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
   sched_queue_modified_cond_ (sched_queue_modified_cond_lock_),
   release_guard_cond_ (release_guard_cond_lock_)
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
              ACE_TEXT ("executive prio=%d\n")
              ACE_TEXT ("blocked prio=%d\n")
              ACE_TEXT ("active prio=%d\n")
              ACE_TEXT ("inactive prio=%d\n"),
              max_prio_,
              min_prio_,
	      executive_prio_,
	      blocked_prio_,
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
                        mon, sched_queue_modified_cond_lock_, 0);

      if (this->shutdown_flagged_)
        break;

      while (!sched_queue_modified_)
        {
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG ((LM_DEBUG,
                      "(%t): sched thread about to wait on cv\n"));
#endif
          sched_queue_modified_cond_.wait ();
        }

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t): sched thread done waiting on cv\n"));
#endif

      sched_queue_modified_ = 0;

      ACE_Guard<ACE_SYNCH_RECURSIVE_MUTEX> synch_lock_mon(this->synch_lock_);
#ifdef KOKYU_DSRT_LOGGING
   ACE_DEBUG((LM_DEBUG,"(%t): sched thread get the ready queue lock\n"));
#endif

      if (this->ready_queue_.current_size () <= 0) {
        ACE_GUARD_RETURN (cond_lock_t,
             rg_mon, this->release_guard_cond_lock_, 0);
        this->release_guard_cond_.signal();
        continue;
      }

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
                  "(%t|%T):most eligible thr handle = %d \n",
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
  DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, SCHEDULE_ENTER, 0,0,NULL);
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
              "(%t|%T):schedule_i after thr_setprio \n"));
#endif

  //ready_queue_.dump ();

/*first release ready_queue_ lock. Otherwise if the scheduler gets the
  sched_queue_modified_cond_lock first, then try to get the ready_queue_ lock
  just when one thread who gets the ready_queue_ lock first, then try to get
  sched_queue_modified_cond_lock. Deadlock happens.
*/
   guard.release ();
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG((LM_DEBUG,"(%t):schedule_i after release ready queue lock\n"));
#endif

  //@@ Perhaps the lock could be moved further down just before
  //setting the condition variable?
  ACE_GUARD_RETURN (cond_lock_t,
                    mon, this->sched_queue_modified_cond_lock_, 0);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i after acquiring sched_queue_modified lock\n"));
#endif

  this->sched_queue_modified_ = 1;
  this->sched_queue_modified_cond_.signal ();

/*
  mon.release ();

int prio;
  if (ACE_OS::thr_setprio (thr_handle,
                           this->inactive_prio_,
                           this->sched_policy_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("thr_setprio failed")), -1);
    }
ACE_OS::thr_getprio (thr_handle, prio);

*/

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i exit\n"));
#endif
  DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, SCHEDULE_EXIT, 0,0,NULL);
  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
release_guard_i (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  int int_guid;
  ACE_OS::memcpy (&int_guid,
                  guid.get_buffer (),
                  guid.length ());

  DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, RELEASE_GUARD_START, int_guid, 0, NULL);
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG((LM_DEBUG,"(%t|%T):release guard enter and current task id is %d and period is %d.\n",qos.task_id_,qos.period_));
#endif

      ACE_Time_Value cur_time, tv, proper_t,left_time;

//Need a period information. Hope I can get it from QoS.
      TimeBase::TimeT period = qos.period_;
      cur_time = ACE_OS::gettimeofday ();

//If I change the guid to Task ID, will it work?
      if(this->release_map_.find(qos.task_id_, tv)==0)
      {
#ifdef MY_KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Get the previous release time is %d, %d\n",tv.sec(),tv.usec()));
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Get the cur time is %d, %d\n",cur_time.sec(), cur_time.usec()));
#endif
        proper_t = tv + ACE_Time_Value (period/10000000, (period%10000000)/10);
        if( proper_t  <= cur_time )
           {
#ifdef MY_KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Over the proper release time\n"));
#endif
             this->release_map_.rebind(qos.task_id_, cur_time);
/*DTTIME:
  Release time on the server side. please record the guid in your DSUI_EVENT_LOG
*/
             DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, RG_EVENT_RELEASED, qos.task_id_, 0, NULL);
             this->schedule_i (guid, qos);
           }
        else
           {
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

           if(this->ready_queue_.current_size()) {
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG((LM_DEBUG,"(%t|%T):BEFORE GOING TO SLEEP\n"));
#endif
           left_time = proper_t - cur_time;
           timeval time_tv;
           time_tv.tv_sec = left_time.sec();
           time_tv.tv_usec = left_time.usec();

           left_time += ACE_OS::gettimeofday ();
#ifdef MY_KOKYU_DSRT_LOGGING
           ACE_DEBUG((LM_DEBUG,"GOING TO SLEEP FOR %d, %d\n", time_tv.tv_sec, time_tv.tv_usec));
#endif
           guard.release ();
	   ACE_GUARD_RETURN (cond_lock_t,
                        rg_mon, this->release_guard_cond_lock_, 0);
           if (this->release_guard_cond_.wait (&left_time) == -1)
             {
             ACE_ERROR ((LM_ERROR,
                      "(%t|%T): release_cond.wait timed out \n"));
             }
           rg_mon.release();
           }
           else 
		guard.release ();

             cur_time = ACE_OS::gettimeofday ();
#ifdef MY_KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): And Current time is set in the map and %d, %d.\n",cur_time.sec(), cur_time.usec() ));
#endif

             this->release_map_.rebind(qos.task_id_, cur_time);
/*DTTIME:
  Release time on the server side. please record the guid in your DSUI_EVENT_LOG
*/
             DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, RG_EVENT_DELAYED_RELEASED, qos.task_id_, 0, NULL);

             this->schedule_i (guid, qos);
           }
      }
      else
      {
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Can not find release information in map\n"));
#endif

             this->release_map_.bind(qos.task_id_, cur_time);
/*DTTIME:
  Release time on the server side. please record the guid in your DSUI_EVENT_LOG
*/
             DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, NONRG_EVENT_RELEASED, qos.task_id_, 0, NULL);

             this->schedule_i (guid, qos);
      }

  DSUI_EVENT_LOG (DSRT_DIRECT_DISPATCH_FAM, RELEASE_GUARD_END, int_guid, 0, NULL);
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
  //@@ Perhaps the lock could be got rid of. It looks like the state
  //of this object is not getting modified here. It makes calls to
  //other methods, which already are thread-safe.
  //ACE_Guard<cond_lock_t> mon(sched_queue_modified_cond_lock_);

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
      guard.release ();
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

  guard.release ();
  
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG((LM_DEBUG,"(%t): AFTER release ready queue lock in cancel_schedule_i\n"));
#endif

  ACE_GUARD_RETURN (cond_lock_t,
                    mon, this->sched_queue_modified_cond_lock_, 0);
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG((LM_DEBUG,"(%t): AFTER get the sched_queue_modified_lock in cancel_schedule_i\n"));
#endif

  this->sched_queue_modified_ = 1;
  this->sched_queue_modified_cond_.signal ();
  return 0;
}

template <class DSRT_Scheduler_Traits> int
DSRT_Direct_Dispatcher_Impl<DSRT_Scheduler_Traits>::
shutdown_i ()
{
  this->shutdown_flagged_ = 1;

  ACE_Guard<cond_lock_t> mon(this->sched_queue_modified_cond_lock_);
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
