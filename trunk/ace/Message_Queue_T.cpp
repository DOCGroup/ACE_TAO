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

#include "ace/Strategies.h"     // Need ACE_Notification_Strategy

ACE_RCSID(ace, Message_Queue_T, "$Id$")

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

      for (temp = this->tail_;
           temp != 0;
           temp = temp->prev ())
        if (temp->msg_priority () >= new_item->msg_priority ())
          // Break out when we've located an item that has
          // greater or equal priority.
          break;

      if (temp == 0)
        // Check for simple case of inserting at the head of the queue,
        // where all we need to do is insert <new_item> before the
        // current head.
        return this->enqueue_head_i (new_item);
      else if (temp->next () == 0)
        // Check for simple case of inserting at the tail of the
        // queue, where all we need to do is insert <new_item> after
        // the current tail.
        return this->enqueue_tail_i (new_item);
      else
        {
          // Insert the new message behind the message of
          // greater or equal priority.  This ensures that FIFO order is
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
  if (this->head_ ==0)
  {
    ACE_ERROR_RETURN((LM_ERROR, ASYS_TEXT ("Attempting to dequeue from empty queue")), -1);
  }

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
                                                     ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  // Wait for at least one item to become available.

  if (this->wait_not_empty_cond (ace_mon, timeout) == -1)
    return -1;

  first_item = this->head_;
  return this->cur_count_;
}

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::wait_not_full_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                                    ACE_Time_Value *timeout)
{
  int result = 0;
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  while (this->is_full_i () && result != -1)
    {
      ++this->enqueue_waiters_;
      // @@ Need to add sanity checks for failure...
      mon.release ();
      if (timeout == 0)
        result = this->not_full_cond_.acquire ();
      else
        result = this->not_full_cond_.acquire (*timeout);
      int error = errno;
      mon.acquire ();
      errno = error;
    }
#else
  ACE_UNUSED_ARG (mon);

  // Wait while the queue is full.

  while (this->is_full_i ())
    {
      if (this->not_full_cond_.wait (timeout) == -1)
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
                                                       ACE_Time_Value *timeout)
{
  int result = 0;
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  while (this->is_empty_i () && result != -1)
    {
      ++this->dequeue_waiters_;
      // @@ Need to add sanity checks for failure...
      mon.release ();
      if (timeout == 0)
        result = this->not_empty_cond_.acquire ();
      else
        {
          result = this->not_empty_cond_.acquire (*timeout);
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
      if (this->not_empty_cond_.wait (timeout) == -1)
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
                                                ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, timeout) == -1)
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
                                                ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_prio");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, timeout) == -1)
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
                                         ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue");
  return this->enqueue_prio (new_item, timeout);
}

// Block indefinitely waiting for an item to arrive,
// does not ignore alerts (e.g., signals).

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail (ACE_Message_Block *new_item,
                                              ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::enqueue_tail");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_full_cond (ace_mon, timeout) == -1)
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

// Remove an item from the front of the queue.  If timeout == 0 block
// indefinitely (or until an alert occurs).  Otherwise, block for upto
// the amount of time specified by timeout.

template <ACE_SYNCH_DECL> int
ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head (ACE_Message_Block *&first_item,
                                              ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::dequeue_head");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  if (this->deactivated_)
    {
      errno = ESHUTDOWN;
      return -1;
    }

  if (this->wait_not_empty_cond (ace_mon, timeout) == -1)
  {
    return -1;
  }

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
  , pending_head_ (0)
  , pending_tail_ (0)
  , late_head_ (0)
  , late_tail_ (0)
  , beyond_late_head_ (0)
  , beyond_late_tail_ (0)
  , message_strategy_ (message_strategy)
{
  // note, the ACE_Dynamic_Message_Queue assumes full responsibility for the
  // passed ACE_Dynamic_Message_Strategy object, and deletes it in its own dtor
}

template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::~ACE_Dynamic_Message_Queue (void)
{
  delete &(this->message_strategy_);
}
// dtor: free message strategy and let base class dtor do the rest

template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::remove_messages (
  ACE_Message_Block *&list_head,
  ACE_Message_Block *&list_tail,
  u_int status_flags)
{
  int result = 0;

  // start with an empty list
  list_head = 0;
  list_tail = 0;


  // get the current time
  ACE_Time_Value current_time = ACE_OS::gettimeofday ();

  // refresh priority status boundaries in the queue
  result = this->refresh_queue (current_time);
  if (result < 0)
  {
    return result;
  }

  if ((status_flags & (u_int) ACE_Dynamic_Message_Strategy::PENDING) &&
      (this->pending_head_) && (this->pending_tail_))
  {
    // patch up pointers for the new tail of the queue
    if (this->pending_head_->prev ())
    {
      this->tail_ = this->pending_head_->prev ();
      this->pending_head_->prev ()->next (0);
    }
    else
    {
      // the list has become empty
      this->head_ = 0;
      this->tail_ = 0;
    }

    // point to the head and tail of the list
    list_head = this->pending_head_;
    list_tail = this->pending_tail_;

    // cut the pending messages out of the queue entirely
    this->pending_head_->prev (0);
    this->pending_head_ = 0;
    this->pending_tail_ = 0;
  }

  if ((status_flags & (u_int) ACE_Dynamic_Message_Strategy::LATE) &&
      (this->late_head_) && (this->late_tail_))
  {
    // patch up pointers for the (possibly) new head and tail of the queue
    if (this->late_tail_->next ())
    {
      this->late_tail_->next ()->prev (this->late_head_->prev ());
    }
    else
    {
      this->tail_ = this->late_head_->prev ();
    }
    if (this->late_head_->prev ())
    {
      this->late_head_->prev ()->next (this->late_tail_->next ());
    }
    else
    {
      this->head_ = this->late_tail_->next ();
    }

    // put late messages behind pending messages (if any) being returned
    this->late_head_->prev (list_tail);
    if (list_tail)
    {
      list_tail->next (this->late_head_);
    }
    else
    {
      list_head = this->late_head_;
    }
    list_tail = this->late_tail_;

    this->late_tail_->next (0);
    this->late_head_ = 0;
    this->late_tail_ = 0;
  }

  if ((status_flags & (u_int) ACE_Dynamic_Message_Strategy::BEYOND_LATE) &&
      (this->beyond_late_head_) && (this->beyond_late_tail_))
  {
    // patch up pointers for the new tail of the queue
    if (this->beyond_late_tail_->next ())
    {
      this->head_ = this->beyond_late_tail_->next ();
      this->beyond_late_tail_->next ()->prev (0);
    }
    else
    {
      // the list has become empty
      this->head_ = 0;
      this->tail_ = 0;
    }

    // put beyond late messages at the end of the list being returned
    if (list_tail)
    {
      this->beyond_late_head_->prev (list_tail);
      list_tail->next (this->beyond_late_head_);
    }
    else
    {
      list_head = this->beyond_late_head_;
    }
    list_tail = this->beyond_late_tail_;

    this->beyond_late_tail_->next (0);
    this->beyond_late_head_ = 0;
    this->beyond_late_tail_ = 0;
  }

  // decrement message and size counts for removed messages
  ACE_Message_Block *temp1, *temp2;
  for (temp1 = list_head; temp1 != 0; temp1 = temp1->next ())
  {
    this->cur_count_--;

    for (temp2 = temp1; temp2 != 0; temp2 = temp2->cont ())
    {
      this->cur_bytes_ -= temp2->size ();
    }
  }

  return result;
}
  // Detach all messages with status given in the passed flags from
  // the queue and return them by setting passed head and tail pointers
  // to the linked list they comprise.  This method is intended primarily
  // as a means of periodically harvesting messages that have missed
  // their deadlines, but is available in its most general form.  All
  // messages are returned in priority order, from head to tail, as of
  // the time this method was called.



template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dequeue_head (ACE_Message_Block *&first_item,
                                                        ACE_Time_Value *timeout)
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

  // refresh priority status boundaries in the queue
  result = this->refresh_queue (current_time);
  if (result < 0)
  {
    return result;
  }

  // *now* it's appropriate to wait for an enqueued item
  result = this->wait_not_empty_cond (ace_mon, timeout);
  if (result == -1)
  {
    return result;
  }

  // call the internal dequeue method, which selects an
  // item from the highest priority status portion of
  // the queue that has messages enqueued.
  result = dequeue_head_i (first_item);

  return result;
}
  // Dequeue and return the <ACE_Message_Block *>
  // at the (logical) head of the queue.

template <ACE_SYNCH_DECL> void
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("ACE_Message_Queue<ACE_SYNCH_USE> (base class): \n")));
  this->ACE_Message_Queue<ACE_SYNCH_USE>::dump ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("pending_head_ = %u\n")
              ASYS_TEXT ("pending_tail_ = %u\n")
              ASYS_TEXT ("late_head_ = %u\n")
              ASYS_TEXT ("late_tail_ = %u\n")
              ASYS_TEXT ("beyond_late_head_ = %u\n")
              ASYS_TEXT ("beyond_late_tail_ = %u\n"),
              this->pending_head_,
              this->pending_tail_,
              this->late_head_,
              this->late_tail_,
              this->beyond_late_head_,
              this->beyond_late_tail_));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("message_strategy_ : \n")));
  message_strategy_.dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
  // dump the state of the queue

