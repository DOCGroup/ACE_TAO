// $Id$

#include "Dispatcher_Task.h"

#include "ace/Malloc_T.h"
#include "ace/Synch_T.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatcher_Task.i"
#endif /* __ACE_INLINE__ */

#include "kokyu_dsui_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>

ACE_RCSID(Kokyu, Dispatcher_Task, "$Id$")

namespace 
//anonymous namespace - use this to avoid polluting the global namespace
{
  const int ALLOC_POOL_CHUNKS = 200;
}

namespace Kokyu
{

typedef ACE_Cached_Allocator<Dispatch_Queue_Item, ACE_SYNCH_MUTEX> 
Dispatch_Queue_Item_Allocator;

int
Dispatcher_Task::initialize ()
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

  if (this->allocator_ == 0)
    {
      ACE_NEW_RETURN (this->allocator_,
                      Dispatch_Queue_Item_Allocator(ALLOC_POOL_CHUNKS),
                      -1);
      own_allocator_ = 1;
    }

  return 0;
}

int
Dispatcher_Task::svc (void)
{
  int done = 0;

    ACE_hthread_t thr_handle;
    ACE_Thread::self (thr_handle);
    int prio;

    if (ACE_Thread::getprio (thr_handle, prio) == -1)
      {
        if (errno == ENOTSUP)
          {
            ACE_DEBUG((LM_DEBUG,
                       ACE_TEXT ("getprio not supported on this platform\n")
                       ));
            return 0;
          }
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("getprio failed")),
                          -1);
      }

    //ACE_DEBUG ((LM_DEBUG, "(%t) Dispatcher Thread started prio=%d\n", prio));

  while (!done)
    {
      ACE_Message_Block *mb;
      if (this->getq (mb) == -1)
        if (ACE_OS::last_error () == ESHUTDOWN)
          return 0;
        else
          ACE_ERROR ((LM_ERROR,
                      "EC (%P|%t) getq error in Dispatching Queue\n"));

      //ACE_DEBUG ((LM_DEBUG, "(%t) : next command got from queue\n"));

      Dispatch_Queue_Item *qitem =
        ACE_dynamic_cast(Dispatch_Queue_Item*, mb);

      if (qitem == 0)
        {
          ACE_Message_Block::release (mb);
          continue;
        }

      Dispatch_Command* command = qitem->command ();

      ACE_ASSERT(command != 0);

      //@@DSUI - DISPATCH_EVENT_EXECUTE event
      /* Log KOKYU_GROUP_FAM event: DISPATCH_EVENT_EXECUTE
       * ID: 0
       * Length of string data: 0
       * String data: NULL
       */
      DSUI_EVENT_LOG (KOKYU_GROUP_FAM, DISPATCH_EVENT_EXECUTE, 0, 0, NULL);
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
  //@@DSUI - DISPATCH_EVENT_ENQUEUE event
  /* Log KOKYU_GROUP_FAM event: DISPATCH_EVENT_ENQUEUE
   * ID: 0
   * Length of string data: 0
   * String data: NULL
   *
   * ID is meant to be a unique identifier for the stream of events. 
   * So if you are tracking a command (number 504) through the system
   * you would replace 0 with variable containing 504
   *
   * The string data is there in case you want to add someting to the
   * datastream data to describe or add information to the timestamp
   */
  DSUI_EVENT_LOG (KOKYU_GROUP_FAM, DISPATCH_EVENT_ENQUEUE, 0, 0, NULL);
  void* buf = this->allocator_->malloc (sizeof (Dispatch_Queue_Item));

  if (buf == 0)
    return -1;

  ACE_Message_Block *mb =
    new (buf) Dispatch_Queue_Item (cmd,
                                   qos_info,
                                   &(this->data_block_),
                                   ACE_Message_Block::DONT_DELETE,
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

template class ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >;
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
template class ACE_Cached_Allocator<Kokyu::Dispatch_Queue_Item, ACE_SYNCH_MUTEX>;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item> >;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item>, ACE_SYNCH_MUTEX>;
template class ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item> >
#pragma instantiate ACE_Cached_Allocator<Kokyu::Dispatch_Queue_Item, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Cached_Mem_Pool_Node<Kokyu::Dispatch_Queue_Item>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
