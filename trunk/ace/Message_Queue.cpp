// $Id$

#if !defined (ACE_MESSAGE_QUEUE_C)
#define ACE_MESSAGE_QUEUE_C

#define ACE_BUILD_DLL
#include "ace/Message_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

#if defined (VXWORKS)

////////////////////////////////
// class ACE_Message_Queue_Vx //
////////////////////////////////

void
ACE_Message_Queue_Vx::dump (void) const
{
  ACE_TRACE ("ACE_Message_Queue_Vx::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("deactivated = %d\n")
              ASYS_TEXT ("low_water_mark = %d\n")
              ASYS_TEXT ("high_water_mark = %d\n")
              ASYS_TEXT ("cur_bytes = %d\n")
              ASYS_TEXT ("cur_count = %d\n")
              ASYS_TEXT ("head_ = %u\n")
              ASYS_TEXT ("MSG_Q_ID = %u\n"),
              this->deactivated_,
              this->low_water_mark_,
              this->high_water_mark_,
              this->cur_bytes_,
              this->cur_count_,
              this->head_,
              this->tail_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Message_Queue_Vx::ACE_Message_Queue_Vx (size_t max_messages,
                                            size_t max_message_length,
                                            ACE_Notification_Strategy *ns)
  : ACE_Message_Queue<ACE_NULL_SYNCH> (0, 0, ns),
    max_messages_ (ACE_static_cast (int, max_messages)),
    max_message_length_ (ACE_static_cast (int, max_message_length))
{
  ACE_TRACE ("ACE_Message_Queue_Vx::ACE_Message_Queue_Vx");

  if (this->open (max_messages_, max_message_length_, ns) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("open")));
}

ACE_Message_Queue_Vx::~ACE_Message_Queue_Vx (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::~ACE_Message_Queue_Vx");
}

// Don't bother locking since if someone calls this function more than
// once for the same queue, we're in bigger trouble than just
// concurrency control!

int
ACE_Message_Queue_Vx::open (size_t max_messages,
                            size_t max_message_length,
                            ACE_Notification_Strategy *ns)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::open");
  this->high_water_mark_ = 0;
  this->low_water_mark_  = 0;
  this->deactivated_ = 0;
  this->cur_bytes_ = 0;
  this->cur_count_ = 0;
  this->head_ = 0;
  this->notification_strategy_ = ns;
  this->max_messages_ = ACE_static_cast (int, max_messages);
  this->max_message_length_ = ACE_static_cast (int, max_message_length);

  if (tail_)
    {
      // Had already created a msgQ, so delete it.
      close ();
      activate_i ();
    }

  return (this->tail_ =
            ACE_reinterpret_cast (ACE_Message_Block *,
              ::msgQCreate (max_messages_,
                            max_message_length_,
                            MSG_Q_FIFO))) == NULL ? -1 : 0;
}

// Implementation of the public deactivate() method
// (assumes locks are held).

int
ACE_Message_Queue_Vx::deactivate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::deactivate_i");
  int current_status =
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;

  this->deactivated_ = 1;
  return current_status;
}

int
ACE_Message_Queue_Vx::activate_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::activate_i");
  int current_status =
    this->deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;
  this->deactivated_ = 0;
  return current_status;
}

// Clean up the queue if we have not already done so!

int
ACE_Message_Queue_Vx::close (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::close");
  // Don't lock, because we don't have a lock.  It shouldn't be
  // necessary, anyways.

  this->deactivate_i ();

  // Don't bother to free up the remaining message on the list,
  // because we don't have any way to iterate over what's in the
  // queue.

  return ::msgQDelete (msgq ());
}

int
ACE_Message_Queue_Vx::signal_enqueue_waiters (void)
{
  // No-op.
  return 0;
}

int
ACE_Message_Queue_Vx::signal_dequeue_waiters (void)
{
  // No-op.
  return 0;
}

int
ACE_Message_Queue_Vx::enqueue_tail_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::enqueue_tail_i");

  if (new_item == 0)
    return -1;

  // Don't try to send a composite message!!!!  Only the first
  // block will be sent.

  this->cur_count_++;

  // Always use this method to actually send a message on the queue.
  if (::msgQSend (msgq (),
                  new_item->rd_ptr (),
                  new_item->size (),
                  WAIT_FOREVER,
                  MSG_PRI_NORMAL) == OK)
    return ::msgQNumMsgs (msgq ());
  else
    return -1;
}

int
ACE_Message_Queue_Vx::enqueue_head_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::enqueue_head_i");

  // Just delegate to enqueue_tail_i.
  return enqueue_tail_i (new_item);
}

int
ACE_Message_Queue_Vx::enqueue_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::enqueue_i");

  if (new_item == 0)
    return -1;

  if (this->head_ == 0)
    // Should always take this branch.
    return this->enqueue_head_i (new_item);
  else
    ACE_NOTSUP_RETURN (-1);
}

// Actually get the first ACE_Message_Block (no locking, so must be
// called with locks held).  This method assumes that the queue has at
// least one item in it when it is called.

int
ACE_Message_Queue_Vx::dequeue_head_i (ACE_Message_Block *&first_item)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::dequeue_head_i");

  // We don't allocate a new Message_Block:  the caller must provide
  // it, and must ensure that it is big enough (without chaining).

  if (first_item == 0  ||  first_item->wr_ptr () == 0)
    return -1;

  if (::msgQReceive (msgq (),
                     first_item->wr_ptr (),
                     first_item->size (),
                     WAIT_FOREVER) == ERROR)
    return -1;
  else
    return ::msgQNumMsgs (msgq ());
}

// Take a look at the first item without removing it.

int
ACE_Message_Queue_Vx::wait_not_full_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                          ACE_Time_Value *tv)
{
  // Always return here, and let the VxWorks message queue handle blocking.
  return 0;
}

int
ACE_Message_Queue_Vx::wait_not_empty_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                           ACE_Time_Value *tv)
{
  // Always return here, and let the VxWorks message queue handle blocking.
  return 0;
}

#if ! defined (ACE_REQUIRES_FUNC_DEFINITIONS)
int
ACE_Message_Queue_Vx::peek_dequeue_head (ACE_Message_Block *&,
                                         ACE_Time_Value *tv)
{
  ACE_NOTSUP_RETURN (-1);
}
#endif /* ! ACE_REQUIRES_FUNC_DEFINITIONS */

#endif /* VXWORKS */

#endif /* ACE_MESSAGE_QUEUE_C */