template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::enqueue_i");

  int result = 0;

  // get the current time
  ACE_Time_Value current_time = ACE_OS::gettimeofday ();

  // refresh priority status boundaries in the queue
  result = this->refresh_queue (current_time);
  if (result < 0)
  {
    return result;
  }

  // where we enqueue depends on the message's priority status
  switch (message_strategy_.priority_status (*new_item, current_time))
  {
    case ACE_Dynamic_Message_Strategy::PENDING:
      if (this->pending_tail_ == 0)
      {
        // Check for simple case of an empty pending queue, where all we need to
        // do is insert <new_item> into the tail of the queue.
        pending_head_ = new_item;
        pending_tail_ = pending_head_;
        result = this->enqueue_tail_i (new_item);
      }
      else
      {
        // enqueue the new message in priority order in the pending sublist
        result = sublist_enqueue_i (new_item,
                                    current_time,
                                    this->pending_head_,
                                    this->pending_tail_,
                                    ACE_Dynamic_Message_Strategy::PENDING);
      }

      break;

    case ACE_Dynamic_Message_Strategy::LATE:
      if (this->late_tail_ == 0)
      {
        late_head_ = new_item;
        late_tail_ = late_head_;

        if (this->pending_head_ == 0)
        {
          // Check for simple case of an empty pending queue, where all
          // we need to do is insert <new_item> into the tail of the queue.
          result = this->enqueue_tail_i (new_item);
        }
        else if (this->beyond_late_tail_ == 0)
        {
          // Check for simple case of an empty beyond late queue, where all
          // we need to do is insert <new_item> into the head of the queue.
          result = this->enqueue_head_i (new_item);
        }
        else
        {
          // otherwise, we can just splice the new message in between
          // the pending and beyond late portions of the queue
          this->beyond_late_tail_->next (new_item);
          new_item->prev (this->beyond_late_tail_);
          this->pending_head_->prev (new_item);
          new_item->next (this->pending_head_);
        }
      }
      else
      {
        // enqueue the new message in priority order in the late sublist
        result = sublist_enqueue_i (new_item,
                                    current_time,
                                    this->late_head_,
                                    this->late_tail_,
                                    ACE_Dynamic_Message_Strategy::LATE);
      }
      break;

    case ACE_Dynamic_Message_Strategy::BEYOND_LATE:
      if (this->beyond_late_tail_ == 0)
      {
        // Check for simple case of an empty beyond late queue, where all
        // we need to do is insert <new_item> into the head of the queue.
        beyond_late_head_ = new_item;
        beyond_late_tail_ = beyond_late_head_;
        result = this->enqueue_head_i (new_item);
      }
      else
      {
        // all beyond late messages have the same (zero) priority, so
        // just put the new one at the end of the beyond late messages
        if (this->beyond_late_tail_->next ())
        {
          this->beyond_late_tail_->next ()->prev (new_item);
        }
        else
        {
          this->tail_ = new_item;
        }

        new_item->next (this->beyond_late_tail_->next ());
        this->beyond_late_tail_->next (new_item);
        new_item->prev (this->beyond_late_tail_);
        this->beyond_late_tail_ = new_item;
      }

      break;

    // should never get here, but just in case...
    default:
      result = -1;
      break;
  }

  return result;
}
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.
  // priority may be *dynamic* or *static* or a combination or *both*
  // It calls the priority evaluation function passed into the Dynamic
  // Message Queue constructor to update the priorities of all enqueued
  // messages.


