// Message_Queue.cpp
// $Id$

#if !defined (ACE_MESSAGE_QUEUE_C)
#define ACE_MESSAGE_QUEUE_C

#define ACE_BUILD_DLL
#include "ace/Message_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue)

template <ACE_SYNCH_1>
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::ACE_Message_Queue_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue)
  : queue_ (queue),
    curr_ (queue_.head_)
{
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::next (ACE_Message_Block *&entry)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_ != 0)
    {
      entry = this->curr_;
      return 1;
    }
  else
    return 0;
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::done (void) const
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  return this->curr_ == 0;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  this->curr_ = this->curr_->next ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_1> void 
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::dump (void) const
{
}

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Iterator)

template <ACE_SYNCH_1>
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::ACE_Message_Queue_Reverse_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue)
  : queue_ (queue),
    curr_ (queue_.tail_)
{
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::next (ACE_Message_Block *&entry)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_ != 0)
    {
      entry = this->curr_;
      return 1;
    }
  else
    return 0;
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::done (void) const
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  return this->curr_ == 0;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  this->curr_ = this->curr_->prev ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_1> void 
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::dump (void) const
{
}

template <ACE_SYNCH_1> void
ACE_Message_Queue<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
	      "deactivated = %d\n"
	      "low_water_mark = %d\n"
	      "high_water_mark = %d\n"	     
	      "cur_bytes = %d\n"	     
	      "cur_count = %d\n",
	      "head_ = %u\n",
	      "tail_ = %u\n",
	      this->deactivated_,
	      this->low_water_mark_, 
	      this->high_water_mark_,
	      this->cur_bytes_, 
	      this->cur_count_,
	      this->head_,
	      this->tail_));
  ACE_DEBUG ((LM_DEBUG,"notfull_cond: \n"));
  notfull_cond_.dump();
  ACE_DEBUG ((LM_DEBUG,"notempty_cond: \n"));
  notempty_cond_.dump();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <ACE_SYNCH_1>
ACE_Message_Queue<ACE_SYNCH_2>::ACE_Message_Queue (size_t hwm, 
						   size_t lwm,
						   ACE_Notification_Strategy *ns)
  : notempty_cond_ (this->lock_),
    notfull_cond_ (this->lock_)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::ACE_Message_Queue");

  if (this->open (hwm, lwm, ns) == -1)
    ACE_ERROR ((LM_ERROR, "open"));
}

template <ACE_SYNCH_1>
ACE_Message_Queue<ACE_SYNCH_2>::~ACE_Message_Queue (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::~ACE_Message_Queue");
  if (this->head_ != 0 && this->close () == -1)
    ACE_ERROR ((LM_ERROR, "close"));
}
  
// Don't bother locking since if someone calls this function more than
// once for the same queue, we're in bigger trouble than just
// concurrency control!

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::open (size_t hwm, 
				      size_t lwm,
				      ACE_Notification_Strategy *ns)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::open");
  this->high_water_mark_ = hwm;
  this->low_water_mark_  = lwm;
  this->deactivated_ = 0;
  this->cur_bytes_ = 0;
  this->cur_count_ = 0;
  this->tail_ = 0;
  this->head_ = 0;
  this->notification_strategy_ = ns;
  return 0;
}

// Implementation of the public deactivate() method
// (assumes locks are held).

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::deactivate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::deactivate_i");
  int current_status = 
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;

  // Wakeup all waiters.
  this->notempty_cond_.broadcast ();
  this->notfull_cond_.broadcast ();

  this->deactivated_ = 1;
  return current_status;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::activate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::activate_i");
  int current_status = 
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;
  this->deactivated_ = 0;
  return current_status;
}

// Clean up the queue if we have not already done so! 

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::close (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::close");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  int res = this->deactivate_i ();

  // Free up the remaining message on the list 

  for (this->tail_ = 0; this->head_ != 0; )
    {
      this->cur_count_--;

      ACE_Message_Block *temp;

      // Decrement all the counts.
      for (temp = this->head_;
	   temp != 0; 
	   temp = temp->cont ())
	this->cur_bytes_ -= temp->size ();

      temp = this->head_;
      this->head_ = this->head_->next ();

      // Make sure to use <release> rather than <delete> since this is
      // reference counted.
      temp->release ();
    }

  return res;
}

