// $Id$

#include "Dispatcher_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatcher_Task.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Task, "$Id$")

namespace Kokyu
{

int
Dispatcher_Task::init ()
{
  switch(curr_config_info_.dispatching_type_)
    {
    case FIFO_DISPATCHING:
      ACE_NEW_RETURN (
          this->the_queue_,
          ACE_Message_Queue<ACE_SYNCH>,
          -1);
      break;

    case DEADLINE_DISPATCHING:
      ACE_NEW_RETURN (
          this->the_queue_,
          ACE_Dynamic_Message_Queue<ACE_SYNCH> (deadline_msg_strategy_),
          -1);
      break;

    case LAXITY_DISPATCHING:
      ACE_NEW_RETURN (
           this->the_queue_,
           ACE_Dynamic_Message_Queue<ACE_SYNCH> (laxity_msg_strategy_),
           -1);
      break;

    default:
      return -1;
      break;
    }

  if (this->the_queue_ != 0)
    {
      this->msg_queue(this->the_queue_);
    }
  return 0;
}

int
Dispatcher_Task::svc (void)
{
  int done = 0;
  while (!done)
    {
      ACE_Message_Block *mb;
      if (this->getq (mb) == -1)
        if (ACE_OS::last_error () == ESHUTDOWN)
          return 0;
        else
          ACE_ERROR ((LM_ERROR,
                      "EC (%P|%t) getq error in Dispatching Queue\n"));

      Dispatch_Queue_Item *qitem =
        ACE_dynamic_cast(Dispatch_Queue_Item*, mb);

      if (qitem == 0)
        {
          ACE_Message_Block::release (mb);
          continue;
        }

      Dispatch_Command* command = qitem->command ();

      ACE_ASSERT(command != 0);
      int result = command->execute ();

      if (command->can_be_deleted ())
        command->destroy ();

      ACE_Message_Block::release (mb);

      if (result == -1)
        done = 1;
    }
  return 0;
}

int
Dispatcher_Task::enqueue (const Dispatch_Command* cmd,
                          const QoSDescriptor& qos_info)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (Dispatch_Queue_Item));

  if (buf == 0)
    return -1;

  ACE_Message_Block *mb =
    new (buf) Dispatch_Queue_Item (cmd,
                                   qos_info,
                                   this->data_block_.duplicate (),
                                   this->allocator_);

  this->putq (mb);

  return 0;
}

int Dispatcher_Task::get_native_prio ()
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
  {
    if (errno == ENOTSUP)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("getprior not supported on this platform\n")
               ));
      return 0;
    }
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("getprio failed")),
                      -1);
  }

  return prio;
}

void Dispatch_Queue_Item::init_i (const QoSDescriptor& qos_info)
{
  this->msg_priority (qos_info.preemption_priority_);
  this->msg_execution_time (qos_info.execution_time_);
  this->msg_deadline_time (qos_info.deadline_);
}

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
