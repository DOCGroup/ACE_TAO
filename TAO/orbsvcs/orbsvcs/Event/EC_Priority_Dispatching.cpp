// $Id$

#include "EC_Priority_Dispatching.h"
#include "EC_Dispatching_Task.h"
#include "EC_Event_Channel.h"
#include "EC_QOS_Info.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Priority_Dispatching.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Priority_Dispatching, "$Id$")

ACE_INLINE
TAO_EC_Priority_Dispatching::TAO_EC_Priority_Dispatching (TAO_EC_Event_Channel *ec)
  :  ntasks_ (0),
     tasks_ (0),
     scheduler_ (ec->scheduler ())
{
}

void
TAO_EC_Priority_Dispatching::activate (void)
{
  if (this->tasks_ != 0)
    return;

  // @@ Query the scheduler to obtain this....
  this->ntasks_ = ACE_Scheduler_MAX_PRIORITIES;
  ACE_NEW (this->tasks_, TAO_EC_Dispatching_Task*[this->ntasks_]);

  // @@ Query the scheduler to obtain the priorities!
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

  for (int i = 0; i < this->ntasks_; ++i)
    {
      ACE_NEW (this->tasks_[i],
               TAO_EC_Dispatching_Task (&this->thread_manager_));

      // @@ Query the scheduler to obtain the priority...
      long flags = THR_BOUND | THR_SCHED_FIFO;
      if (this->tasks_[i]->activate (flags, 1, 1, priority) == -1)
        {
          flags = THR_BOUND;
          priority = ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                     ACE_SCOPE_THREAD);
          if (this->tasks_[i]->activate (flags, 1, 1, priority) == -1)
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate queue %d", i));
        }
    }
}

void
TAO_EC_Priority_Dispatching::shutdown (void)
{
  if (this->tasks_ == 0)
    return;

  for (int i = 0; i < this->ntasks_; ++i)
    this->tasks_[i]->putq (new TAO_EC_Shutdown_Command);

  this->thread_manager_.wait ();

  for (int j = 0; j < this->ntasks_; ++j)
    delete this->tasks_[j];

  delete[] this->tasks_;
  this->tasks_ = 0;
}

void
TAO_EC_Priority_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                                   const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventComm::EventSet event_copy = event;
  this->push_nocopy (proxy, event_copy, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Priority_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                          RtecEventComm::EventSet& event,
                                          TAO_EC_QOS_Info& qos_info,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  if (this->tasks_ == 0)
    this->activate ();

  int i = qos_info.preemption_priority;
  if (i < 0 || i >= this->ntasks_)
    {
      // @@ Throw something?
      i = 0;
    }


  this->tasks_[i]->push (proxy, event, ACE_TRY_ENV);
}
