// $Id$

#if !defined (ACE_MESSAGE_QUEUE_C)
#define ACE_MESSAGE_QUEUE_C

#define ACE_BUILD_DLL
#include "ace/Message_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Message_Queue, "$Id$")

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

  if (this->tail_ != 0  &&  this->close () == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("close")));
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
  ACE_UNUSED_ARG (mon);
  ACE_UNUSED_ARG (tv);

  return 0;
}

int
ACE_Message_Queue_Vx::wait_not_empty_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                           ACE_Time_Value *tv)
{
  // Always return here, and let the VxWorks message queue handle blocking.
  ACE_UNUSED_ARG (mon);
  ACE_UNUSED_ARG (tv);

  return 0;
}

#if ! defined (ACE_NEEDS_FUNC_DEFINITIONS)
int
ACE_Message_Queue_Vx::peek_dequeue_head (ACE_Message_Block *&,
                                         ACE_Time_Value *tv)
{
  ACE_UNUSED_ARG (tv);
  ACE_NOTSUP_RETURN (-1);
}
#endif /* ! ACE_NEEDS_FUNC_DEFINITIONS */

#endif /* VXWORKS */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)

ACE_Message_Queue_NT::ACE_Message_Queue_NT (size_t max_threads)
  : max_cthrs_ (max_threads),
    cur_thrs_ (0),
    cur_bytes_ (0),
    cur_count_ (0),
    deactivated_ (0),
    completion_port_ (ACE_INVALID_HANDLE)
{
  ACE_TRACE ("ACE_Message_Queue_NT::ACE_Message_Queue_NT");
  this->open (max_threads);
}

int
ACE_Message_Queue_NT::open (size_t max_threads)
{
  ACE_TRACE ("ACE_Message_Queue_NT::open");
  this->max_cthrs_ = max_threads;
  this->completion_port_ = ::CreateIoCompletionPort (ACE_INVALID_HANDLE,
                                                     NULL,
                                                     ACE_Message_Queue_Base::WAS_ACTIVE,
                                                     max_threads);
  return (this->completion_port_ == NULL ? -1 : 0);
}

int
ACE_Message_Queue_NT::close (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::close");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  this->deactivate ();
  return (::CloseHandle (this->completion_port_) ? 0 : -1 );
}

ACE_Message_Queue_NT::~ACE_Message_Queue_NT (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::~ACE_Message_Queue_NT");
  this->close ();
}

int
ACE_Message_Queue_NT::enqueue (ACE_Message_Block *new_item,
                               ACE_Time_Value *)
{
  ACE_TRACE ("ACE_Message_Queue_NT::enqueue");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  if (!this->deactivated_)
    {
      size_t msize = 0;
      for (ACE_Message_Block *temp = new_item;
           temp != 0;
           temp = temp->cont ())
        msize += temp->size ();
      if (::PostQueuedCompletionStatus (this->completion_port_,
                                        msize,
                                        this->deactivated_,
                                        ACE_reinterpret_cast (LPOVERLAPPED, new_item)))
        {
          // Update the states once I succeed.
          this->cur_bytes_ += msize;
          return ++this->cur_count_;
        }
    }
  else
    errno = ESHUTDOWN;

  // Fail to enqueue the message.
  return -1;
}

int
ACE_Message_Queue_NT::dequeue (ACE_Message_Block *&first_item,
                               ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue_NT::dequeue_head");

  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
    if (this->deactivated_)   // Make sure the MQ is not deactivated before
      {                         // I proceed.
        errno = ESHUTDOWN;      // Operation on deactivated MQ not allowed.
        return -1;
      }
    else
      ++this->cur_thrs_;        // Increase the waiting thread count.
  }

  DWORD shutdown;
  DWORD msize;
  // Get a message from the completion port.
  int retv = ::GetQueuedCompletionStatus (this->completion_port_,
                                          &msize,
                                          &shutdown,
                                          ACE_reinterpret_cast (LPOVERLAPPED *, &first_item),
                                          (timeout == 0 ? INFINITE : timeout->msec ()));
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
    --this->cur_thrs_;          // Decrease waiting thread count.
    if (retv)
      {
        if (!shutdown)
          {                     // Really get a valid MB from the queue.
            --this->cur_count_;
            this->cur_bytes_ -= msize;
            return this->cur_count_;
          }
        else                    // I am woken up by deactivate ().
            errno = ESHUTDOWN;
      }
  }
  return -1;
}

int
ACE_Message_Queue_NT::deactivate (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::deactivate");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  if (this->deactivated_)       // Check if I have been deactivated already.
    return ACE_Message_Queue_Base::WAS_INACTIVE;

  this->deactivated_ = 1;

  // Get the number of shutdown messages necessary to wake up
  // all waiting threads.

  for (size_t cntr = this->cur_thrs_ - this->cur_count_;
       cntr > 0; cntr++)
    ::PostQueuedCompletionStatus (this->completion_port_,
                                  0,
                                  this->deactivated_,
                                  NULL);
  return ACE_Message_Queue_Base::WAS_ACTIVE;
}

int
ACE_Message_Queue_NT::activate (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::activate");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  if (!this->deactivated_)
    return ACE_Message_Queue_Base::WAS_ACTIVE;

  this->deactivated_ = 0;
  return ACE_Message_Queue_Base::WAS_INACTIVE;
}

void
ACE_Message_Queue_NT::dump (void) const
{
  ACE_TRACE ("ACE_Message_Queue_NT::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("deactivated = %d\n")
              ASYS_TEXT ("max_cthrs_ = %d\n")
              ASYS_TEXT ("cur_thrs_ = %d\n")
              ASYS_TEXT ("cur_bytes = %d\n")
              ASYS_TEXT ("cur_count = %d\n")
              ASYS_TEXT ("completion_port_ = %x\n"),
              this->deactivated_,
              this->max_cthrs_,
              this->cur_thrs_,
              this->cur_bytes_,
              this->cur_count_,
              this->completion_port_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */

#endif /* ACE_MESSAGE_QUEUE_C */
