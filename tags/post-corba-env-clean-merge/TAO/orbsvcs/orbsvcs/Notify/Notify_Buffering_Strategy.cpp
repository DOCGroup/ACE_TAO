// $Id$

#include "Notify_Buffering_Strategy.h"
#include "Notify_Command.h"

#include "orbsvcs/CosNotificationC.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_Buffering_Strategy, "$Id$")

TAO_Notify_Buffering_Strategy::TAO_Notify_Buffering_Strategy (TAO_Notify_Property_Long* const queue_length)
  : queue_length_ (queue_length),
    max_queue_length_ (0),
    order_policy_ (CosNotification::AnyOrder),
    discard_policy_ (CosNotification::AnyOrder)
{
}

TAO_Notify_Buffering_Strategy::~TAO_Notify_Buffering_Strategy ()
{
}

void
TAO_Notify_Buffering_Strategy::execute (ACE_Message_Queue<ACE_SYNCH>* msg_queue, TAO_Notify_Command *mb TAO_ENV_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/, ACE_Time_Value *tv)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Enqueing command priority %d, queue_length = %d\n",
                mb->msg_priority (), queue_length_->value ()));

  int result = 0;

  if (max_queue_length_ != 0 && queue_length_->value () > max_queue_length_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "max queue length reached, discarding event\n"));

      if (this->discard_policy_ == CosNotification::AnyOrder ||
          this->discard_policy_ == CosNotification::FifoOrder)
        {
          ACE_Message_Block *first_item;
          result = msg_queue->dequeue_head (first_item, tv);
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "Invalid discard policy\n"));
          result = -1;
        }
      //case (CosNotification::PriorityOrder):
      //case (CosNotification::DeadlineOrder):
      //case (CosNotification::LifoOrder):

      if (result == -1) // we could not dequeue successfully.
        return; // behave as if we discarded this event.
    }

  // Queue according to order policy
  if (this->order_policy_ == CosNotification::AnyOrder ||
      this->order_policy_ == CosNotification::FifoOrder)
    {
      // Insert at the end of the queue.
      result = msg_queue->enqueue_tail (mb, tv);
    }
  else if (this->order_policy_ == CosNotification::PriorityOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "enqueue in priority order\n"));
      result = msg_queue->enqueue_prio (mb, tv);
    }
  else  // CosNotification::DeadlineOrder
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Invalid discard policy\n"));
      result = -1;
    }

  if (result == -1) // we could not enqueue successfully
    {
      ACE_DEBUG ((LM_DEBUG, "Panic! failed to enqueue event"));
      return; // behave as if we discarded this event.
    }

  // increment the global count of events.
  (*queue_length_)++;
}

void
TAO_Notify_Buffering_Strategy::max_queue_length (CORBA::Long max_queue_length)
{
  this->max_queue_length_ = max_queue_length;
}

void
TAO_Notify_Buffering_Strategy::order_policy (CORBA::Short order_policy)
{
  this->order_policy_ = order_policy;
}

void
TAO_Notify_Buffering_Strategy::discard_policy (CORBA::Short discard_policy)
{
  this->discard_policy_ = discard_policy;
}
