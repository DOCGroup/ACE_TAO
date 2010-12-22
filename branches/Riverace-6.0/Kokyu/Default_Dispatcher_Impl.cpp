// $Id$

#include "Default_Dispatcher_Impl.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "Default_Dispatcher_Impl.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{

Default_Dispatcher_Impl::Default_Dispatcher_Impl ()
  : activated_ (0)
{
}

int
Default_Dispatcher_Impl::init_i (const Dispatcher_Attributes& attrs)
{
  //create and init the dispatcher tasks here

  ACE_DEBUG ((LM_DEBUG, "entering init_t\n" ));
  int size = ACE_Utils::truncate_cast<int> (attrs.config_info_set_.size ());

  if (size == 0)
    {
      return -1;
    }

  this->ntasks_ = size;

  Dispatcher_Task_Auto_Ptr * tasks_array=0;
  ACE_NEW_RETURN (tasks_array, Dispatcher_Task_Auto_Ptr[ntasks_], -1);

  //ACE_DEBUG ((LM_DEBUG, "after new on task array\n" ));
  tasks_.reset(tasks_array);

  //ACE_DEBUG ((LM_DEBUG, "task array auto_ptr set\n" ));

  ConfigInfoSet& config_set =
    const_cast<ConfigInfoSet&> (attrs.config_info_set_);
  ConfigInfoSet::ITERATOR iter(config_set);
  int i=0;

  ConfigInfo* config;
  for (;i<size && iter.next (config);iter.advance ())
    {
      //ACE_DEBUG ((LM_DEBUG, "iter = %d\n", i));
      Dispatcher_Task* task=0;
      ACE_NEW_RETURN (task,
                      Dispatcher_Task (*config,
                                       ACE_Thread_Manager::instance()),
                      -1);
      auto_ptr<Dispatcher_Task> tmp_task_auto_ptr (task);
      tasks_[i++] = tmp_task_auto_ptr;
      //I couldn't use reset because MSVC6 auto_ptr does not have reset method.
      //So in configurations where the auto_ptr maps to the std::auto_ptr instead
      //of ACE auto_ptr, this would be a problem.
      //tasks_[i++].reset (task);
    }

  this->thr_creation_flags_ = attrs.thread_creation_flags ();

  if (attrs.immediate_activation_ && !this->activated_)
    {
      this->activate_i ();
    }

  curr_config_info_ = attrs.config_info_set_;
  return 0;
}

int
Default_Dispatcher_Impl::activate_i ()
{
  int i;

  if (this->activated_)
    return 0;

  for(i=0; i<ntasks_; ++i)
    {
      Priority_t priority =
        tasks_[i]->get_curr_config_info ().thread_priority_;

      if (this->tasks_[i]->activate (this->thr_creation_flags_,
                                     1, 1, priority) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("EC (%P|%t) cannot activate queue.")
             ACE_TEXT ("Need superuser privilege to run in RT class\n")),
             -1);
        }
    }

  this->activated_ = 1;
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

  //@@VS - We should insert this into the lowest prio queue.
  //How do we know that the last queue is the lowest prio queue.
  if (task == 0)
    task = tasks_[ntasks_-1].get ();

  return task->enqueue (cmd, qos_info);
}

int
Default_Dispatcher_Impl::shutdown_i ()
{
  //This needs to be revisited based on mode transition and
  //consistent cut through the queues

  //post shutdown command to all tasks
  int i;

  for(i=0; i<ntasks_; ++i)
    {
      QoSDescriptor qos_info;
      Shutdown_Task_Command* shutdown_cmd = 0;
      ACE_NEW_RETURN (shutdown_cmd, Shutdown_Task_Command, -1);
      tasks_[i]->enqueue (shutdown_cmd, qos_info);
    }

  //wait for all tasks to exit
  for (i=0; i<ntasks_; ++i)
    {
      tasks_[i]->wait ();
    }

  return 0;
}

int
Shutdown_Task_Command::execute ()
{
  return -1;
}

}
