// $Id$

#include "Buffering_Strategy.h"

#if ! defined (__ACE_INLINE__)
#include "Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Buffering_Strategy, "$id$")

#include "ace/Message_Queue.h"
#include "orbsvcs/CosNotificationC.h"
#include "Method_Request.h"
#include "Notify_Extensions.h"
#include "AdminProperties.h"
#include "QoSProperties.h"
#include "Notify_Signal_Property_T.h"
#include "tao/debug.h"

TAO_NS_Buffering_Strategy::TAO_NS_Buffering_Strategy (ACE_Message_Queue<ACE_SYNCH>& msg_queue, TAO_NS_AdminProperties& admin_properties)
  : msg_queue_ (msg_queue),
    queue_length_ (admin_properties.queue_length ()),
    max_queue_length_ (admin_properties.max_queue_length ()),
    max_events_per_consumer_ (CosNotification::MaxEventsPerConsumer, 0),
    order_policy_ (CosNotification::OrderPolicy, CosNotification::AnyOrder),
    discard_policy_ (CosNotification::DiscardPolicy, CosNotification::AnyOrder),
    use_max_queue_ (0),
    blocking_timeout_ (TAO_Notify_Extensions::BlockingPolicy)
{
}

TAO_NS_Buffering_Strategy::~TAO_NS_Buffering_Strategy ()
{
}

void
TAO_NS_Buffering_Strategy::update_qos_properties (const TAO_NS_QoSProperties& qos_properties)
{
  this->max_events_per_consumer_.set (qos_properties);
  this->order_policy_.set (qos_properties);
  this->discard_policy_.set (qos_properties);
  this->blocking_timeout_.set (qos_properties);
}

void
TAO_NS_Buffering_Strategy::execute (TAO_NS_Method_Request& method_request, ACE_Time_Value *tv)
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
                              method_request.msg_priority (), this->queue_length_.value (),
                              this->max_queue_length_.value ()));
      else
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Enqueing command "
                              "priority %d, queue_length = %d, max events"
                              " per consumer = %d\n",
                              method_request.msg_priority (), this->queue_length_.value (),
                              this->max_events_per_consumer_.value ()));
    }

  int result = 0;

  // If we decide to use_max_queue_ and max_queue_length_ is not 0 and
  // the queue_length_ is more than max_queue_length_.
  // Or if we did not decide to use_max_queue_ and max_events_per_consumer_
  // and queue_length_ is more than max_events_per_consumer_.
  CORBA::Long current = this->queue_length_.value ();
  if ((this->use_max_queue_ && this->max_queue_length_ != 0 &&
       current >= this->max_queue_length_.value ()) ||
      (!this->use_max_queue_ && this->max_events_per_consumer_ != 0 &&
       current >= this->max_events_per_consumer_.value ()))
    {
      if (this->blocking_timeout_.is_valid ())
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
                                             this->blocking_timeout_.value ()));
# else
                         ACE_Time_Value (this->blocking_timeout_.value () / 1);
# endif /* ACE_CONFIG_WIN32_H */

            }
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Notify (%P|%t) - Blocking\n"));

          if (this->queue_length_.wait_for_change (&absolute) == -1)
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
                                      this->discard_policy_.value ()));
              else
                ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - max events per "
                                      "consumer reached, discarding event "
                                      "with policy %d\n",
                                      this->discard_policy_.value ()));
            }

          if (this->discard_policy_ == CosNotification::AnyOrder ||
              this->discard_policy_ == CosNotification::FifoOrder)
            {
              ACE_Message_Block *first_item;
              result = this->msg_queue_.dequeue_head (first_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::LifoOrder)
            {
              ACE_Message_Block *last_item;
              result = this->msg_queue_.dequeue_tail (last_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::DeadlineOrder)
            {
              ACE_Message_Block *dequeued_item;
              result = this->msg_queue_.dequeue_deadline (dequeued_item, tv);
            }
          else if (this->discard_policy_ == CosNotification::PriorityOrder)
            {
              ACE_Message_Block *dequeued_item;
              result = this->msg_queue_.dequeue_prio (dequeued_item, tv);
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
              this->queue_length_--;
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
      result = this->msg_queue_.enqueue_tail (&method_request, tv);
    }
  else if (this->order_policy_ == CosNotification::PriorityOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "enqueue in priority order\n"));
      result = this->msg_queue_.enqueue_prio (&method_request, tv);
    }
  else if (this->order_policy_ == CosNotification::DeadlineOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "enqueue in deadline order\n"));
      result = this->msg_queue_.enqueue_deadline (&method_request, tv);
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
      this->queue_length_++;
    }
}
