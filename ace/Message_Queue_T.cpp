// $Id$

#if !defined (ACE_MESSAGE_QUEUE_T_C)
#define ACE_MESSAGE_QUEUE_T_C

#define ACE_BUILD_DLL
// #include Message_Queue.h instead of Message_Queue_T.h to avoid
// circular include problems.
#include "ace/Message_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue_T.i"
#endif /* __ACE_INLINE__ */

#include "ace/Strategies.h"	// Need ACE_Notification_Strategy

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue)

ACE_ALLOC_HOOK_DEFINE(ACE_Dynamic_Message_Queue)


//////////////////////////////////////
// class ACE_Message_Queue_Iterator //
//////////////////////////////////////

template <ACE_SYNCH_DECL>
ACE_Message_Queue_Iterator<ACE_SYNCH_USE>::ACE_Message_Queue_Iterator (ACE_Message_Queue <ACE_SYNCH_USE> &q)
  : queue_ (q),
    curr_ (q.head_)
{
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Iterator<ACE_SYNCH_USE>::next (ACE_Message_Block *&entry)
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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Iterator<ACE_SYNCH_USE>::done (void) const
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  return this->curr_ == 0;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Iterator<ACE_SYNCH_USE>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_)
    this->curr_ = this->curr_->next ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_DECL> void
ACE_Message_Queue_Iterator<ACE_SYNCH_USE>::dump (void) const
{
}

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Iterator)


//////////////////////////////////////////////
// class ACE_Message_Queue_Reverse_Iterator //
//////////////////////////////////////////////

template <ACE_SYNCH_DECL>
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>::ACE_Message_Queue_Reverse_Iterator (ACE_Message_Queue <ACE_SYNCH_USE> &q)
  : queue_ (q),
    curr_ (queue_.tail_)
{
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>::next (ACE_Message_Block *&entry)
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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>::done (void) const
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  return this->curr_ == 0;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_)
    this->curr_ = this->curr_->prev ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_DECL> void
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>::dump (void) const
{
}


/////////////////////////////
// class ACE_Message_Queue //
/////////////////////////////

template <ACE_SYNCH_DECL> void
ACE_Message_Queue<ACE_SYNCH_USE>::dump (void) const
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("deactivated = %d\n")
              ASYS_TEXT ("low_water_mark = %d\n")
              ASYS_TEXT ("high_water_mark = %d\n")
              ASYS_TEXT ("cur_bytes = %d\n")
              ASYS_TEXT ("cur_count = %d\n")
              ASYS_TEXT ("head_ = %u\n")
              ASYS_TEXT ("tail_ = %u\n"),
              this->deactivated_,
              this->low_water_mark_,
              this->high_water_mark_,
              this->cur_bytes_,
              this->cur_count_,
              this->head_,
              this->tail_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("not_full_cond: \n")));
  not_full_cond_.dump ();
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("not_empty_cond: \n")));
  not_empty_cond_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <ACE_SYNCH_DECL>
ACE_Message_Queue<ACE_SYNCH_USE>::ACE_Message_Queue (size_t hwm,
                                                     size_t lwm,
                                                     ACE_Notification_Strategy *ns)
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  : not_empty_cond_ (0),
    not_full_cond_ (0),
    enqueue_waiters_ (0),
    dequeue_waiters_ (0)
#else
  : not_empty_cond_ (this->lock_),
    not_full_cond_ (this->lock_)
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::ACE_Message_Queue");

  if (this->open (hwm, lwm, ns) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("open")));
}

template <ACE_SYNCH_DECL>
ACE_Message_Queue<ACE_SYNCH_USE>::~ACE_Message_Queue (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::~ACE_Message_Queue");
  if (this->head_ != 0 && this->close () == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("close")));
}

// Don't bother locking since if someone calls this function more than
// once for the same queue, we're in bigger trouble than just
// concurrency control!

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::open (size_t hwm,
                                        size_t lwm,
                                        ACE_Notification_Strategy *ns)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::open");
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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::deactivate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::deactivate_i");
  int current_status =
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;

  // Wakeup all waiters.