template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::sublist_enqueue_i (ACE_Message_Block *new_item,
                                                             const ACE_Time_Value &current_time,
                                                             ACE_Message_Block *&sublist_head,
                                                             ACE_Message_Block *&sublist_tail,
                                                             ACE_Dynamic_Message_Strategy::Priority_Status status)
{
  int result = 0;
  ACE_Message_Block *current_item = 0;

  // find message after which to enqueue new item,
  // based on message priority and priority status
  for (current_item = sublist_tail;
       current_item;
       current_item = current_item->prev ())
  {
    if (message_strategy_.priority_status (*current_item, current_time) == status)
    {
      if (current_item->msg_priority () >= new_item->msg_priority ())
      {
        break;
      }
    }
    else
    {
      sublist_head = new_item;
      break;
    }
  }

  if (current_item == 0)
  {
    // if the new message has highest priority of any,
    // put it at the head of the list (and sublist)
    result = enqueue_head_i (new_item);
    sublist_head = new_item;
  }
  else
  {
    // insert the new item into the list
    new_item->next (current_item->next ());
    new_item->prev (current_item);
    if (current_item->next ())
    {
      current_item->next ()->prev (new_item);
    }
    else
    {
      this->tail_ = new_item;
    }

    current_item->next (new_item);

    // if the new item has lowest priority of any in the sublist,
    // move the tail pointer of the sublist back to the new item
    if (current_item == sublist_tail)
    {
      sublist_tail = new_item;
    }
  }

  return result;
}
  // enqueue a message in priority order within a given priority status sublist


