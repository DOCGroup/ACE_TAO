// $Id$

#include "Dispatcher_Task.h"

#include "ace/Malloc_T.h"
#include "ace/OS_NS_errno.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatcher_Task.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_DSUI)
#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>
#endif // ACE_HAS_DSUI

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
          ACE_Message_Queue<ACE_SYNCH> (ACE_Message_Queue_Base::DEFAULT_HWM,
                                        ACE_Message_Queue_Base::DEFAULT_LWM,
                                        0,
                                        1),
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

#ifdef KOKYU_HAS_RELEASE_GUARD
  this->deferrer_attr_.task_ = this;
  this->deferrer_.init(this->deferrer_attr_);

  this->releases_.open();
#endif //KOKYU_HAS_RELEASE_GUARD

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

#if defined (ACE_HAS_DSUI)
      DSUI_EVENT_LOG (DISP_TASK_FAM, BEFORE_GETQ_CALL, 0, 0, NULL);
#endif // ACE_HAS_DSUI

      int result = this->getq (mb);

#if defined (ACE_HAS_DSUI)
      DSUI_EVENT_LOG (DISP_TASK_FAM, AFTER_GETQ_CALL, 0, 0, NULL);
#endif // ACE_HAS_DSUI

      if (result == -1)
        {
          if (ACE_OS::last_error () == ESHUTDOWN)
            {
              return 0;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "EC (%P|%t) getq error in Dispatching Queue\n"));
            }
        }

      ACE_Time_Value tv = ACE_OS::gettimeofday();
      ACE_DEBUG ((LM_DEBUG, "Dispatcher_Task::svc() (%t) : next command got from queue at %u\n",tv.msec()));

      Dispatch_Queue_Item *qitem =
        ACE_dynamic_cast(Dispatch_Queue_Item*, mb);

      if (qitem == 0)
        {
          ACE_Message_Block::release (mb);
          continue;
        }

      Dispatch_Command* command = qitem->command ();

#if defined (ACE_HAS_DSUI)
      //@BT INSTRUMENT with event ID: EVENT_DEQUEUED Measure time
      //between event released (enqueued) and dispatched
      Kokyu::Object_Counter::object_id oid = command->getID();
      DSUI_EVENT_LOG (DISP_TASK_FAM, EVENT_DEQUEUED, 0, sizeof(Kokyu::Object_Counter::object_id), (char*)&oid);
      ACE_ASSERT(command != 0);

      //@BT INSTRUMENT with event ID: EVENT_START_DISPATCHING Measure
      //time to actually dispatch event
      DSUI_EVENT_LOG (DISP_TASK_FAM, EVENT_START_DISPATCHING, 0, sizeof(Kokyu::Object_Counter::object_id), (char*)&oid);

      tv = ACE_OS::gettimeofday();
      ACE_DEBUG ((LM_DEBUG, "Dispatcher_Task::svc() (%t) : beginning event dispatch at %u\n",tv.msec()));
#endif //ACE_HAS_DSUI

      result = command->execute ();

#if defined (ACE_HAS_DSUI)
      //@BT INSTRUMENT with event ID: EVENT_FINISHED_DISPATCHING
      //Measure time to actually dispatch event
      DSUI_EVENT_LOG (DISP_TASK_FAM, EVENT_FINISHED_DISPATCHING, 0, sizeof(Kokyu::Object_Counter::object_id), (char*)&oid);

      tv = ACE_OS::gettimeofday();
      ACE_DEBUG ((LM_DEBUG, "Dispatcher_Task::svc() (%t) : end event dispatch at %u\n",tv.msec()));
#endif //ACE_HAS_DSUI

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
  //Subclasses which override this function should now be overriding
  //enqueue(ACE_Message_Block*) because that is where the main
  //behavior is defined. This might invalidate those classes, but the
  //decision seemed reasonable since it allows Dispatch_Deferrer to
  //use the same Dispatch_Queue_Item as Dispatcher_Task without any
  //more memory allocation and it doesn't break the interface.

  void* buf = this->allocator_->malloc (sizeof (Dispatch_Queue_Item));

  if (buf == 0)
    return -1;

  Dispatch_Queue_Item *qitem =
    new (buf) Dispatch_Queue_Item (cmd,
                                   qos_info,
                                   &(this->data_block_),
                                   ACE_Message_Block::DONT_DELETE,
                                   this->allocator_);

#ifdef KOKYU_HAS_RELEASE_GUARD
  //if current release time < last release time + period then defer dispatch
  ACE_Time_Value now(ACE_OS::gettimeofday());
  long rel_msec;
  if (this->releases_.find(qos_info,rel_msec) < 0)
    {
      //new QosDescriptor, so just set last release to zero
      rel_msec = 0;
    }
  ACE_Time_Value release;
  release.msec(rel_msec);
  release += qos_info.deadline_;

  if (now < release)
    {
      //defer until last release time + period
      this->deferrer_.dispatch(qitem);

#if defined (ACE_HAS_DSUI)
      //@BT INSTRUMENT with event ID: EVENT_DEFERRED Measure delay
      //between original dispatch and dispatch because of RG
      Kokyu::Object_Counter::object_id oid = cmd->getID();
      DSUI_EVENT_LOG (DISP_TASK_FAM, EVENT_DEFERRED, 0, sizeof(Kokyu::Object_Counter::object_id), (char*)&oid);

      ACE_Time_Value tv = ACE_OS::gettimeofday();
      ACE_DEBUG ((LM_DEBUG, "Dispatcher_Task::enqueue() (%t) : event deferred at %u\n",tv.msec()));
#endif //ACE_HAS_DSUI
    }
  else
    {
      //release!
#endif //KOKYU_HAS_RELEASE_GUARD

      this->enqueue_i (qitem);

#ifdef KOKYU_HAS_RELEASE_GUARD
    } //else now >= release
#endif //KOKYU_HAS_RELEASE_GUARD

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
