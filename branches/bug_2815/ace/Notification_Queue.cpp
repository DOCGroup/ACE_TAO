// $Id$

#include "ace/Notification_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Notification_Queue.inl"
#endif /* __ACE_INLINE__ */

ACE_Notification_Queue::
ACE_Notification_Queue()
  : ACE_Copy_Disabled()
  , alloc_queue_()
  , notify_queue_()
  , free_queue_()
{
}

ACE_Notification_Queue::
~ACE_Notification_Queue()
{
  reset();
}

int
ACE_Notification_Queue::
open()
{
  ACE_TRACE ("ACE_Notification_Queue::open");

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

  if (!this->free_queue_.is_empty ())
    return 0;

  return allocate_more_buffers();
}

void
ACE_Notification_Queue::
reset()
{
  ACE_TRACE ("ACE_Notification_Queue::reset");

  // Free up the dynamically allocated resources.
  ACE_Notification_Queue_Node **b = 0;

  for (ACE_Unbounded_Queue_Iterator<ACE_Notification_Queue_Node *> alloc_iter (this->alloc_queue_);
       alloc_iter.next (b) != 0;
       alloc_iter.advance ())
    {
      delete [] *b;
      *b = 0;
    }

  this->alloc_queue_.reset ();

  // Swap with an empty list to reset the contents
  Buffer_List().swap(notify_queue_);
  Buffer_List().swap(free_queue_);
}

int ACE_Notification_Queue::
allocate_more_buffers()
{
  ACE_TRACE ("ACE_Notification_Queue::allocate_more_buffers");

  ACE_Notification_Queue_Node *temp = 0;

  ACE_NEW_RETURN (temp,
		  ACE_Notification_Queue_Node[ACE_REACTOR_NOTIFICATION_ARRAY_SIZE],
		  -1);

  if (this->alloc_queue_.enqueue_head (temp) == -1)
    {
      delete [] temp;
      return -1;
    }

  for (size_t i = 0; i < ACE_REACTOR_NOTIFICATION_ARRAY_SIZE; ++i)
    {
      free_queue_.push_front(temp + i);
    }

  return 0;
}

int
ACE_Notification_Queue::
purge_pending_notifications(ACE_Event_Handler * eh,
			    ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Notification_Queue::purge_pending_notifications");

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

  if (this->notify_queue_.is_empty ())
    return 0;

  Buffer_List local_queue;

  int number_purged = 0;
  while(!notify_queue_.is_empty())
    {
      ACE_Notification_Queue_Node * node = notify_queue_.pop_front();

      if (!node->matches_for_purging(eh))
	{
	  // Easy case, save the node and continue;
	  local_queue.push_back(node);
	  continue;
	}

      if (!node->mask_disables_all_notifications(mask))
	{
	  node->clear_mask(mask);
	  local_queue.push_back(node);
	  continue;
	}

      free_queue_.push_back(node);
      ACE_Event_Handler *event_handler = node->get().eh_;
      event_handler->remove_reference ();
      ++number_purged;
    }

  // now put it back in the notify queue
  local_queue.swap(notify_queue_);

  return number_purged;
}

int ACE_Notification_Queue::
push_new_notification(
  ACE_Notification_Buffer const & buffer)
{
  ACE_TRACE ("ACE_Notification_Queue::push_new_notification");

  bool notification_required = false;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

  // No pending notifications.
  if (this->notify_queue_.is_empty ())
    notification_required = true;

  if (free_queue_.is_empty())
    {
      if (allocate_more_buffers() == -1)
	{
	  return -1;
	}
    }

  ACE_Notification_Queue_Node * node =
    free_queue_.pop_front();

  ACE_ASSERT (node != 0);
  node->set(buffer);

  notify_queue_.push_back(node);

  if (!notification_required)
    {
      return 0;
    }

  return 1;
}

int
ACE_Notification_Queue::pop_next_notification(
  ACE_Notification_Buffer & current,
  bool & more_messages_queued,
  ACE_Notification_Buffer & next)
{
  ACE_TRACE ("ACE_Notification_Queue::pop_next_notification");

  more_messages_queued = false;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

  if (notify_queue_.is_empty ())
    {
      return 0;
    }

  ACE_Notification_Queue_Node * node =
    notify_queue_.pop_front();

  current = node->get();
  free_queue_.push_back(node);

  if(!this->notify_queue_.is_empty())
    {
      more_messages_queued = true;
      next = notify_queue_.head()->get();
    }

  return 1;
}