template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dequeue_head_i (ACE_Message_Block *&first_item)
{
  ACE_TRACE ("ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::dequeue_head_i");

  int result = 0;
  int last_in_subqueue = 0;

  // first, try to dequeue from the head of the pending list
  if (this->pending_head_)
  {
    first_item = this->pending_head_;

    if (0 == this->pending_head_->prev ())
    {
      this->head_ = this->pending_head_->next ();
    }
    else
    {
      this->pending_head_->prev ()->next (this->pending_head_->next ());
    }

    if (0 == this->pending_head_->next ())
    {
      this->tail_ = this->pending_head_->prev ();
      this->pending_head_ = 0;
      this->pending_tail_ = 0;
    }
    else
    {
      this->pending_head_->next ()->prev (this->pending_head_->prev ());
      this->pending_head_ = this->pending_head_->next ();
    }

    first_item->prev (0);
    first_item->next (0);
  }
  // second, try to dequeue from the head of the late list
  else if (this->late_head_)
  {
        last_in_subqueue =
      (this->late_head_ == this->late_tail_) ? 1 : 0;

    first_item = this->late_head_;

    if (0 == this->late_head_->prev ())
    {
      this->head_ = this->late_head_->next ();
    }
    else
    {
      this->late_head_->prev ()->next (this->late_head_->next ());
    }

    if (0 == this->late_head_->next ())
    {
      this->tail_ = this->late_head_->prev ();
    }
    else
    {
      this->late_head_->next ()->prev (this->late_head_->prev ());
      this->late_head_ = this->late_head_->next ();
    }

    if (last_in_subqueue)
    {
      this->late_head_ = 0;
      this->late_tail_ = 0;
    }

    first_item->prev (0);
    first_item->next (0);
  }
  // finally, try to dequeue from the head of the beyond late list
  else if (this->beyond_late_head_)
  {
        last_in_subqueue =
      (this->beyond_late_head_ == this->beyond_late_tail_) ? 1 : 0;

    first_item = this->beyond_late_head_;
    this->head_ = this->beyond_late_head_->next ();

    if (0 == this->beyond_late_head_->next ())
    {
      this->tail_ = this->beyond_late_head_->prev ();
    }
    else
    {
      this->beyond_late_head_->next ()->prev (this->beyond_late_head_->prev ());
      this->beyond_late_head_ = this->beyond_late_head_->next ();
    }

    if (last_in_subqueue)
    {
      this->beyond_late_head_ = 0;
      this->beyond_late_tail_ = 0;
    }

    first_item->prev (0);
    first_item->next (0);
  }
  else
  {
    // nothing to dequeue: set the pointer to zero and return an error code
    first_item = 0;
    result = -1;
  }

  return result;
}
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // logical queue.  Attempts first to dequeue from the pending
  // portion of the queue, or if that is empty from the late portion,
  // or if that is empty from the beyond late portion, or if that is
  // empty just sets the passed pointer to zero and returns -1.



template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::refresh_queue (const ACE_Time_Value &current_time)
{
  int result;

  result = refresh_pending_queue (current_time);

  if (result != -1)
  {
    result = refresh_late_queue (current_time);
  }

  return result;
}
  // Refresh the queue using the strategy
  // specific priority status function.


