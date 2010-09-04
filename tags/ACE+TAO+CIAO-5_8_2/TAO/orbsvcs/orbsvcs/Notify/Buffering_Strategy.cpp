// $Id$

#include "orbsvcs/Notify/Buffering_Strategy.h"

#include "orbsvcs/Notify/Method_Request.h"
#include "orbsvcs/Notify/Notify_Extensions.h"
#include "orbsvcs/Notify/QoSProperties.h"
#include "orbsvcs/Notify/Notify_Extensions.h"

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"

#include "ace/Message_Queue.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Buffering_Strategy::TAO_Notify_Buffering_Strategy (
  TAO_Notify_Message_Queue& msg_queue,
  const TAO_Notify_AdminProperties::Ptr& admin_properties)
: msg_queue_ (msg_queue)
, admin_properties_ (admin_properties)
, global_queue_lock_ (admin_properties->global_queue_lock ())
, global_queue_length_ (admin_properties->global_queue_length ())
, max_queue_length_ (admin_properties->max_global_queue_length ())
, order_policy_ (CosNotification::OrderPolicy, CosNotification::AnyOrder)
, discard_policy_ (CosNotification::DiscardPolicy, CosNotification::AnyOrder)
, max_events_per_consumer_ (CosNotification::MaxEventsPerConsumer)
, blocking_policy_ (TAO_Notify_Extensions::BlockingPolicy)
, global_not_full_ (admin_properties->global_queue_not_full())
, local_not_full_ (global_queue_lock_)
, local_not_empty_ (global_queue_lock_)
, shutdown_ (false)
, tracker_ (0)
{
}

TAO_Notify_Buffering_Strategy::~TAO_Notify_Buffering_Strategy ()
{
}

void
TAO_Notify_Buffering_Strategy::update_qos_properties
  (const TAO_Notify_QoSProperties& qos_properties)
{
  this->order_policy_.set (qos_properties);
  this->discard_policy_.set (qos_properties);
  this->max_events_per_consumer_.set(qos_properties);
  this->blocking_policy_.set (qos_properties);
}

void
TAO_Notify_Buffering_Strategy::shutdown (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_);

  if (this->shutdown_)
  {
    return;
  }

  this->shutdown_ = true;

  this->local_not_empty_.broadcast ();
  this->global_not_full_.broadcast();
  this->local_not_full_.broadcast();
}

ACE_Time_Value
TAO_Notify_Buffering_Strategy::oldest_event (void)
{
  ACE_Time_Value tv (ACE_Time_Value::max_time);
  ACE_Message_Block* mb = 0;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, tv);
  TAO_Notify_Message_Queue::ITERATOR itr (this->msg_queue_);
  while(itr.next (mb))
    {
      TAO_Notify_Method_Request_Queueable* event =
        dynamic_cast<TAO_Notify_Method_Request_Queueable*> (mb);
      if (event != 0)
        {
          const ACE_Time_Value& etime = event->creation_time ();
          if (etime < tv)
            tv = etime;
        }
      itr.advance ();
    }

  return tv;
}


TAO_Notify_Buffering_Strategy::Tracker::Tracker (void)
  : child_ (0)
{
}


TAO_Notify_Buffering_Strategy::Tracker::~Tracker (void)
{
}


void
TAO_Notify_Buffering_Strategy::Tracker::register_child (TAO_Notify_Buffering_Strategy::Tracker * child)
{
  if (this->child_ == 0)
    {
      this->child_ = child;
    }
  else if (this->child_ != child)
    {
      this->child_->register_child (child);
    }
  // we simply ignore duplicate registrations.
}

void
TAO_Notify_Buffering_Strategy::Tracker::unregister_child (TAO_Notify_Buffering_Strategy::Tracker * child)
{
  if (this->child_ == child)
    {
      this->child_ = this->child_->child_;
    }
  else if (this->child_ != 0)
    {
      this->child_->unregister_child (child);
    }
}