#if !defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  this->not_empty_cond_.broadcast ();
  this->not_full_cond_.broadcast ();
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */

  this->deactivated_ = 1;
  return current_status;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::activate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::activate_i");
  int current_status =
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;
  this->deactivated_ = 0;
  return current_status;
}

// Clean up the queue if we have not already done so!

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::close (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::close");
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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::signal_enqueue_waiters (void)
{
#if !defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  if (this->not_full_cond_.signal () != 0)
    return -1;
#else
  if (this->enqueue_waiters_ > 0)
    {
      --this->enqueue_waiters_;
      return this->not_full_cond_.release ();
    }
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
  return 0;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::signal_dequeue_waiters (void)
{
#if !defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  // Tell any blocked threads that the queue has a new item!
  if (this->not_empty_cond_.signal () != 0)
    return -1;
#else
  if (this->dequeue_waiters_ > 0)
    {
      --this->dequeue_waiters_;
      return this->not_empty_cond_.release ();
    }
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
  return 0;
}

// Actually put the node at the end (no locking so must be called with
// locks held).

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail_i");

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

  if (this->signal_dequeue_waiters () == -1)
    return -1;
  else
    return this->cur_count_;
}

// Actually put the node at the head (no locking)

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_head_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_head_i");

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

  if (this->signal_dequeue_waiters () == -1)
    return -1;
  else
    return this->cur_count_;
}

// Actually put the node at its proper position relative to its
// priority.

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_i");

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

      for (temp = this->head_;
           temp != 0;
           temp = temp->next ())
        if (temp->msg_priority () < new_item->msg_priority ())
          // Break out when we've located an item that has lower
          // priority that <new_item>.
          break;

      if (temp == 0)
        // Check for simple case of inserting at the tail of the queue,
        // where all we need to do is insert <new_item> after the
        // current tail.
        return this->enqueue_tail_i (new_item);
      else if (temp->prev () == 0)
        // Check for simple case of inserting at the head of the
        // queue, where all we need to do is insert <new_item> before
        // the current head.
        return this->enqueue_head_i (new_item);
      else
        {
          // Insert the new message ahead of the item of
          // lesser priority.  This ensures that FIFO order is
          // maintained when messages of the same priority are
          // inserted consecutively.
          new_item->next (temp);
          new_item->prev (temp->prev ());
          temp->prev ()->next (new_item);
          temp->prev (new_item);
        }
    }

  // Make sure to count *all* the bytes in a composite message!!!

  for (ACE_Message_Block *temp = new_item;
       temp != 0;
       temp = temp->cont ())
    this->cur_bytes_ += temp->size ();

  this->cur_count_++;

  if (this->signal_dequeue_waiters () == -1)
    return -1;
  else
    return this->cur_count_;
}

// Actually get the first ACE_Message_Block (no locking, so must be
// called with locks held).  This method assumes that the queue has at
// least one item in it when it is called.

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head_i (ACE_Message_Block *&first_item)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head_i");
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

  if (this->signal_enqueue_waiters () == -1)
    return -1;
  else
    return this->cur_count_;
}