template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::refresh_pending_queue (const ACE_Time_Value &current_time)
{
  ACE_Dynamic_Message_Strategy::Priority_Status current_status;

  // refresh priority status boundaries in the queue
  if (this->pending_head_)
  {
    current_status = message_strategy_.priority_status (*this->pending_head_, current_time);
    switch (current_status)
    {
      case ACE_Dynamic_Message_Strategy::BEYOND_LATE:

        // make sure the head of the beyond late queue is set
        // (there may not have been any beyond late messages previously)
        this->beyond_late_head_ = this->head_;

        // zero out the late queue pointers, and set them only if
        // there turn out to be late messages in the pending sublist
        this->late_head_ = 0;
        this->late_tail_ = 0;

        // advance through the beyond late messages in the pending queue
        do
        {
          this->pending_head_ = this->pending_head_->next ();

          if (this->pending_head_)
          {
            current_status = message_strategy_.priority_status (*this->pending_head_,
                                                                current_time);
          }
          else
          {
            break;  // do while
          }

        } while (current_status == ACE_Dynamic_Message_Strategy::BEYOND_LATE);

        if (this->pending_head_)
        {
          // point tail of beyond late sublist to previous item
          this->beyond_late_tail_ = this->pending_head_->prev ();

          if (current_status == ACE_Dynamic_Message_Strategy::PENDING)
          {
            // there are no late messages left in the queue
            break; // switch
          }
          else
          {
            if (current_status != ACE_Dynamic_Message_Strategy::LATE)
            {
              // if we got here, something is *seriously* wrong with the queue
              ACE_ERROR_RETURN((LM_ERROR,
                                ASYS_TEXT ("Unexpected message priority status [%d] (expected LATE)"),
                                (int) current_status),
                               -1);
            }

            // intentionally fall through to the next case
          }
        }
        else
        {
          // there are no pending or late messages left in the queue
          this->beyond_late_tail_ = this->tail_;
          this->pending_head_ = 0;
          this->pending_tail_ = 0;

          break; // switch
        }

      case ACE_Dynamic_Message_Strategy::LATE:

        // make sure the head of the late queue is set (there may not have been
        // any late messages previously, or they may have all become beyond late)
        if (this->late_head_ == 0)
        {
          this->late_head_ = this->pending_head_;
        }

        // advance through the beyond late messages in the pending queue
        do
        {
          this->pending_head_ = this->pending_head_->next ();

          if (this->pending_head_)
          {
            current_status = message_strategy_.priority_status (*this->pending_head_,
                                                                current_time);
          }
          else
          {
            break;  // do while
          }

        } while (current_status == ACE_Dynamic_Message_Strategy::LATE);

        if (this->pending_head_)
        {
          if (current_status != ACE_Dynamic_Message_Strategy::PENDING)
          {
            // if we got here, something is *seriously* wrong with the queue
            ACE_ERROR_RETURN((LM_ERROR,
                              ASYS_TEXT ("Unexpected message priority status [%d] (expected PENDING)"),
                              (int) current_status),
                             -1);
          }

          // point tail of late sublist to previous item
          this->late_tail_ = this->pending_head_->prev ();
        }
        else
        {
          // there are no pending messages left in the queue
          this->late_tail_ = this->tail_;
          this->pending_head_ = 0;
          this->pending_tail_ = 0;
        }

        break; // switch

      case ACE_Dynamic_Message_Strategy::PENDING:

        // do nothing - the pending queue is unchanged

        break; // switch

      default:
        // if we got here, something is *seriously* wrong with the queue
        ACE_ERROR_RETURN((LM_ERROR,
                          ASYS_TEXT ("Unknown message priority status [%d]"),
                          (int) current_status),
                         -1);
        break; // switch
    }
  }

  return 0;
}
  // Refresh the pending queue using the strategy
  // specific priority status function.

