// $Id$

#include "Buffering_Strategy.h"

#if ! defined (__ACE_INLINE__)
#include "Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Buffering_Strategy, "$Id$")

#include "ace/Message_Queue.h"
#include "orbsvcs/CosNotificationC.h"
#include "Method_Request.h"
#include "Notify_Extensions.h"
#include "QoSProperties.h"
#include "tao/debug.h"

TAO_NS_Buffering_Strategy::TAO_NS_Buffering_Strategy (TAO_NS_Message_Queue& msg_queue, TAO_NS_AdminProperties_var& admin_properties, CORBA::Long batch_size)
  : msg_queue_ (msg_queue),
    admin_properties_ (admin_properties),
    global_queue_lock_ (admin_properties->global_queue_lock ()),
    global_queue_not_full_condition_ (admin_properties->global_queue_not_full_condition ()),
    global_queue_length_ (admin_properties->global_queue_length ()),
    max_global_queue_length_ (admin_properties->max_global_queue_length ()),
    max_local_queue_length_ (0),
    order_policy_ (CosNotification::OrderPolicy, CosNotification::AnyOrder),
    discard_policy_ (CosNotification::DiscardPolicy, CosNotification::AnyOrder),
    use_discarding_ (1),
    local_queue_not_full_condition_ (global_queue_lock_),
    batch_size_ (batch_size),
    batch_size_reached_condition_ (global_queue_lock_),
    shutdown_ (0)
{
}

TAO_NS_Buffering_Strategy::~TAO_NS_Buffering_Strategy ()
{
}

void
TAO_NS_Buffering_Strategy::update_qos_properties (const TAO_NS_QoSProperties& qos_properties)
{
  this->order_policy_.set (qos_properties);

  if (this->discard_policy_.set (qos_properties) != -1)
    {
      this->use_discarding_ = 1;
    }

  TAO_NS_Property_Time blocking_timeout (TAO_Notify_Extensions::BlockingPolicy);

  if (blocking_timeout.set (qos_properties) != -1) // if set to a valid time, init the blocking_time_
    {
      this->use_discarding_ = 0;

      this->blocking_time_ =
# if defined (ACE_CONFIG_WIN32_H)
        ACE_Time_Value (ACE_static_cast (long, blocking_timeout.value ()));
# else
      ACE_Time_Value (blocking_timeout.value () / 1);
# endif /* ACE_CONFIG_WIN32_H */
    }
}

void
TAO_NS_Buffering_Strategy::shutdown (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->global_queue_lock_);

  this->shutdown_ = 1;

  this->global_queue_not_full_condition_.broadcast ();
  this->local_queue_not_full_condition_.broadcast ();
  this->batch_size_reached_condition_.broadcast ();
}

