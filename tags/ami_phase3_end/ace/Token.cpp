// $Id$

#define ACE_BUILD_DLL
#include "ace/Thread.h"
#include "ace/Token.h"

#if defined (DEBUGGING)
#include "ace/streams.h"
#endif /* DEBUGGING */

ACE_RCSID(ace, Token, "$Id$")

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Synch_T.h"
#include "ace/Token.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Token)


void
ACE_Token::dump (void) const
{
  ACE_TRACE ("ACE_Token::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nthread = %d"), ACE_Thread::self ()));
  // @@ Is there a portable way to do this?
  // ACE_DEBUG ((LM_DEBUG, "\nowner_ = %l", (long) this->owner_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nowner_ addr = %x"), &this->owner_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nwaiters_ = %d"), this->waiters_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nin_use_ = %d"), this->in_use_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nnesting level = %d"), this->nesting_level_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Token::ACE_Token_Queue_Entry::ACE_Token_Queue_Entry (ACE_Thread_Mutex &m,
                                                         ACE_thread_t t_id)
  : next_ (0),
    thread_id_ (t_id),
#if defined (ACE_TOKEN_USES_SEMAPHORE)
    cv_ (0),
#else
    cv_ (m),
#endif /* ACE_TOKEN_USES_SEMAPHORE */
    runable_ (0)
{
#if defined (ACE_TOKEN_USES_SEMAPHORE)
  ACE_UNUSED_ARG (m);
#endif /* ACE_TOKEN_USES_SEMAPHORE */

  ACE_TRACE ("ACE_Token::ACE_Token_Queue_Entry::ACE_Token_Queue_Entry");
}

ACE_Token::ACE_Token_Queue::ACE_Token_Queue (void)
  : head_ (0),
    tail_ (0)
{
  ACE_TRACE ("ACE_Token::ACE_Token_Queue::ACE_Token_Queue");
}

// Remove an entry from the list.  Must be
// called with locks held.

void
ACE_Token::ACE_Token_Queue::remove_entry (ACE_Token::ACE_Token_Queue_Entry *entry)
{
  ACE_TRACE ("ACE_Token::remove_entry");
  ACE_Token_Queue_Entry *curr = 0;
  ACE_Token_Queue_Entry *prev = 0;

  if (this->head_ == 0)
    return;

  for (curr = this->head_;
       curr != 0 && curr != entry;
       curr = curr->next_)
    prev = curr;

  if (curr == 0) // Didn't find the entry...
    return;
  else if (prev == 0) // Delete at the head.
    this->head_ = this->head_->next_;
  else // Delete in the middle.
    prev->next_ = curr->next_;

  // We need to update the tail of the list
  // if we've deleted the last entry.

  if (curr->next_ == 0)
    this->tail_ = curr;
}

ACE_Token::ACE_Token (LPCTSTR name, void *any)
  : lock_ (name, any),
    in_use_ (0),
    waiters_ (0),
    nesting_level_ (0),
    signal_all_threads_ (0)
{
//  ACE_TRACE ("ACE_Token::ACE_Token");
}

ACE_Token::~ACE_Token (void)
{
  ACE_TRACE ("ACE_Token::~ACE_Token");
}