template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::refresh_late_queue (const ACE_Time_Value &current_time)
{
  ACE_Dynamic_Message_Strategy::Priority_Status current_status;

  if (this->late_head_)
  {
    current_status = message_strategy_.priority_status (*this->late_head_,
                                                        current_time);
    switch (current_status)
    {
      case ACE_Dynamic_Message_Strategy::BEYOND_LATE:

        // make sure the head of the beyond late queue is set
        // (there may not have been any beyond late messages previously)
        this->beyond_late_head_ = this->head_;

        // advance through the beyond late messages in the late queue
        do
        {
          this->late_head_ = this->late_head_->next ();

          if (this->late_head_)
          {
            current_status = message_strategy_.priority_status (*this->late_head_,
                                                                current_time);
          }
          else
          {
            break;  // do while
          }

        } while (current_status == ACE_Dynamic_Message_Strategy::BEYOND_LATE);

        if (this->late_head_)
        {
          // point tail of beyond late sublist to previous item
          this->beyond_late_tail_ = this->late_head_->prev ();

          if (current_status == ACE_Dynamic_Message_Strategy::PENDING)
          {
            // there are no late messages left in the queue
            this->late_head_ = 0;
            this->late_tail_ = 0;
          }
          else if (current_status != ACE_Dynamic_Message_Strategy::LATE)
          {
            // if we got here, something is *seriously* wrong with the queue
            ACE_ERROR_RETURN((LM_ERROR,
                              ASYS_TEXT ("Unexpected message priority status [%d] (expected LATE)"),
                              (int) current_status),
                             -1);
          }
        }
        else
        {
          // there are no late messages left in the queue
          this->beyond_late_tail_ = this->tail_;
          this->late_head_ = 0;
          this->late_tail_ = 0;
        }

        break;  // switch

      case ACE_Dynamic_Message_Strategy::LATE:

        // do nothing - the late queue is unchanged

        break; // switch

      case ACE_Dynamic_Message_Strategy::PENDING:

        // if we got here, something is *seriously* wrong with the queue
        ACE_ERROR_RETURN((LM_ERROR,
                          ASYS_TEXT ("Unexpected message priority status "
                                     "[%d] (expected LATE or BEYOND_LATE)"),
                          (int) current_status),
                         -1);

        break; // switch

      default:

        // if we got here, something is *seriously* wrong with the queue
        ACE_ERROR_RETURN((LM_ERROR,
                          ASYS_TEXT ("Unknown message priority status [%d]"),
                          (int) current_status),
                         -1);

        break; // switch
    }
  }

  return 0;
}
  // Refresh the late queue using the strategy
  // specific priority status function.


template <ACE_SYNCH_DECL> int
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head (
  ACE_Message_Block *&first_item,
  ACE_Time_Value *timeout)
{
  return ACE_Message_Queue<ACE_SYNCH_USE>::peek_dequeue_head (first_item, timeout);
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
                                                                         u_long dynamic_priority_max,
                                                                         u_long dynamic_priority_offset)
{
  ACE_Deadline_Message_Strategy *adms;

  ACE_NEW_RETURN (adms,
                  ACE_Deadline_Message_Strategy (static_bit_field_mask,
                                                 static_bit_field_shift,
                                                 dynamic_priority_max,
                                                 dynamic_priority_offset),
                  0);

  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*adms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by time to deadline) ACE_Dynamic_Message_Queue


template <ACE_SYNCH_DECL>
ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_laxity_message_queue (size_t hwm,
                                                                       size_t lwm,
                                                                       ACE_Notification_Strategy *ns,
                                                                       u_long static_bit_field_mask,
                                                                       u_long static_bit_field_shift,
                                                                       u_long dynamic_priority_max,
                                                                       u_long dynamic_priority_offset)
{
  ACE_Laxity_Message_Strategy *alms;

  ACE_NEW_RETURN (alms,
                  ACE_Laxity_Message_Strategy (static_bit_field_mask,
                                               static_bit_field_shift,
                                               dynamic_priority_max,
                                               dynamic_priority_offset),
                  0);


  return new ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> (*alms, hwm, lwm, ns);
}
  // factory method for a dynamically prioritized (by laxity) ACE_Dynamic_Message_Queue


#if defined (VXWORKS)

template <ACE_SYNCH_DECL>
ACE_Message_Queue_Vx *
ACE_Message_Queue_Factory<ACE_SYNCH_USE>::create_Vx_message_queue (size_t max_messages,
                                                                   size_t max_message_length,
                                                                   ACE_Notification_Strategy *ns)
{
  return new ACE_Message_Queue_Vx (max_messages, max_message_length, ns);
}
  // factory method for a wrapped VxWorks message queue

#endif /* defined (VXWORKS) */



#endif /* ACE_MESSAGE_QUEUE_T_C */
