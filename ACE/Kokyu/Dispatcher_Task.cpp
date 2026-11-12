#include "Dispatcher_Task.h"

#include "ace/Malloc_T.h"
#include "ace/OS_NS_errno.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatcher_Task.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{
using Dispatch_Queue_Item_Allocator =
  ACE_Cached_Allocator<Dispatch_Queue_Item, ACE_SYNCH_MUTEX>;

int
Dispatcher_Task::initialize ()
{
  switch(curr_config_info_.dispatching_type_)
    {
    case FIFO_DISPATCHING:
      {
      ACE_Message_Queue<ACE_SYNCH> *queue {};
      ACE_NEW_RETURN (
          queue,
          ACE_Message_Queue<ACE_SYNCH>,
          -1);
      this->owned_queue_.reset (queue);
      break;
      }

    case DEADLINE_DISPATCHING:
      {
      ACE_Deadline_Message_Strategy *strategy_raw {};
      ACE_NEW_RETURN (strategy_raw,
                      ACE_Deadline_Message_Strategy (
                        this->curr_config_info_.reordering_flags_.static_bit_field_mask_,
                        this->curr_config_info_.reordering_flags_.static_bit_field_shift_,
                        this->curr_config_info_.reordering_flags_.dynamic_priority_max_,
                        this->curr_config_info_.reordering_flags_.dynamic_priority_offset_),
                      -1);
      std::unique_ptr<ACE_Deadline_Message_Strategy> strategy (strategy_raw);
      ACE_Dynamic_Message_Queue<ACE_SYNCH> *deadline_queue {};
      ACE_NEW_RETURN (
          deadline_queue,
          ACE_Dynamic_Message_Queue<ACE_SYNCH> (*strategy),
          -1);
      strategy.release ();
      this->owned_queue_.reset (deadline_queue);
      break;
      }

    case LAXITY_DISPATCHING:
      {
      ACE_Laxity_Message_Strategy *strategy_raw {};
      ACE_NEW_RETURN (strategy_raw,
                      ACE_Laxity_Message_Strategy (
                        this->curr_config_info_.reordering_flags_.static_bit_field_mask_,
                        this->curr_config_info_.reordering_flags_.static_bit_field_shift_,
                        this->curr_config_info_.reordering_flags_.dynamic_priority_max_,
                        this->curr_config_info_.reordering_flags_.dynamic_priority_offset_),
                      -1);
      std::unique_ptr<ACE_Laxity_Message_Strategy> strategy (strategy_raw);
      ACE_Dynamic_Message_Queue<ACE_SYNCH> *laxity_queue {};
      ACE_NEW_RETURN (
           laxity_queue,
           ACE_Dynamic_Message_Queue<ACE_SYNCH> (*strategy),
           -1);
      strategy.release ();
      this->owned_queue_.reset (laxity_queue);
      break;
      }

    default:
      return -1;
    }

  this->msg_queue (this->owned_queue_.get ());

  if (this->allocator_ == nullptr)
    {
      Dispatch_Queue_Item_Allocator *allocator {};
      ACE_NEW_RETURN (allocator,
                      Dispatch_Queue_Item_Allocator (ALLOC_POOL_CHUNKS),
                      -1);
      this->owned_allocator_.reset (allocator);
      this->allocator_ = allocator;
    }

  return 0;
}

int
Dispatcher_Task::svc ()
{
  ACE_hthread_t thr_handle {};
  ACE_Thread::self (thr_handle);
  int prio {};

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("getprio not supported on this platform\n")));
          return 0;
        }
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("getprio failed")),
                        -1);
    }

  for (;;)
    {
      ACE_Message_Block *mb {};

      if (this->getq (mb) == -1)
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

      Dispatch_Queue_Item * const qitem =
        dynamic_cast<Dispatch_Queue_Item*> (mb);

      if (qitem == nullptr)
        {
          ACE_Message_Block::release (mb);
          continue;
        }

      Dispatch_Command * const command = qitem->command ();

      ACE_ASSERT (command != nullptr);
      int const result = command->execute ();

      if (command->can_be_deleted ())
        command->destroy ();

      ACE_Message_Block::release (mb);

      if (result == -1)
        break;
    }
  return 0;
}

int
Dispatcher_Task::enqueue (Dispatch_Command const *cmd,
                          QoSDescriptor const &qos_info)
{
  void * const buf = this->allocator_->malloc (sizeof (Dispatch_Queue_Item));

  if (buf == nullptr)
    return -1;

  // ACE_Message_Block::release returns this allocator-backed block to the pool.
  ACE_Message_Block * const mb =
    new (buf) Dispatch_Queue_Item (cmd,
                                   qos_info,
                                   &(this->data_block_),
                                   ACE_Message_Block::DONT_DELETE,
                                   this->allocator_);

  if (this->putq (mb) == -1)
    {
      ACE_Message_Block::release (mb);
      return -1;
    }

  return 0;
}

void Dispatch_Queue_Item::init_i (QoSDescriptor const &qos_info)
{
  this->msg_priority (qos_info.preemption_priority_);
  this->msg_execution_time (qos_info.execution_time_);
  this->msg_deadline_time (qos_info.deadline_);
}

}