// Take a look at the first item without removing it.

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head (ACE_Message_Block *&first_item,
                                                     ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  // Wait for at least one item to become available.

  if (this->wait_not_empty_cond (ace_mon, tv) == -1)
    return -1;

  first_item = this->head_;
  return this->cur_count_;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::wait_not_full_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                                    ACE_Time_Value *tv)
{
  int result = 0;
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  while (this->is_full_i () && result != -1)
    {
      ++this->enqueue_waiters_;
      // @@ Need to add sanity checks for failure...
      mon.release ();
      if (tv == 0)
        result = this->not_full_cond_.acquire ();
      else
        result = this->not_full_cond_.acquire (*tv);
      int error = errno;
      mon.acquire ();
      errno = error;
    }
#else
  ACE_UNUSED_ARG (mon);

  // Wait while the queue is full.

  while (this->is_full_i ())
    {
      if (this->not_full_cond_.wait (tv) == -1)
        {
          if (errno == ETIME)
            errno = EWOULDBLOCK;
          result = -1;
          break;
        }
      if (this->deactivated_)
        {
          errno = ESHUTDOWN;
          result = -1;
          break;
        }
    }
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
  return result;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::wait_not_empty_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                                       ACE_Time_Value *tv)
{
  int result = 0;
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  while (this->is_empty_i () && result != -1)
    {
      ++this->dequeue_waiters_;
      // @@ Need to add sanity checks for failure...
      mon.release ();
      if (tv == 0)
        result = this->not_empty_cond_.acquire ();
      else
        {
          result = this->not_empty_cond_.acquire (*tv);
          if (result == -1 && errno == ETIME)
            errno = EWOULDBLOCK;
        }
      int error = errno;
      mon.acquire ();
      errno = error;
    }
#else
  ACE_UNUSED_ARG (mon);

  // Wait while the queue is empty.

  while (this->is_empty_i ())
    {
      if (this->not_empty_cond_.wait (tv) == -1)
        {
          if (errno == ETIME)
            errno = EWOULDBLOCK;
          result = -1;
          break;
        }
      if (this->deactivated_)
        {
          errno = ESHUTDOWN;
          result = -1;
          break;
        }
    }
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
  return result;
}

// Block indefinitely waiting for an item to arrive, does not ignore
// alerts (e.g., signals).

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_head (ACE_Message_Block *new_item,
                                                ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, tv) == -1)
    return -1;

  int queue_count = this->enqueue_head_i (new_item);

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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_prio (ACE_Message_Block *new_item,
                                                ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_prio");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, tv) == -1)
    return -1;

  int queue_count = this->enqueue_i (new_item);

  if (queue_count == -1)
    return -1;
  else
    {
      this->notify ();
      return queue_count;
    }
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue (ACE_Message_Block *new_item,
                                         ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue");
  return this->enqueue_prio (new_item, tv);
}

// Block indefinitely waiting for an item to arrive,
// does not ignore alerts (e.g., signals).

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail (ACE_Message_Block *new_item,
                                              ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, tv) == -1)
    return -1;

  int queue_count = this->enqueue_tail_i (new_item);

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

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head (ACE_Message_Block *&first_item,
                                              ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_empty_cond (ace_mon, tv) == -1)
    return -1;

  return this->dequeue_head_i (first_item);
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::notify (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::notify");

  // By default, don't do anything.
  if (this->notification_strategy_ == 0)
    return 0;
  else
    return this->notification_strategy_->notify ();
}


/////////////////////////////////////
// class ACE_Dynamic_Message_Queue //
/////////////////////////////////////

  // = Initialization and termination methods.
template <ACE_SYNCH_DECL> 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::ACE_Dynamic_Message_Queue (
                                                      ACE_Dynamic_Message_Strategy & message_strategy,
                                                      size_t hwm,
                                                      size_t lwm,
                                                      ACE_Notification_Strategy *ns)
  : ACE_Message_Queue<ACE_SYNCH_USE> (hwm, lwm, ns)
  , message_strategy_ (message_strategy)
{
  // note, the ACE_Dynamic_Message_Queue assumes full responsibility for the
  // passed ACE_Dynamic_Message_Strategy object, and deletes it in its own dtor 
}