// Actually put the node at the end (no locking so must be called with
// locks held).

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_tail_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_tail_i");

  if (new_item == 0)
    return -1;

  // List was empty, so build a new one.
  if (this->tail_ == 0)
    {
      this->head_ = new_item;
      this->tail_ = new_item;
      new_item->next (0);
      new_item->prev (0);
    }
  // Link at the end.
  else 
    {
      new_item->next (0);
      this->tail_->next (new_item);
      new_item->prev (this->tail_);
      this->tail_ = new_item;
    }

  // Make sure to count *all* the bytes in a composite message!!! 

  for (ACE_Message_Block *temp = new_item; 
       temp != 0; 
       temp = temp->cont ())
    this->cur_bytes_ += temp->size ();

  this->cur_count_++;

  // Tell any blocked threads that the queue has a new item!
  if (this->notempty_cond_.signal () != 0)
    return -1;
  else
    return this->cur_count_;
}

// Actually put the node at the head (no locking) 

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_head_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_head_i");

  if (new_item == 0)
    return -1;

  new_item->prev (0);
  new_item->next (this->head_);

  if (this->head_ != 0)
      this->head_->prev (new_item);
  else
    this->tail_ = new_item;

  this->head_ = new_item;

  // Make sure to count *all* the bytes in a composite message!!!

  for (ACE_Message_Block *temp = new_item; 
       temp != 0; 
       temp = temp->cont ())
    this->cur_bytes_ += temp->size ();

  this->cur_count_++;

  // Tell any blocked threads that the queue has a new item! 
  if (this->notempty_cond_.signal () != 0)
    return -1;
  else
    return this->cur_count_;
}

// Actually put the node at its proper position relative to its
// priority.

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_i");

  if (new_item == 0)
    return -1;

  if (this->head_ == 0)
    // Check for simple case of an empty queue, where all we need to
    // do is insert <new_item> into the head.
    return this->enqueue_head_i (new_item);
  else
    {
      ACE_Message_Block *temp;

      // Figure out where the new item goes relative to its priority.
      // We start looking from the highest priority to the lowest
      // priority.

      for (temp = this->tail_;
	   temp != 0;
	   temp = temp->prev ())
	{
	  if (temp->msg_priority () >= new_item->msg_priority ())
	    // Break out when we've located an item that has higher
	    // priority that <new_item>.
	    break;
	}

      if (temp == 0)
	// Check for simple case of inserting at the head of the queue,
	// where all we need to do is insert <new_item> before the
	// current head.
	return this->enqueue_head_i (new_item);
      else if (temp->next () == 0)
	// Check for simple case of inserting at the end of the
	// queue, where all we need to do is insert <new_item> after
	// the current tail.
	return this->enqueue_tail_i (new_item);
      else
	{
	  // Insert the message right before the item of equal or
	  // higher priority.  This ensures that FIFO order is
	  // maintained when messages of the same priority are
	  // inserted consecutively.
	  new_item->prev (temp);
	  new_item->next (temp->next ());
	  temp->next ()->prev (new_item);
	  temp->next (new_item);
	}
    }

  // Make sure to count *all* the bytes in a composite message!!!

  for (ACE_Message_Block *temp = new_item; 
       temp != 0; 
       temp = temp->cont ())
    this->cur_bytes_ += temp->size ();

  this->cur_count_++;

  // Tell any blocked threads that the queue has a new item! 
  if (this->notempty_cond_.signal () != 0)
    return -1;
  else
    return this->cur_count_;
}

// Actually get the first ACE_Message_Block (no locking, so must be called
// with locks held).  This method assumes that the queue has at least
// one item in it when it is called. 

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::dequeue_head_i (ACE_Message_Block *&first_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::dequeue_head_i");
  first_item = this->head_;
  this->head_ = this->head_->next ();

  if (this->head_ == 0)
    this->tail_ = 0;
  else
    // The prev pointer of the first message block has to point to
    // NULL...
    this->head_->prev (0);

  // Make sure to subtract off all of the bytes associated with this
  // message.
  for (ACE_Message_Block *temp = first_item; 
       temp != 0; 
       temp = temp->cont ())
    this->cur_bytes_ -= temp->size ();

  this->cur_count_--;

#if 0
  if (this->cur_bytes_ <= this->low_water_mark_)
  // If queue is no longer full signal any waiting threads. 
#endif /* 0 */

  if (this->notfull_cond_.signal () != 0)
    return -1;
  else
    return this->cur_count_;
}