int
TAO_NS_Buffering_Strategy::enqueue (TAO_NS_Method_Request& method_request)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  // while either local or global max reached
  while ((this->max_local_queue_length_ != 0 &&
          this->msg_queue_.message_count () == this->max_local_queue_length_)
         ||
         (this->max_global_queue_length_.value () != 0 &&
          this->global_queue_length_ == this->max_global_queue_length_.value ()))
    {
      if (this->shutdown_ == 1) // if we're shutdown, don't play this silly game.
        return -1;

      if (this->use_discarding_ == 1)
        {
          if (this->global_queue_length_ == this->max_global_queue_length_.value ()
              && this->msg_queue_.message_count () == 0) // global max. reached but can't discard
            {
              // block. this is a hack because the real solution is to locate the appropriate queue and dequeue from it.
              this->global_queue_not_full_condition_.wait ();
            }
          else // local max reached or, at global max but non-zero local count.
            {
              if (this->discard () == -1)
                return -1;

              --this->global_queue_length_;

              // ACE_DEBUG ((LM_DEBUG, "Discarded from %x, global_queue_length = %d\n", this, this->global_queue_length_));

              this->global_queue_not_full_condition_.signal ();
              this->local_queue_not_full_condition_.signal ();
            }
          }
        else // block
          {
            if (this->msg_queue_.message_count () == this->max_local_queue_length_) // local maximum reached
              {
                if (this->blocking_time_ == ACE_Time_Value::zero) // wait forever if need be.
                  {
                    this->local_queue_not_full_condition_.wait ();
                  }
                else // finite blocking time.
                  {
                    ACE_Time_Value absolute = ACE_OS::gettimeofday () + this->blocking_time_;

                    if (this->local_queue_not_full_condition_.wait (&absolute) == -1) // returns -1 on timeout
                      return -1; // Note message is discarded if it could not be enqueued in the given time.
                  }
              }
            else  // global max reached
              {
                if (this->blocking_time_ == ACE_Time_Value::zero) // wait forever if need be.
                  {
                    this->global_queue_not_full_condition_.wait ();
                  }
                else // finite blocking time.
                  {
                    ACE_Time_Value absolute = ACE_OS::gettimeofday () + blocking_time_;

                    if (this->global_queue_not_full_condition_.wait (&absolute) == -1) // returns -1 on timeout
                      return -1;
                  }
              }
          } // block
    } // while

  if (this->queue (method_request) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                  "Panic! failed to enqueue event"));
      return -1;
    }

  ++this->global_queue_length_;

  // ACE_DEBUG ((LM_DEBUG, "Inserted to %x, global_queue_length = %d\n", this, this->global_queue_length_));

  if (this->msg_queue_.message_count () == this->batch_size_)
    batch_size_reached_condition_.signal ();

  return this->msg_queue_.message_count ();
}

int
TAO_NS_Buffering_Strategy::dequeue (TAO_NS_Method_Request* &method_request, const ACE_Time_Value *abstime)
{
  ACE_Message_Block *mb;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  while (this->msg_queue_.message_count () < this->batch_size_) // block
    {
      this->batch_size_reached_condition_.wait (abstime);

      if (this->shutdown_ == 1) // if we're shutdown, don't play this silly game.
        return -1;

      if (errno == ETIME)
        return 0;
    }

  if (this->msg_queue_.dequeue (mb) == -1)
    return -1;

  method_request = ACE_dynamic_cast (TAO_NS_Method_Request*, mb);

  if (method_request == 0)
    return -1;

  --this->global_queue_length_;

  // ACE_DEBUG ((LM_DEBUG, "Dequeued from %x, global_queue_length = %d\n", this, this->global_queue_length_));

  this->global_queue_not_full_condition_.signal ();
  this->local_queue_not_full_condition_.signal ();

  return 1;
}

int
TAO_NS_Buffering_Strategy::queue (TAO_NS_Method_Request& method_request)
{
  int result;

  // Queue according to order policy
  if (this->order_policy_ == CosNotification::AnyOrder ||
      this->order_policy_ == CosNotification::FifoOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                    "enqueue in fifo order\n"));
      // Insert at the end of the queue.
      result = this->msg_queue_.enqueue_tail (&method_request);
    }
  else if (this->order_policy_ == CosNotification::PriorityOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                    "enqueue in priority order\n"));
      result = this->msg_queue_.enqueue_prio (&method_request);
    }
  else if (this->order_policy_ == CosNotification::DeadlineOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                    "enqueue in deadline order\n"));
      result = this->msg_queue_.enqueue_deadline (&method_request);
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Invalid order policy\n"));

      result = -1;
    }

  return result;
}

int
TAO_NS_Buffering_Strategy::discard (void)
{
  ACE_Message_Block *mb;
  int result;

  if (this->discard_policy_ == CosNotification::AnyOrder ||
      this->discard_policy_ == CosNotification::FifoOrder)
    {
      result = this->msg_queue_.dequeue_head (mb);
    }
  else if (this->discard_policy_ == CosNotification::LifoOrder)
    {
      result = this->msg_queue_.dequeue_tail (mb);
    }
  else if (this->discard_policy_ == CosNotification::DeadlineOrder)
    {
      result = this->msg_queue_.dequeue_deadline (mb);
    }
  else if (this->discard_policy_ == CosNotification::PriorityOrder)
    {
      result = this->msg_queue_.dequeue_prio (mb);
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                    "Invalid discard policy\n"));
      result = -1;
    }

  return result;
}
