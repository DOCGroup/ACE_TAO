// $Id$

#include "ace/Notification_Queue.h"

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
  ACE_Notification_Buffer **b = 0;

  for (ACE_Unbounded_Queue_Iterator<ACE_Notification_Buffer *> alloc_iter (this->alloc_queue_);
       alloc_iter.next (b) != 0;
       alloc_iter.advance ())
    {
      delete [] *b;
      *b = 0;
    }

  this->alloc_queue_.reset ();
  this->notify_queue_.reset ();
  this->free_queue_.reset ();
}

int ACE_Notification_Queue::
allocate_more_buffers()
{
  ACE_TRACE ("ACE_Notification_Queue::allocate_more_buffers");

  ACE_Notification_Buffer *temp = 0;

  ACE_NEW_RETURN (temp,
		  ACE_Notification_Buffer[ACE_REACTOR_NOTIFICATION_ARRAY_SIZE],
		  -1);

  if (this->alloc_queue_.enqueue_head (temp) == -1)
    {
      delete [] temp;
      return -1;
    }

  for (size_t i = 0; i < ACE_REACTOR_NOTIFICATION_ARRAY_SIZE; ++i)
    if (free_queue_.enqueue_head (temp + i) == -1)
      return -1;

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

  ACE_Notification_Buffer *temp = 0;
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> local_queue;

  size_t queue_size = this->notify_queue_.size ();
  int number_purged = 0;
  size_t i;
  for (i = 0; i < queue_size; ++i)
    {
      if (-1 == this->notify_queue_.dequeue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("dequeue_head")),
                          -1);

      // If this is not a Reactor notify (it is for a particular handler),
      // and it matches the specified handler (or purging all),
      // and applying the mask would totally eliminate the notification, then
      // release it and count the number purged.
      if ((0 != temp->eh_) &&
          (0 == eh || eh == temp->eh_) &&
          ACE_BIT_DISABLED (temp->mask_, ~mask)) // the existing notificationmask
                                                 // is left with nothing when
                                                 // applying the mask
      {
        if (-1 == this->free_queue_.enqueue_head (temp))
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             ACE_LIB_TEXT ("enqueue_head")),
                            -1);

        ACE_Event_Handler *event_handler = temp->eh_;
        event_handler->remove_reference ();

        ++number_purged;
      }
      else
      {
        // To preserve it, move it to the local_queue.
        // But first, if this is not a Reactor notify (it is for a
        // particular handler), and it matches the specified handler
        // (or purging all), then apply the mask
        if ((0 != temp->eh_) &&
            (0 == eh || eh == temp->eh_))
          {
            ACE_CLR_BITS(temp->mask_, mask);
          }

        if (-1 == local_queue.enqueue_head (temp))
          return -1;
      }
    }

  if (this->notify_queue_.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
			 ACE_LIB_TEXT ("Notification queue should be ")
			 ACE_LIB_TEXT ("empty after purging")),
			-1);
    }

  // now put it back in the notify queue
  queue_size = local_queue.size ();
  for (i = 0; i < queue_size; ++i)
    {
      if (-1 == local_queue.dequeue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("dequeue_head")),
                          -1);

      if (-1 == this->notify_queue_.enqueue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("enqueue_head")),
                          -1);
    }

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

  ACE_Notification_Buffer *temp = 0;

  if (free_queue_.dequeue_head (temp) == -1)
    {
      if (allocate_more_buffers() == -1)
	{
	  return -1;
	}

      free_queue_.dequeue_head(temp);
    }

  ACE_ASSERT (temp != 0);
  *temp = buffer;

  if (notify_queue_.enqueue_tail (temp) == -1)
    return -1;

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

  ACE_Notification_Buffer *temp = 0;

  if (notify_queue_.dequeue_head (temp) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("dequeue_head")),
                        -1);
    }

  current = *temp;
  if (free_queue_.enqueue_head (temp) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("enqueue_head")),
                        -1);
    }

  ACE_Notification_Buffer ** n = 0;

  if(!this->notify_queue_.is_empty())
    {
      // The queue is not empty, need to queue another message.
      this->notify_queue_.get (n, 0);
      more_messages_queued = true;
      next = **n;
    }

  return 1;
}

					      
