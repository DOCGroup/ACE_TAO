// $Id$

#include "ace/Message_Queue.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (ace,
           Message_Queue,
           "$Id$")


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Message_Queue_Base::~ACE_Message_Queue_Base (void)
{
}

int
ACE_Message_Queue_Base::state (void)
{
  ACE_TRACE ("ACE_Message_Queue_Base::state");

  return this->state_;
}

#if defined (ACE_VXWORKS)

////////////////////////////////
// class ACE_Message_Queue_Vx //
////////////////////////////////

void
ACE_Message_Queue_Vx::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Message_Queue_Vx::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  switch (this->state_)
    {
    case ACE_Message_Queue_Base::ACTIVATED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = ACTIVATED\n")));
      break;
    case ACE_Message_Queue_Base::DEACTIVATED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = DEACTIVATED\n")));
      break;
    case ACE_Message_Queue_Base::PULSED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = PULSED\n")));
      break;
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("low_water_mark = %d\n")
              ACE_LIB_TEXT ("high_water_mark = %d\n")
              ACE_LIB_TEXT ("cur_bytes = %d\n")
              ACE_LIB_TEXT ("cur_length = %d\n")
              ACE_LIB_TEXT ("cur_count = %d\n")
              ACE_LIB_TEXT ("head_ = %u\n")
              ACE_LIB_TEXT ("MSG_Q_ID = %u\n"),
              this->low_water_mark_,
              this->high_water_mark_,
              this->cur_bytes_,
              this->cur_length_,
              this->cur_count_,
              this->head_,
              this->tail_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Message_Queue_Vx::ACE_Message_Queue_Vx (size_t max_messages,
                                            size_t max_message_length,
                                            ACE_Notification_Strategy *ns)
  : ACE_Message_Queue<ACE_NULL_SYNCH> (0, 0, ns),
    max_messages_ (static_cast<int> (max_messages)),
    max_message_length_ (static_cast<int> (max_message_length))
{
  ACE_TRACE ("ACE_Message_Queue_Vx::ACE_Message_Queue_Vx");

  if (this->open (max_messages_, max_message_length_, ns) == -1)
    ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("open")));
}

ACE_Message_Queue_Vx::~ACE_Message_Queue_Vx (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::~ACE_Message_Queue_Vx");

  if (this->tail_ != 0  &&  this->close () == -1)
    ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("close")));
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
  this->cur_bytes_ = 0;
  this->cur_length_ = 0;
  this->cur_count_ = 0;
  this->head_ = 0;
  this->notification_strategy_ = ns;
  this->max_messages_ = static_cast<int> (max_messages);
  this->max_message_length_ = static_cast<int> (max_message_length);

  if (tail_)
    {
      // Had already created a msgQ, so delete it.
      close ();
      activate_i ();
    }

  return (this->tail_ =
          reinterpret_cast<ACE_Message_Block *> (
              ::msgQCreate (max_messages_,
                            max_message_length_,
                            MSG_Q_FIFO))) == 0 ? -1 : 0;
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
ACE_Message_Queue_Vx::is_empty_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::is_empty_i");
  return ::msgQNumMsgs (msgq ()) == 0;
}

int
ACE_Message_Queue_Vx::is_full_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::is_full_i");
  return ::msgQNumMsgs (msgq ()) >= max_messages_;
}

size_t
ACE_Message_Queue_Vx::high_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::high_water_mark");
  ACE_NOTSUP_RETURN ((size_t) -1);
}

void
ACE_Message_Queue_Vx::high_water_mark (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::high_water_mark");
  ACE_NOTSUP;
}

size_t
ACE_Message_Queue_Vx::low_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::low_water_mark");
  // Don't need to guard, because this is fixed.

  ACE_NOTSUP_RETURN ((size_t) -1);
}

void
ACE_Message_Queue_Vx::low_water_mark (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::low_water_mark");
  ACE_NOTSUP;
}

size_t
ACE_Message_Queue_Vx::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_bytes");
  ACE_NOTSUP_RETURN ((size_t) -1);
}

size_t
ACE_Message_Queue_Vx::message_length (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_length");
  ACE_NOTSUP_RETURN ((size_t) -1);
}

size_t
ACE_Message_Queue_Vx::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_count");
  // Don't need to guard, because this is a system call.

  return ::msgQNumMsgs (msgq ());
}

void
ACE_Message_Queue_Vx::message_bytes (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_bytes");
  ACE_NOTSUP;
}

void
ACE_Message_Queue_Vx::message_length (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_length");
  ACE_NOTSUP;
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

  ++this->cur_count_;

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

int
ACE_Message_Queue_Vx::enqueue_deadline_i (ACE_Message_Block *new_item)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::enqueue_deadline_i");

  // Just delegate to enqueue_tail_i.
  return enqueue_tail_i (new_item);
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

int
ACE_Message_Queue_Vx::dequeue_prio_i (ACE_Message_Block *& /*dequeued*/)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::dequeue_prio_i");
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_Message_Queue_Vx::dequeue_tail_i (ACE_Message_Block *& /*dequeued*/)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::dequeue_tail_i");
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_Message_Queue_Vx::dequeue_deadline_i (ACE_Message_Block *& /*dequeued*/)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::dequeue_deadline_i");
  ACE_NOTSUP_RETURN (-1);
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
                                         ACE_Time_Value *)
{
  ACE_NOTSUP_RETURN (-1);
}
#endif /* ! ACE_NEEDS_FUNC_DEFINITIONS */

#endif /* ACE_VXWORKS */

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

