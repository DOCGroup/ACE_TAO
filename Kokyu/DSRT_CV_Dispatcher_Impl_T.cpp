// $Id$

#ifndef DSRT_CV_DISPATCHER_IMPL_T_CPP
#define DSRT_CV_DISPATCHER_IMPL_T_CPP

#include "DSRT_CV_Dispatcher_Impl_T.h"

#if !defined (__ACE_INLINE__)
//#include "DSRT_CV_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>

ACE_RCSID(Kokyu, DSRT_CV_Dispatcher_Impl_T, "$Id$")

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
DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
DSRT_CV_Dispatcher_Impl (ACE_Sched_Params::Policy sched_policy,
                         int sched_scope)
  :DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits> (sched_policy, sched_scope),
   run_cond_ (run_cond_lock_), release_cond_ (release_cond_lock_)
{
}

template <class DSRT_Scheduler_Traits> int
DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
init_i (const DSRT_ConfigInfo&)
{
  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
schedule_i (Guid_t id, const DSRT_QoSDescriptor& qos)
{

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i enter\n"));
#endif
  DSUI_EVENT_LOG (DSRT_CV_DISPATCH_FAM, SCHEDULE_ENTER, 0,0,NULL);
  DSRT_Dispatch_Item<DSRT_Scheduler_Traits>* item;
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  if (ACE_OS::thr_setprio (thr_handle,
                           this->blocked_prio_,
                           this->sched_policy_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("thr_setprio failed")), -1);
    }

  ACE_NEW_RETURN (item,
                  DSRT_Dispatch_Item<DSRT_Scheduler_Traits> (id, qos),
                  -1);
  item->thread_handle (thr_handle);

  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);
  if (this->ready_queue_.insert (item) == -1)
    return -1;

#ifdef KOKYU_DSRT_LOGGING
  this->ready_queue_.dump ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i after ready_q.insert\n"));
#endif

  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> item_var;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, cond_guard, run_cond_lock_, -1);
  this->ready_queue_.most_eligible (item_var);

  guard.release ();

  ACE_hthread_t most_eligible_thr_handle = item_var->thread_handle ();

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T):curr thr handle = %d\n", 
                  thr_handle)); 
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T):curr scheduled thr handle = %d\n", 
                  this->curr_scheduled_thr_handle_)); 
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T):most eligible thr handle = %d \n",
                  most_eligible_thr_handle)); 
#endif

  if (this->curr_scheduled_thr_handle_ == thr_handle &&
      most_eligible_thr_handle != thr_handle)
    {
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T):curr sched thr handle = thr_handle & "
                  "most eligible thr handle != curr thr handle. "
                  "about to do a broadcast on CV to wake up most eligible\n")); 
#endif
      this->curr_scheduled_thr_handle_ = most_eligible_thr_handle;
      //wake up the most eligible thread
      this->run_cond_.broadcast ();
    }

  //if the current thread is not the most eligible, then wait.
  //if the current thread is most eligible, but some thread is
  //scheduled currently, then wait.
  while (most_eligible_thr_handle != thr_handle ||
         (most_eligible_thr_handle == thr_handle && 
          this->curr_scheduled_thr_handle_ != thr_handle &&
          this->curr_scheduled_thr_handle_ != 0))
    {
      ACE_Time_Value tv (60,0);
      tv += ACE_OS::gettimeofday ();
      //wait a maximum of 1 min. This is an escape latch against lockups. 
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T): About to block on cv\n"));
#endif
      if (this->run_cond_.wait (&tv) == -1) 
        {
          ACE_ERROR ((LM_ERROR, 
                      "(%t|%T): run_cond.wait timed out -- Possible Lockup\n"));
        }
      this->ready_queue_.most_eligible (item_var);
      most_eligible_thr_handle = item_var->thread_handle ();
    }
  this->curr_scheduled_guid_ = item_var->guid ();
  this->curr_scheduled_thr_handle_ = most_eligible_thr_handle;

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T): %d is currently running\n",
                  thr_handle));
#endif

  if (ACE_OS::thr_setprio (thr_handle,
                           this->active_prio_,
                           this->sched_policy_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("thr_setprio failed")));
    }

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):schedule_i exit\n"));
#endif
  DSUI_EVENT_LOG (DSRT_CV_DISPATCH_FAM, SCHEDULE_EXIT, 0,0,NULL);
  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