template <ACE_SYNCH_DECL> 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::~ACE_Dynamic_Message_Queue (void)
{
  delete &message_strategy_;
}
// dtor: free message strategy and let base class dtor do the rest

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_i");

  int result;

  // get the current time
  ACE_Time_Value current_time = ACE_OS::gettimeofday ();  

  // refresh dynamic priority of the new message
  result = message_strategy_.update_priority (*new_item, current_time);
  if (result < 0)
  {
    return result;
  }

  // refresh dynamic priorities of messages in the queue
  result = this->refresh_priorities (current_time);
  if (result < 0)
  {
    return result;
  }

  // reorganize the queue according to the new priorities
  result = this->refresh_queue (current_time);
  if (result < 0)
  {
    return result;
  }

  // invoke the base class method
  result = ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_i (new_item);

  return result;
}
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.
  // priority may be *dynamic* or *static* or a combination or *both*
  // It calls the priority evaluation function passed into the Dynamic
  // Message Queue constructor to update the priorities of all enqueued 
  // messages.

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dequeue_head (ACE_Message_Block *&first_item, 
                                                        ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dequeue_head");

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  int result;

  // get the current time
  ACE_Time_Value current_time = ACE_OS::gettimeofday ();
  
  // refresh dynamic priorities of messages in the queue
  result = this->refresh_priorities (current_time);
  if (result < 0)
  {
    return result;
  }

  // reorganize the queue according to the new priorities,
  // possibly dropping messages which are later than can
  // be represented by the range of priority values
  result = this->refresh_queue (current_time);
  if (result < 0)
  {
    return result;
  }

  // *now* it's appropriate to wait for an enqueued item
  result = this->wait_not_empty_cond (ace_mon, tv);
  if (result == -1)
  {
    return result;
  }

  // invoke the internal virtual method
  return this->dequeue_head_i (first_item);
}
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::refresh_priorities (const ACE_Time_Value & tv)
{
  int result = 0;

  // apply the priority update function to all enqueued
  // messages, starting at the head of the queue
  ACE_Message_Block *temp = ACE_Message_Queue<ACE_SYNCH_USE>::head_;
  while (temp)
  {
    result = message_strategy_.update_priority (*temp, tv);
    if (result < 0)
    {
      break;
    }
    
    temp = temp->next ();
  }

  return result;
}
  // refresh the priorities in the queue according
  // to a specific priority assignment function

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::refresh_queue (const ACE_Time_Value & tv)
{
  // Remove messages that are later than the priority range can represent
  int result = remove_stale_messages (tv);
  if (result < 0)
  {
    return result;
  }

  // Refresh the order of messages in the queue, 
  // putting pending messages ahead of late messages
  return reorder_queue (tv);
}
  // refresh the order of messages in the queue
  // after refreshing their priorities


