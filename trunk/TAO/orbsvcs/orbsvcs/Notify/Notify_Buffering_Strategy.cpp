// $Id$

#include "Notify_Buffering_Strategy.h"
#include "Notify_Command.h"

#include "orbsvcs/CosNotificationC.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_Buffering_Strategy, "$Id$")

TAO_Notify_Buffering_Strategy::TAO_Notify_Buffering_Strategy (
                                 TAO_Notify_Signal_Property_Long* const queue_length)
  : queue_length_ (queue_length),
    max_queue_length_ (0),
    max_events_per_consumer_ (0),
    order_policy_ (CosNotification::AnyOrder),
    discard_policy_ (CosNotification::AnyOrder),
    use_max_queue_ (0),
    blocking_timeout_ (0),
    use_blocking_timeout_ (0)
{
}

TAO_Notify_Buffering_Strategy::~TAO_Notify_Buffering_Strategy ()
{
}

void
TAO_Notify_Buffering_Strategy::execute (ACE_Message_Queue<ACE_SYNCH>* msg_queue, TAO_Notify_Command *mb TAO_ENV_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/, ACE_Time_Value *tv)
{
  // use_max_queue_ may be set to 0.  If max_events_per_consumer_ is
  // 0, then we are going to use the maximum queue length regardless.
  if (this->max_events_per_consumer_ == 0)
    this->use_max_queue_ = 1;

  if (TAO_debug_level > 0)
    {
      if (this->use_max_queue_)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Enqueing command "
                              "priority %d, queue_length = %d, max queue"
                              " length = %d\n",
                              mb->priority (), this->queue_length_->value (),
                              this->max_queue_length_));
      else
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Enqueing command "
                              "priority %d, queue_length = %d, max events"
                              " per consumer = %d\n",
                              mb->priority (), this->queue_length_->value (),
                              this->max_events_per_consumer_));
    }

  int result = 0;

  // If we decide to use_max_queue_ and max_queue_length_ is not 0 and
  // the queue_length_ is more than max_queue_length_.
  // Or if we did not decide to use_max_queue_ and max_events_per_consumer_
  // and queue_length_ is more than max_events_per_consumer_.
  CORBA::Long current = this->queue_length_->value ();
  if ((this->use_max_queue_ && this->max_queue_length_ != 0 &&
       current > this->max_queue_length_) ||
      (!this->use_max_queue_ && this->max_events_per_consumer_ != 0 &&
       current > this->max_events_per_consumer_))
    {
      if (this->use_blocking_timeout_)
        {

          // I am multiplying blocking_timeout_ by 1 because it is a
          // CORBA::ULongLong, which on some platforms doesn't automatically
          // convert to the long that the ACE_Time_Value expects.  The /
          // operator returns a 32-bit integer.
          ACE_Time_Value absolute;

          if (this->blocking_timeout_ != 0)
            {
              absolute = ACE_OS::gettimeofday () +
# if defined (ACE_CONFIG_WIN32_H)
                         ACE_Time_Value (
                            ACE_static_cast (long,
                                             this->blocking_timeout_));
# else
                         ACE_Time_Value (this->blocking_timeout_ / 1);
# endif /* ACE_CONFIG_WIN32_H */

            }
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Notify (%P|%t) - Blocking\n"));

          if (this->queue_length_->wait_for_change (&absolute) == -1)
            {
              // We timed out waiting for the queue length to change
              ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                                    "Timed out blocking on the client\n"));
              return;
            }

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Notify (%P|%t) - Resuming\n"));
        }
      else
        {
          if (TAO_debug_level > 0)
            {
              if (this->use_max_queue_)
                ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - max queue length "
                                      "reached, discarding event with "
                                      "policy %d\n",
                                      this->discard_policy_));
              else
                ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - max events per "
                                      "consumer reached, discarding event "
                                      "with policy %d\n",
                                      this->discard_policy_));
            }

          if (this->discard_policy_ == CosNotification::AnyOrder ||
              this->discard_policy_ == CosNotification::FifoOrder)
            {
              ACE_Message_Block *first_item;
              result = msg_queue->dequeue_head (first_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::LifoOrder)
            {
              ACE_Message_Block *last_item;
              result = msg_queue->dequeue_tail (last_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::DeadlineOrder)
            {
              ACE_Message_Block *dequeued_item;
              result = msg_queue->dequeue_deadline (dequeued_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::PriorityOrder)
            {
              ACE_Message_Block *dequeued_item;
              result = msg_queue->dequeue_prio (dequeued_item, tv);
            }
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                                      "Invalid discard policy\n"));
              result = -1;
            }

          if (result == -1) // we could not dequeue successfully.
            {
              return; // behave as if we discarded this event.
            }
          else
            {
              // decrement the global count of events since we successfully
              // dequeued an item from the message queue.
              (*this->queue_length_)--;
            }
        }
    }

  // Queue according to order policy
  if (this->order_policy_ == CosNotification::AnyOrder ||
      this->order_policy_ == CosNotification::FifoOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "enqueue in fifo order\n"));
      // Insert at the end of the queue.
      result = msg_queue->enqueue_tail (mb, tv);
    }
  else if (this->order_policy_ == CosNotification::PriorityOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "enqueue in priority order\n"));
      result = msg_queue->enqueue_prio (mb, tv);
    }
  else if (this->order_policy_ == CosNotification::DeadlineOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "enqueue in deadline order\n"));
      result = msg_queue->enqueue_deadline (mb, tv);
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Invalid order policy\n"));
      result = -1;
    }

  if (result == -1) // we could not enqueue successfully
    {
      ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                            "Panic! failed to enqueue event"));
      // behave as if we discarded this event.
    }
  else
    {
      // increment the global count of events.
      (*this->queue_length_)++;
    }
}

void
TAO_Notify_Buffering_Strategy::max_queue_length (CORBA::Long max_queue_length)
{
  this->max_queue_length_ = max_queue_length;
}

void
TAO_Notify_Buffering_Strategy::max_events_per_consumer (CORBA::Long max_events_per_consumer)
{
  this->max_events_per_consumer_ = max_events_per_consumer;
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

void
TAO_Notify_Buffering_Strategy::blocking_timeout (TimeBase::TimeT timeout)
{
  this->blocking_timeout_ = timeout;
  this->use_blocking_timeout_ = 1;
}
