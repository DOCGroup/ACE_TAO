// $Id$

#include "Default_Dispatcher_Impl.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "Default_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

namespace Kokyu
{
int
Default_Dispatcher_Impl::init_i (const ConfigInfoSet& config_info_set)
{
  //create and init the dispatcher tasks here

  ACE_DEBUG ((LM_DEBUG, "entering init_t\n" ));
  int size;
  size = config_info_set.size ();

  if (size == 0)
    return -1;

  this->ntasks_ = size;

  Dispatcher_Task_Auto_Ptr * tmp=0;
  ACE_NEW_RETURN (tmp, Dispatcher_Task_Auto_Ptr[ntasks_], -1);

  ACE_DEBUG ((LM_DEBUG, "after new on task array\n" ));
  tasks_.reset(tmp);

  ACE_DEBUG ((LM_DEBUG, "task array auto_ptr set\n" ));

  ConfigInfoSet& config_set = const_cast<ConfigInfoSet&> (config_info_set);
  ConfigInfoSet::ITERATOR iter(config_set);
  int i=0;

  ConfigInfo* config;
  for (;i<size && iter.next (config);iter.advance ())
    {
      ACE_DEBUG ((LM_DEBUG, "iter = %d\n", i));
      Dispatcher_Task* tmp2=0;
      ACE_NEW_RETURN (tmp2, Dispatcher_Task (*config), -1);
      tasks_[i++].reset (tmp2);
    }

  this->activate ();

  curr_config_info_ = config_info_set;
  return 0;
}

int
Default_Dispatcher_Impl::activate ()
{
  int i;
  for(i=0; i<ntasks_; ++i)
    {
      long flags = THR_BOUND | THR_SCHED_FIFO;

      Priority_t priority =
        tasks_[i]->get_curr_config_info ().thread_priority_;

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

  return 0;
}

Dispatcher_Task*
Default_Dispatcher_Impl::find_task_with_preemption_prio (Priority_t prio)
{
  int i;

  if (prio >=0)
  {
    for( i=0; i<ntasks_; ++i)
    {
      if ( tasks_[i]->preemption_priority () == prio)
        return  tasks_[i].get();
    }
  }

  return 0;
}

int
Default_Dispatcher_Impl::dispatch_i (const Dispatch_Command* cmd,
                  const QoSDescriptor& qos_info)
{
  //delegate to the appropriate task
  if (qos_info.preemption_priority_ < 0)
    return -1;

  Dispatcher_Task* task =
    find_task_with_preemption_prio (qos_info.preemption_priority_);

  if (task != 0)
    task->enqueue (cmd, qos_info);
  else
    tasks_[0]->enqueue (cmd, qos_info);

  return 0;
}

int
Default_Dispatcher_Impl::shutdown_i ()
{
  //post shutdown command to all tasks
  int i;

  for(i=0; i<ntasks_; ++i)
    {
      QoSDescriptor qos_info;
      Shutdown_Task_Command* shutdown_cmd = 0;
      ACE_NEW_RETURN (shutdown_cmd, Shutdown_Task_Command, -1);
      tasks_[i]->enqueue (shutdown_cmd, qos_info);
    }

  return 0;
}

}