template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::remove_stale_messages (const ACE_Time_Value & tv)
{
  int result = 0;

  // start at the beginning of the list
  ACE_Message_Block *current = head_;

  // maintain a list of dropped messages to
  // be appended to the end of the list after
  // the sweep is complete
  ACE_Message_Block *append_list_head = 0;
  ACE_Message_Block *append_list_tail = 0;

  while (current)
  {
    // messages that have overflowed the given time bounds must be removed
    if (message_strategy_.is_beyond_late (*current, tv))
    {
      // find the end of the chain of overflowed messages
      ACE_Message_Block *remove_head = current;
      ACE_Message_Block *remove_tail = current;
      while ((remove_tail) && (remove_tail->next ()) &&
             message_strategy_.is_beyond_late (*(remove_tail->next ()), tv))
      {
        // extend the chain of messages to be removed
        remove_tail = remove_tail->next ();
      }

      // fix up list pointers to bypass the overflowed message chain

      if (remove_tail->next ())
      {
        remove_tail->next ()->prev (remove_head->prev ());
      }
      else
      {
        tail_ = remove_head->prev ();
      }

      if (remove_head->prev ())
      {
        remove_head->prev ()->next (remove_tail->next ());
      }
      else
      {
        head_ = remove_tail->next ();
      }

      // move the current pointer past the end of the chain
      current = remove_tail->next ();

      // Cut the chain of overflowed messages out of the list
      remove_head->prev (0);
      remove_tail->next (0);
      
      // Call strategy's drop_message method on each overflowed message. 
      // Cannot just delete each message even though reference counting
      // at the data bloc level means that the underlying data block will
      // not be deleted if another message block is still pointing to it. 
      // If the entire set of message blocks is known in advance, they may
      // be allocated on the stack instead of the heap (to speed performance),
      // and the caller *cannot* surrender ownership of the memory to the
      // list. Putting this policy in the strategy allows the correct memory
      // management scheme to be configured in either case.
      ACE_Message_Block *temp1 = remove_head;
      ACE_Message_Block *temp2 = remove_head->next ();
      ACE_Message_Block *size_temp;
      size_t msg_size;
      while (temp1)
      {
        // Make sure to count *all* the bytes in a composite message!!!
        for (size_temp = temp1, msg_size = 0;
             size_temp != 0;
             size_temp = size_temp->cont ())
        {
          msg_size += size_temp->size ();
        }

        result = message_strategy_.drop_message (temp1);
        if (result < 0)
        {
          return result;
        }

        if (temp1)
        {
          // if the message was not destroyed, zero out its priority and
          // put it on the list to append to the back of the queue
          temp1->msg_priority (0);
          temp1->next (0);
          if (append_list_tail)
          {
            temp1->prev (append_list_tail);
            append_list_tail->next (temp1);
          }
          else
          {
            temp1->prev (0);
            append_list_head = temp1;
          }
          append_list_tail = temp1;
        }
        else
        {
          // if the message was destroyed, decrease the message
          // count and byte count in the message queue
          this->cur_count_--;
          this->cur_bytes_ -= msg_size;
        }

        temp1 = temp2;
        temp2 = temp2 ? temp2->next () : temp2;
      }
    }
    else 
    {
      current = current->next ();    
    }
  }

  // append any saved dropped messages to the end of the queue
  if (append_list_tail)
  {
    if (tail_)
    {
      tail_->next (append_list_head);
      append_list_head->prev (tail_);
      tail_ = append_list_tail;
    }
    else
    {
      head_ = append_list_head;
      tail_ = append_list_tail;
    }
  }

  return result;
}
  // Remove messages that are later than the priority range can represent.

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::reorder_queue (const ACE_Time_Value & tv)
{
  // if the queue is not empty, and the first message is late, need to reorder
  if ((head_) && (! message_strategy_.is_pending (*head_, tv)))
  {
    // find the end of the chain of newly late messages 
    // (since the last time the queue was reordered)
    ACE_Message_Block *reorder_head = head_;
    ACE_Message_Block *reorder_tail = head_;
    while ((reorder_tail) && (reorder_tail->next ()) &&
            reorder_tail->next ()->msg_priority () <= reorder_head->msg_priority ())
    {
      // extend the chain of messages to be removed
      reorder_tail = reorder_tail->next ();
    }

    // if a proper subset of the queue is out of order, reorganize the queue
    if (reorder_tail != tail_)
    {
      // fix up list pointers to bypass the overflowed message chain
      if (reorder_tail->next ())
      {
        reorder_tail->next ()->prev (reorder_head->prev ());
      }
      else
      {
        tail_ = reorder_head->prev ();
      }
      if (reorder_head->prev ())
      {
        reorder_head->prev ()->next (reorder_tail->next ());
      }
      else
      {
        head_ = reorder_tail->next ();
      }
    }
  }

  return 0;
}
  // Refresh the order of messages in the queue.


template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head (
  ACE_Message_Block *&first_item,
  ACE_Time_Value *tv)
{
  return ACE_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head (first_item, tv);
}
  // private method to hide public base class method: just calls base class method

template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_tail (
  ACE_Message_Block *new_item,
  ACE_Time_Value *timeout)
{
  return ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail (new_item, timeout);
}
  // private method to hide public base class method: just calls base class method


template <ACE_SYNCH_DECL> int 
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_head (
  ACE_Message_Block *new_item,
  ACE_Time_Value *timeout)
{
  return ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_head (new_item, timeout);
}
  // private method to hide public base class method: just calls base class method