int
TAO_Notify_Buffering_Strategy::enqueue (TAO_Notify_Method_Request_Queueable* method_request)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  if (this->shutdown_)
    return -1;

  bool discarded_existing = false;

  bool local_overflow = this->max_events_per_consumer_.is_valid() &&
    static_cast <CORBA::Long> (this->msg_queue_.message_count ()) >= this->max_events_per_consumer_.value();

  bool global_overflow = this->max_queue_length_.value () != 0 &&
    this->global_queue_length_ >= this->max_queue_length_.value ();

  while (local_overflow || global_overflow)
    {
      if (blocking_policy_.is_valid())
        {
          ACE_Time_Value timeout;
          ORBSVCS_Time::TimeT_to_Time_Value(timeout, blocking_policy_.value());
          // Condition variables take an absolute time
          timeout += ACE_OS::gettimeofday();
          if (local_overflow)
            {
              local_not_full_.wait(&timeout);
            }
          else
            {
              global_not_full_.wait(&timeout);
            }
          if (errno != ETIME)
            {
              local_overflow =
                this->max_events_per_consumer_.is_valid() &&
                static_cast <CORBA::Long> (this->msg_queue_.message_count ()) >= this->max_events_per_consumer_.value();
              global_overflow =
                this->max_queue_length_.value () != 0 &&
                this->global_queue_length_ >= this->max_queue_length_.value ();
              continue;
            }
        }
      if (tracker_ != 0)
        {
          tracker_->count_queue_overflow (local_overflow, global_overflow);
        }

      discarded_existing = this->discard(method_request);
      if (discarded_existing)
        {
          --this->global_queue_length_;
          local_not_full_.signal();
          global_not_full_.signal();
        }
      break;
    }

  if (! (local_overflow || global_overflow) || discarded_existing)
    {
      if (this->queue (method_request) == -1)
        {
          ACE_DEBUG((LM_DEBUG,
                     "Notify (%P|%t) - Panic! failed to enqueue event\n"));
          return -1;
        }

      ++this->global_queue_length_;

      local_not_empty_.signal ();
    }
  else
    {
      ACE_DEBUG((LM_DEBUG,
                 "Notify (%P|%t) - Panic! did not attempt to enqueue event\n"));
      return -1;
    }

  size_t count = this->msg_queue_.message_count ();
  if (this->tracker_ != 0)
    {
      this->tracker_->update_queue_count (count);
    }

  return ACE_Utils::truncate_cast<int> (count);
}

int
TAO_Notify_Buffering_Strategy::dequeue (TAO_Notify_Method_Request_Queueable* &method_request, const ACE_Time_Value *abstime)
{
  ACE_Message_Block *mb;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  if ( this->shutdown_ )
    return -1;

  while (this->msg_queue_.message_count () == 0)
    {
      this->local_not_empty_.wait (abstime);

      if (this->shutdown_)
        return -1;

      if (errno == ETIME)
        return 0;
    }

  if (this->msg_queue_.dequeue (mb) == -1)
    return -1;

  if (this->tracker_ != 0)
    {
      this->tracker_->update_queue_count (this->msg_queue_.message_count ());
    }

  method_request = dynamic_cast<TAO_Notify_Method_Request_Queueable*>(mb);

  if (method_request == 0)
    return -1;

  --this->global_queue_length_;
  local_not_full_.signal();
  global_not_full_.signal();

  return 1;
}

void
TAO_Notify_Buffering_Strategy::set_tracker (
                        TAO_Notify_Buffering_Strategy::Tracker* tracker)
{
  if (this->tracker_ == 0)
    {
      this->tracker_ = tracker;
    }
  else if (this->tracker_ != tracker)
    {
      this->tracker_->register_child (tracker);
    }
}

int
TAO_Notify_Buffering_Strategy::queue (TAO_Notify_Method_Request_Queueable* method_request)
{
  if ( this->shutdown_ )
    return -1;

  CORBA::Short order = this->order_policy_.value();

  if (! this->order_policy_.is_valid() ||
      order == CosNotification::AnyOrder ||
      order == CosNotification::FifoOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - enqueue in fifo order\n"));
      return this->msg_queue_.enqueue_tail (method_request);
    }

  if (order == CosNotification::PriorityOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - enqueue in priority order\n"));
      return this->msg_queue_.enqueue_prio (method_request);
    }

  if (order == CosNotification::DeadlineOrder)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - enqueue in deadline order\n"));
      return this->msg_queue_.enqueue_deadline (method_request);
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Invalid order policy\n"));
  return this->msg_queue_.enqueue_tail (method_request);
}

bool
TAO_Notify_Buffering_Strategy::discard (TAO_Notify_Method_Request_Queueable* method_request)
{
  if (this->shutdown_)
    {
      return false;
    }

  ACE_Message_Block* mb = 0;
  int result = -1;

  if (this->discard_policy_.is_valid() == 0 ||
      this->discard_policy_ == CosNotification::AnyOrder ||
      this->discard_policy_ == CosNotification::FifoOrder)
    {
      result = this->msg_queue_.dequeue_head (mb);
    }
  else if (this->discard_policy_ == CosNotification::LifoOrder)
    {
      // The most current message is NOT the newest one in the queue. It's
      // the one we're about to add to the queue.
      result = -1;
    }
  else if (this->discard_policy_ == CosNotification::DeadlineOrder)
    {
      result = this->msg_queue_.dequeue_deadline (mb);
    }
  else if (this->discard_policy_ == CosNotification::PriorityOrder)
    {
      result = this->msg_queue_.dequeue_prio (mb);
      if (mb->msg_priority() >= method_request->msg_priority())
        {
          this->msg_queue_.enqueue_prio (mb);
          result = -1;
        }
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - Invalid discard policy\n"));
      result = this->msg_queue_.dequeue_head (mb);
    }

  if (result != -1)
    {
      ACE_Message_Block::release (mb);
      return true;
    }

  return false;
}



TAO_END_VERSIONED_NAMESPACE_DECL