// Take a look at the first item without removing it. 

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::peek_dequeue_head (ACE_Message_Block *&first_item, 
						   ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::peek_dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  // Wait for at least one item to become available 

  while (this->is_empty_i ())
    {
      if (this->notempty_cond_.wait (tv) == -1)
	{
	  if (errno == ETIME)
	    errno = EWOULDBLOCK;
	  return -1;
	}
      if (this->deactivated_)
	{
	  errno = ESHUTDOWN;
	  return -1;
	}
    }

  first_item = this->head_;
  return this->cur_count_;
}

// Block indefinitely waiting for an item to arrive,
// does not ignore alerts (e.g., signals). 

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_head (ACE_Message_Block *new_item, 
					      ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  int queue_count;
  {
    if (this->deactivated_)
      {
	errno = ESHUTDOWN;
	return -1;
      }

    // Wait while the queue is full 

    while (this->is_full_i ())
      {
	if (this->notfull_cond_.wait (tv) == -1)
	  {
	    if (errno == ETIME)
	      errno = EWOULDBLOCK;
	    return -1;
	  }
	if (this->deactivated_)
	  {
	    errno = ESHUTDOWN;
	    return -1;
	  }
      }

    queue_count = this->enqueue_head_i (new_item);
  }
  if (queue_count == -1)
    return -1;
  else
    {
      this->notify ();
      return queue_count;
    }
}

// Enqueue an <ACE_Message_Block *> into the <Message_Queue> in
// accordance with its <msg_priority> (0 is lowest priority).  Returns
// -1 on failure, else the number of items still on the queue.

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_prio (ACE_Message_Block *new_item, 
					      ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_prio");

  int queue_count;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

    if (this->deactivated_)
      {
	errno = ESHUTDOWN;
	return -1;
      }

    // Wait while the queue is full 

    while (this->is_full_i ())
      {
	if (this->notfull_cond_.wait (tv) == -1)
	  {
	    if (errno == ETIME)
	      errno = EWOULDBLOCK;
	    return -1;
	  }
	if (this->deactivated_)
	  {
	    errno = ESHUTDOWN;
	    return -1;
	  }
      }

    queue_count = this->enqueue_i (new_item);
  }
  if (queue_count == -1)
    return -1;
  else
    {
      this->notify ();
      return queue_count;
    }
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::enqueue (ACE_Message_Block *new_item, 
					 ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue");
  return this->enqueue_prio (new_item, tv);
}

// Block indefinitely waiting for an item to arrive,
// does not ignore alerts (e.g., signals). 

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::enqueue_tail (ACE_Message_Block *new_item, 
					      ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::enqueue_tail");

  int queue_count;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

    if (this->deactivated_)
      {
	errno = ESHUTDOWN;
	return -1;
      }

    // Wait while the queue is full 

    while (this->is_full_i ())
      {
	if (this->notfull_cond_.wait (tv) == -1)
	  {
	    if (errno == ETIME)
	      errno = EWOULDBLOCK;
	    return -1;
	  }
	if (this->deactivated_)
	  {
	    errno = ESHUTDOWN;
	    return -1;
	  }
      }
    queue_count = this->enqueue_tail_i (new_item);
  }
  if (queue_count == -1)
    return -1;
  else
    {
      this->notify ();
      return queue_count;
    }
}

// Remove an item from the front of the queue.  If TV == 0 block 
// indefinitely (or until an alert occurs).  Otherwise, block for upto
// the amount of time specified by TV. 

template <ACE_SYNCH_1> int 
ACE_Message_Queue<ACE_SYNCH_2>::dequeue_head (ACE_Message_Block *&first_item, 
					      ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  // Wait while the queue is empty.

  while (this->is_empty_i ())
    {
      if (this->notempty_cond_.wait (tv) == -1)
	{
	  if (errno == ETIME)
	    errno = EWOULDBLOCK;
	  return -1;
	}
      if (this->deactivated_)
	{
	  errno = ESHUTDOWN;
	  return -1;
	}
    }

  return this->dequeue_head_i (first_item);
}

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::notify (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::dequeue_head");

  // By default, don't do anything.
  if (this->notification_strategy_ == 0)
    return 0;
  else
    return this->notification_strategy_->notify ();
}

#endif /* ACE_MESSAGE_QUEUE_C */
