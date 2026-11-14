#include "Default_Dispatcher_Impl.h"

#include "ace/Sched_Params.h"
#if ! defined (__ACE_INLINE__)
#include "Default_Dispatcher_Impl.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{
Default_Dispatcher_Impl::Default_Dispatcher_Impl () = default;

int
Default_Dispatcher_Impl::init_i (Dispatcher_Attributes const &attrs)
{
  int const size = ACE_Utils::truncate_cast<int> (attrs.config_info_set_.size ());

  if (size == 0)
    {
      return -1;
    }

  this->ntasks_ = size;

  Dispatcher_Task_Auto_Ptr *tasks_array {};
  ACE_NEW_RETURN (tasks_array, Dispatcher_Task_Auto_Ptr[ntasks_], -1);

  this->tasks_.reset (tasks_array);

  ConfigInfoSet &config_set =
    const_cast<ConfigInfoSet&> (attrs.config_info_set_);
  ConfigInfoSet::ITERATOR iter (config_set);
  ConfigInfo *config {};
  int i {};
  for (; i < size && iter.next (config); iter.advance (), ++i)
    {
      Dispatcher_Task *task {};
      ACE_NEW_RETURN (task,
                      Dispatcher_Task (*config,
                                       ACE_Thread_Manager::instance()),
                      -1);
      this->tasks_[i].reset (task);
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
  if (this->activated_)
    {
      return 0;
    }

  for (int i {}; i < this->ntasks_; ++i)
    {
      Priority_t const priority =
        this->tasks_[i]->get_curr_config_info ().thread_priority_;

      if (this->tasks_[i]->activate (this->thr_creation_flags_,
                                     1, 1, priority) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("EC (%P|%t) cannot activate queue.")
             ACE_TEXT ("Need superuser privilege to run in RT class\n")),
             -1);
        }
    }

  this->activated_ = true;
  return 0;
}

Dispatcher_Task*
Default_Dispatcher_Impl::find_task_with_preemption_prio (Priority_t prio)
{
  if (prio >= 0)
    {
      for (int i {}; i < this->ntasks_; ++i)
        {
          if (this->tasks_[i]->preemption_priority () == prio)
            {
              return this->tasks_[i].get ();
            }
        }
    }

  return nullptr;
}

int
Default_Dispatcher_Impl::dispatch_i (Dispatch_Command const *cmd,
                                     QoSDescriptor const &qos_info)
{
  //delegate to the appropriate task
  if (qos_info.preemption_priority_ < 0)
    return -1;

  Dispatcher_Task *task =
    this->find_task_with_preemption_prio (qos_info.preemption_priority_);

  //@@VS - We should insert this into the lowest prio queue.
  //How do we know that the last queue is the lowest prio queue.
  if (task == nullptr)
    task = this->tasks_[this->ntasks_ - 1].get ();

  return task->enqueue (cmd, qos_info);
}

int
Default_Dispatcher_Impl::shutdown_i ()
{
  //This needs to be revisited based on mode transition and
  //consistent cut through the queues

  //post shutdown command to all tasks
  for (int i {}; i < this->ntasks_; ++i)
    {
      QoSDescriptor const qos_info {};
      Shutdown_Task_Command *raw_shutdown_cmd {};
      ACE_NEW_RETURN (raw_shutdown_cmd, Shutdown_Task_Command, -1);
      std::unique_ptr<Shutdown_Task_Command> shutdown_cmd (raw_shutdown_cmd);
      if (this->tasks_[i]->enqueue (shutdown_cmd.get (), qos_info) == 0)
        shutdown_cmd.release ();
    }

  //wait for all tasks to exit
  for (int i {}; i < this->ntasks_; ++i)
    {
      this->tasks_[i]->wait ();
    }

  return 0;
}

int
Shutdown_Task_Command::execute ()
{
  return -1;
}

}