int
ACE_Token::shared_acquire (void (*sleep_hook_func)(void *),
                           void *arg,
                           ACE_Time_Value *timeout,
                           ACE_Token_Op_Type op_type)
{
  ACE_TRACE ("ACE_Token::shared_acquire");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  ACE_thread_t thr_id = ACE_Thread::self ();

  ACE_Token_Queue *queue = (op_type == ACE_Token::READ_TOKEN ?
                            &this->readers_ :
                            &this->writers_);

#if defined (DEBUGGING)
  cerr << '(' << ACE_Thread::self () << ')'
       << " acquire: owner_ = " << this->owner_
       << ", owner_ addr = " << &this->owner_
       << ", waiters_ = " << this->waiters_
       << ", in_use_ = " << this->in_use_
       << ", nesting level = " << this->nesting_level_ << endl;
#endif /* DEBUGGING */

  if (this->in_use_) // Someone already holds the token.
    {
      if (ACE_OS::thr_equal (thr_id, this->owner_)) // I own it!
        {
          this->nesting_level_++;
          return 0;
        }
      // Do a quick check for "polling" behavior.
      else if (timeout != 0 && timeout->sec () == 0 && timeout->usec () == 0)
        {
          errno = ETIME;
          return -1;
        }
      else // We've got to sleep until we get the token.
        {
          // Allocate q entry on stack.  This works since we don't
          // exit this method's activation record until we've got the
          // token.
          ACE_Token::ACE_Token_Queue_Entry my_entry (this->lock_, thr_id);
          int ret = 0;

          if (queue->head_ == 0) // I'm first and only waiter in line...
            {
              queue->head_ = &my_entry;
              queue->tail_ = &my_entry;
            }
          else // I'm queued at the end of the list.
            {
              queue->tail_->next_ = &my_entry;
              queue->tail_ = &my_entry;
            }

          this->waiters_++;

          // Execute appropriate <sleep_hook> callback.
          // (@@ should these methods return a success/failure
          // status, and if so, what should we do with it?)

          if (sleep_hook_func)
            {
              (*sleep_hook_func) (arg);
              ret++;
            }
          else // Execute virtual method.
            {
              this->sleep_hook ();
              ret++;
            }

          // Sleep until we've got the token (ignore signals).

          while (my_entry.wait (timeout, this->lock_) == -1)
            {
              // Note, this should obey whatever thread-specific
              // interrupt policy is currently in place...
              if (errno == EINTR)
                continue;
#if defined (DEBUGGING)
              cerr << '(' << ACE_Thread::self () << ')'
                   << " acquire: "
                   << (errno == ETIME ? "timed out" : "error occurred")
                   << endl;
#endif /* DEBUGGING */
              // We come here if a timeout occurs or some serious
              // ACE_Condition object error.
              queue->remove_entry (&my_entry);
              return -1;
            }

          ACE_ASSERT (my_entry.runable_);
#if defined (DEBUGGING)
          cerr << '(' << ACE_Thread::self () << ')'
               << " acquire (UNBLOCKED)" << endl;
#endif /* DEBUGGING */
          if (this->signal_all_threads_ != 0)
            return 2;
          else
            return ret;
        }
    }
  else
    {
      this->in_use_ = op_type;
      this->owner_ = thr_id; // Its mine!
      return 0;
    }
}

// By default this is a no-op.

/* virtual */
void
ACE_Token::sleep_hook (void)
{
  ACE_TRACE ("ACE_Token::sleep_hook");
}

int
ACE_Token::acquire (ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Token::acquire");
  return this->shared_acquire (0, 0, timeout, ACE_Token::WRITE_TOKEN);
}

// Acquire the token, sleeping until it is obtained or until
// <timeout> expires.

int
ACE_Token::acquire (void (*sleep_hook_func)(void *),
                    void *arg,
                    ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Token::acquire");
  return this->shared_acquire (sleep_hook_func, arg, timeout, ACE_Token::WRITE_TOKEN);
}

// Try to renew the token.