/////////////////////////////////////
// class ACE_Message_Queue_Factory //
/////////////////////////////////////

template <ACE_SYNCH_DECL>
ACE_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_static_message_queue (size_t hwm,
                                                                       size_t lwm,
                                                                       ACE_Notification_Strategy *ns)
{
  return new ACE_Message_Queue<ACE_SYNCH_USE> (hwm, lwm, ns);
}
  // factory method for a statically prioritized ACE_Message_Queue

template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_deadline_message_queue (size_t hwm,
                                                                         size_t lwm,
                                                                         ACE_Notification_Strategy *ns,
                                                                         u_long static_bit_field_mask,
                                                                         u_long static_bit_field_shift,
                                                                         u_long pending_threshold,
                                                                         u_long dynamic_priority_max,
                                                                         u_long dynamic_priority_offset)
{
  ACE_Deadline_Message_Strategy *adms;

  ACE_NEW_RETURN (adms,
                  ACE_Deadline_Message_Strategy (static_bit_field_mask,
                                                 static_bit_field_shift,
                                                 pending_threshold,
                                                 dynamic_priority_max,
                                                 dynamic_priority_offset),
                  0);

  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*adms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by time to deadline) ACE_Dynamic_Message_Queue

template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_deadline_cleanup_message_queue (size_t hwm,
                                                                                 size_t lwm,
                                                                                 ACE_Notification_Strategy *ns,
                                                                                 u_long static_bit_field_mask,
                                                                                 u_long static_bit_field_shift,
                                                                                 u_long pending_threshold,
                                                                                 u_long dynamic_priority_max,
                                                                                 u_long dynamic_priority_offset)
{
  ACE_Deadline_Cleanup_Message_Strategy *adcms;

  ACE_NEW_RETURN (adcms,
                  ACE_Deadline_Cleanup_Message_Strategy (static_bit_field_mask,
                                                         static_bit_field_shift,
                                                         pending_threshold,
                                                         dynamic_priority_max,
                                                         dynamic_priority_offset),
                  0);

  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*adcms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by time to deadline) 
  // ACE_Dynamic_Message_Queue, with automatic cleanup of beyond late messages


template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_laxity_message_queue (size_t hwm,
                                                                       size_t lwm,
                                                                       ACE_Notification_Strategy *ns,
                                                                       u_long static_bit_field_mask,
                                                                       u_long static_bit_field_shift,
                                                                       u_long pending_threshold,
                                                                       u_long dynamic_priority_max,
                                                                       u_long dynamic_priority_offset)
{
  ACE_Laxity_Message_Strategy *alms;

  ACE_NEW_RETURN (alms,
                  ACE_Laxity_Message_Strategy (static_bit_field_mask,
                                               static_bit_field_shift,
                                               pending_threshold,
                                               dynamic_priority_max,
                                               dynamic_priority_offset),
                  0);


  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*alms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by laxity) ACE_Dynamic_Message_Queue


template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_laxity_cleanup_message_queue (size_t hwm,
                                                                               size_t lwm,
                                                                               ACE_Notification_Strategy *ns,
                                                                               u_long static_bit_field_mask,
                                                                               u_long static_bit_field_shift,
                                                                               u_long pending_threshold,
                                                                               u_long dynamic_priority_max,
                                                                               u_long dynamic_priority_offset)
{
  ACE_Laxity_Message_Strategy *alcms;

  ACE_NEW_RETURN (alcms,
                  ACE_Laxity_Cleanup_Message_Strategy (static_bit_field_mask,
                                                       static_bit_field_shift,
                                                       pending_threshold,
                                                       dynamic_priority_max,
                                                       dynamic_priority_offset),
                  0);

  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*alcms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by laxity) 
  // ACE_Dynamic_Message_Queue, with automatic cleanup of beyond late messages

#endif /* ACE_MESSAGE_QUEUE_T_C */
