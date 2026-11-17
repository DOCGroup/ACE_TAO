#include "Default_Dispatcher_Impl.h"

#include "ace/Sched_Params.h"
#include <new>
#include <utility>
#if ! defined (__ACE_INLINE__)
#include "Default_Dispatcher_Impl.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{
Default_Dispatcher_Impl::Default_Dispatcher_Impl () = default;

int
Default_Dispatcher_Impl::init_i (Dispatcher_Attributes const &attrs)
{
  size_t const size = attrs.config_info_set_.size ();

  if (size == 0)
    {
      return -1;
    }

  std::vector<Dispatcher_Task_Auto_Ptr> tasks;
  try
    {
      tasks.reserve (size);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }

  ConfigInfoSet::const_iterator iter = attrs.config_info_set_.begin ();
  for (size_t i {}; i < size; ++i, ++iter)
    {
      Dispatcher_Task *task {};
      ACE_NEW_RETURN (task,
                      Dispatcher_Task (*iter,
                                       ACE_Thread_Manager::instance()),
                      -1);
      std::unique_ptr<Dispatcher_Task> task_owner (task);
      if (task_owner->msg_queue () == nullptr)
        {
          return -1;
        }
      tasks.emplace_back (std::move (task_owner));
    }

  this->tasks_.swap (tasks);
  tasks.clear ();

  this->thr_creation_flags_ = attrs.thread_creation_flags ();

  if (attrs.immediate_activation_ && !this->activated_)
    {
      if (this->activate_i () == -1)
        {
          // The factory has not exposed this dispatcher yet, so no dispatch
          // commands can be queued while initialization is rolling back.
          for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
            {
              task->msg_queue ()->deactivate ();
            }

          for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
            {
              task->wait ();
            }

          return -1;
        }
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

  for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
    {
      Priority_t const priority =
        task->get_curr_config_info ().thread_priority_;

      if (task->activate (this->thr_creation_flags_,
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
      for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
        {
          if (task->preemption_priority () == prio)
            {
              return task.get ();
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
    {
      return -1;
    }

  Dispatcher_Task *task =
    this->find_task_with_preemption_prio (qos_info.preemption_priority_);

  //@@VS - We should insert this into the lowest prio queue.
  //How do we know that the last queue is the lowest prio queue.
  if (task == nullptr)
    {
      task = this->tasks_.back ().get ();
    }

  return task->enqueue (cmd, qos_info);
}

int
Default_Dispatcher_Impl::shutdown_i ()
{
  //This needs to be revisited based on mode transition and
  //consistent cut through the queues

  //post shutdown command to all tasks
  for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
    {
      QoSDescriptor const qos_info {};
      Shutdown_Task_Command *raw_shutdown_cmd {};
      ACE_NEW_RETURN (raw_shutdown_cmd, Shutdown_Task_Command, -1);
      std::unique_ptr<Shutdown_Task_Command> shutdown_cmd (raw_shutdown_cmd);
      if (task->enqueue (shutdown_cmd.get (), qos_info) == 0)
        {
          shutdown_cmd.release ();
        }
    }

  //wait for all tasks to exit
  for (Dispatcher_Task_Auto_Ptr const &task : this->tasks_)
    {
      task->wait ();
    }

  return 0;
}

int
Shutdown_Task_Command::execute ()
{
  return -1;
}

}