ACE_Message_Queue_NT::ACE_Message_Queue_NT (DWORD max_threads)
  : max_cthrs_ (max_threads),
    cur_thrs_ (0),
    cur_bytes_ (0),
    cur_length_ (0),
    cur_count_ (0),
    completion_port_ (ACE_INVALID_HANDLE)
{
  ACE_TRACE ("ACE_Message_Queue_NT::ACE_Message_Queue_NT");
  this->open (max_threads);
}

int
ACE_Message_Queue_NT::open (DWORD max_threads)
{
  ACE_TRACE ("ACE_Message_Queue_NT::open");
  this->max_cthrs_ = max_threads;
  this->completion_port_ = ::CreateIoCompletionPort (ACE_INVALID_HANDLE,
                                                     0,
                                                     ACE_Message_Queue_Base::ACTIVATED,
                                                     max_threads);
  return (this->completion_port_ == 0 ? -1 : 0);
}

int
ACE_Message_Queue_NT::close (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::close");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
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
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
  if (this->state_ != ACE_Message_Queue_Base::DEACTIVATED)
    {
      size_t const msize = new_item->total_size ();
      size_t const mlength = new_item->total_length ();
      // Note - we send ACTIVATED in the 3rd arg to tell the completion
      // routine it's _NOT_ being woken up because of deactivate().
      ULONG_PTR state_to_post;
      state_to_post = ACE_Message_Queue_Base::ACTIVATED;
      if (::PostQueuedCompletionStatus (this->completion_port_,
                                        static_cast<DWORD> (msize),
                                        state_to_post,
                                        reinterpret_cast<LPOVERLAPPED> (new_item)))
        {
          // Update the states once I succeed.
          this->cur_bytes_ += msize;
          this->cur_length_ += mlength;
          return ACE_Utils::Truncate<int> (++this->cur_count_);
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
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

    // Make sure the MQ is not deactivated before proceeding.
    if (this->state_ == ACE_Message_Queue_Base::DEACTIVATED)
      {
        errno = ESHUTDOWN;      // Operation on deactivated MQ not allowed.
        return -1;
      }
    else
      ++this->cur_thrs_;        // Increase the waiting thread count.
  }

  ULONG_PTR queue_state;
  DWORD msize;
  // Get a message from the completion port.
  int retv = ::GetQueuedCompletionStatus (this->completion_port_,
                                          &msize,
                                          &queue_state,
                                          reinterpret_cast<LPOVERLAPPED *> (&first_item),
                                          (timeout == 0 ? INFINITE : timeout->msec ()));
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
    --this->cur_thrs_;          // Decrease waiting thread count.
    if (retv)
      {
        if (queue_state == ACE_Message_Queue_Base::ACTIVATED)
          {                     // Really get a valid MB from the queue.
            --this->cur_count_;
            this->cur_bytes_ -= msize;
            this->cur_length_ -= first_item->total_length ();
            return ACE_Utils::Truncate<int> (this->cur_count_);
          }
        else                    // Woken up by deactivate () or pulse ().
            errno = ESHUTDOWN;
      }
  }
  return -1;
}

int
ACE_Message_Queue_NT::deactivate (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::deactivate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  int const previous_state = this->state_;
  if (previous_state != ACE_Message_Queue_Base::DEACTIVATED)
    {
      this->state_ = ACE_Message_Queue_Base::DEACTIVATED;

      // Get the number of shutdown messages necessary to wake up all
      // waiting threads.
      DWORD cntr =
        this->cur_thrs_ - static_cast<DWORD> (this->cur_count_);
      while (cntr-- > 0)
        ::PostQueuedCompletionStatus (this->completion_port_,
                                      0,
                                      this->state_,
                                      0);
    }
  return previous_state;
}

int
ACE_Message_Queue_NT::activate (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::activate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
  int const previous_status = this->state_;
  this->state_ = ACE_Message_Queue_Base::ACTIVATED;
  return previous_status;
}

int
ACE_Message_Queue_NT::pulse (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::pulse");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  int const previous_state = this->state_;
  if (previous_state != ACE_Message_Queue_Base::DEACTIVATED)
    {
      this->state_ = ACE_Message_Queue_Base::PULSED;

      // Get the number of shutdown messages necessary to wake up all
      // waiting threads.

      DWORD cntr =
        this->cur_thrs_ - static_cast<DWORD> (this->cur_count_);
      while (cntr-- > 0)
        ::PostQueuedCompletionStatus (this->completion_port_,
                                      0,
                                      this->state_,
                                      0);
    }
  return previous_state;
}

void
ACE_Message_Queue_NT::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Message_Queue_NT::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  switch (this->state_)
    {
    case ACE_Message_Queue_Base::ACTIVATED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = ACTIVATED\n")));
      break;
    case ACE_Message_Queue_Base::DEACTIVATED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = DEACTIVATED\n")));
      break;
    case ACE_Message_Queue_Base::PULSED:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("state = PULSED\n")));
      break;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("max_cthrs_ = %d\n")
              ACE_LIB_TEXT ("cur_thrs_ = %d\n")
              ACE_LIB_TEXT ("cur_bytes = %d\n")
              ACE_LIB_TEXT ("cur_length = %d\n")
              ACE_LIB_TEXT ("cur_count = %d\n")
              ACE_LIB_TEXT ("completion_port_ = %x\n"),
              this->max_cthrs_,
              this->cur_thrs_,
              this->cur_bytes_,
              this->cur_length_,
              this->cur_count_,
              this->completion_port_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO */

ACE_END_VERSIONED_NAMESPACE_DECL