int
ACE_Token::renew (int requeue_position, ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Token::renew");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

#if defined (DEBUGGING)
  this->dump ();
#endif /* DEBUGGING */
  // ACE_ASSERT (ACE_OS::thr_equal (ACE_Thread::self (), this->owner_));

  // Check to see if there are any waiters.  If not, we just keep the token.
  if (this->writers_.head_ != 0 ||
      (this->in_use_ == ACE_Token::READ_TOKEN && this->readers_.head_ != 0))
    {
      // First we determine which queue should we the original threads
      // back and then, from which queue to wake next thread.
      ACE_Token::ACE_Token_Queue *old_q = (this->in_use_ == ACE_Token::READ_TOKEN ?
                                           &this->readers_ : &this->writers_);
      ACE_Token::ACE_Token_Queue *queue;
      if (this->writers_.head_ != 0)
        {
          queue = &this->writers_;
          this->in_use_ = ACE_Token::WRITE_TOKEN;
        }
      else
        {
          queue = &this->readers_;
          this->in_use_ = ACE_Token::READ_TOKEN;
        }

      ACE_Token::ACE_Token_Queue_Entry my_entry (this->lock_, this->owner_);
      int save_nesting_level_ = this->nesting_level_;

      this->owner_ = queue->head_->thread_id_;
      this->nesting_level_ = 0;

      // Wake up next waiter and make it runable.
      queue->head_->runable_ = 1;
      queue->head_->signal ();

      // And remove it from it's queue.
      queue->remove_entry (queue->head_);

      // Now put the current thread back in its queue.
      if (old_q->head_ == 0) // No other threads - just add me
        {
          old_q->head_ = &my_entry;
          old_q->tail_ = &my_entry;
        }
      else if (requeue_position == -1) // Insert at the end of the queue.
        {
          old_q->tail_->next_ = &my_entry;
          old_q->tail_ = &my_entry;
        }
      else if (requeue_position == 0) // Insert at head of queue.
        {
          my_entry.next_ = old_q->head_;
          old_q->head_ = &my_entry;
        }
      else // Insert in the middle of the queue somewhere.
        {
          ACE_Token::ACE_Token_Queue_Entry *insert_after = old_q->head_;

          // Determine where our thread should go in the queue of
          // waiters.

          while (requeue_position-- && insert_after->next_ != 0)
            insert_after = insert_after->next_;

          my_entry.next_ = insert_after->next_;

          if (my_entry.next_ == 0)
            old_q->tail_ = &my_entry;

          insert_after->next_ = &my_entry;
        }

      // Sleep until we've got the token (ignore signals).
      while (my_entry.wait (timeout, this->lock_) == -1)
        {
          // Note, this should obey whatever thread-specific
          // interrupt policy is currently in place...
          if (errno == EINTR)
            continue;
#if defined (DEBUGGING)
          cerr << '(' << ACE_Thread::self () << ')'
               << " renew: "
               << (errno == ETIME ? "timed out" : "error occurred") << endl;
#endif /* DEBUGGING */
          // We come here if a timeout occurs or
          // some serious ACE_Condition object error.
          old_q->remove_entry (&my_entry);
          return -1;
        }

      ACE_ASSERT (my_entry.runable_);
      this->nesting_level_ = save_nesting_level_;
      this->owner_ = my_entry.thread_id_;
    }
  return 0;
}

// Release the current holder of the token (which had
// better be the caller's thread!).

int
ACE_Token::release (void)
{
  ACE_TRACE ("ACE_Token::release");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  // ACE_ASSERT (ACE_OS::thr_equal (ACE_Thread::self (), this->owner_));

#if defined (DEBUGGING)
  this->dump ();
#endif /* DEBUGGING */

  if (this->nesting_level_ > 0)
    --this->nesting_level_;
  else
    {
      if (this->writers_.head_ == 0 && this->readers_.head_ == 0)
        this->signal_all_threads_ = this->in_use_ = 0; // No more waiters...
      else
        {
          ACE_Token_Queue *queue;

          if (this->writers_.head_ != 0)
            // Writer threads get priority to run first.
            {
              queue = &this->writers_;
              this->in_use_ = ACE_Token::WRITE_TOKEN;
            }
          else
            {
              queue = &this->readers_;
              this->in_use_ = ACE_Token::READ_TOKEN;
            }

          this->owner_ = queue->head_->thread_id_;
          --this->waiters_;

          // Wake up waiter and make it runable.
          queue->head_->runable_ = 1;
          queue->head_->signal ();

          queue->head_ = queue->head_->next_;

          if (queue->head_ == 0)
            queue->tail_ = 0;
        }
    }
  return 0;
}

int
ACE_Token::signal_all_threads (void)
{
  ACE_TRACE ("ACE_Token::release");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  if (this->waiters_ != 0)
      this->signal_all_threads_ = 1;
  return this->waiters_;
}

#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