release_guard_i (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG((LM_DEBUG,"(%t|%T):release guard enter and current task id is %d and period is %d.\n",qos.task_id_,qos.period_));
#endif

      ACE_Time_Value cur_time;
      TimeBase::TimeT tv, cur_v;

//Need a period information. Hope I can get it from QoS.
      TimeBase::TimeT period = qos.period_;
      cur_time = ACE_OS::gettimeofday ();
      cur_v = cur_time.sec()*10000000+cur_time.usec()*10 ;

//If I change the guid to Task ID, will it work?
      if(this->release_map_.find(qos.task_id_, tv)==0)
      {
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Get the previous release time is %d and current time is %d\n",tv, cur_v));
#endif
        if( tv + period >= cur_v )
           {
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Over the proper release time\n"));
#endif
             this->release_map_.rebind(qos.task_id_, cur_v);
             this->schedule_i (guid, qos);
           }
        else
           {
           ACE_Time_Value left_time;
//           left_time.sec = (cur_v-tv-period)/10000000;
//           left_time.usec = (cur_v-tv-period)/10;
           left_time.set (ACE_static_cast(ACE_UINT32, (cur_v-tv-period) / 10000000),
           ACE_static_cast(ACE_UINT32, ((cur_v-tv-period) % 10000000) / 1000));

#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): About to block on release guard cond\n"));
#endif
           if (this->release_cond_.wait (&left_time) == -1)
             {
             ACE_ERROR ((LM_ERROR,
                      "(%t|%T): release_cond.wait timed out \n"));
             cur_time = ACE_OS::gettimeofday ();
             cur_v = cur_time.sec()*10000000+cur_time.usec()*10 ;
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): And Current time %d is set in the map.\n",cur_v));
#endif

             this->release_map_.rebind(qos.task_id_, cur_v);
             this->schedule_i (guid, qos);
             }
           }
      }
      else
      {
#ifdef KOKYU_DSRT_LOGGING
           ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Can not find release information in map\n"));
#endif

             this->release_map_.bind(qos.task_id_, cur_v);
             this->schedule_i (guid, qos);
      }

  return 0;
}

template <class DSRT_Scheduler_Traits>
int DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule_i (Guid_t guid, const DSRT_QoSDescriptor& qos)
{
  return this->schedule_i (guid, qos);
}

template <class DSRT_Scheduler_Traits>
int DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule_i (Guid_t guid, Block_Flag_t flag)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t): update schedule for block entered\n"));
#endif

  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> dispatch_item;
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  int found = this->ready_queue_.find (guid, dispatch_item);

#ifdef KOKYU_DSRT_LOGGING
  if (found == 0)
      ACE_DEBUG ((LM_DEBUG, "(%t|%T): %d found in ready queue\n", thr_handle));
  else
      ACE_DEBUG ((LM_DEBUG, "(%t|%T): %d not found in ready queue\n", 
                  thr_handle));    
#endif

  if (found == 0 && flag == BLOCK)
    {
      thr_handle = dispatch_item->thread_handle ();

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t|%T): update schedule: %d found\n", thr_handle));
#endif

      if (ACE_OS::thr_setprio (thr_handle, 
                               this->blocked_prio_, 
                               this->sched_policy_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("thr_setprio failed")));
        }

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
DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
cancel_schedule_i (Guid_t guid)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->synch_lock_, -1);

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): about to remove guid\n"));
#endif

  this->ready_queue_.remove (guid);

#ifdef KOKYU_DSRT_LOGGING
  this->ready_queue_.dump ();
#endif

  if (this->curr_scheduled_thr_handle_ == thr_handle)
    {
      this->curr_scheduled_guid_ = 0;
      this->curr_scheduled_thr_handle_ = 0;
    }

  ACE_GUARD_RETURN (cond_lock_t,
                    mon, this->run_cond_lock_, 0);
  this->run_cond_.broadcast ();
  return 0;
}

template <class DSRT_Scheduler_Traits> int
DSRT_CV_Dispatcher_Impl<DSRT_Scheduler_Traits>::
shutdown_i ()
{
  this->shutdown_flagged_ = 1;
  return 0;
}

}

#endif /* DSRT_CV_DISPATCHER_IMPL_T_CPP */
