// $Id$

#include "ace/Barrier.h"

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Barrier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Barrier, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Sub_Barrier)

void
ACE_Sub_Barrier::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Sub_Barrier::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->barrier_finished_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("running_threads_ = %d"), this->running_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Sub_Barrier::ACE_Sub_Barrier (u_int count,
                                  ACE_Thread_Mutex &lock,
                                  const ACE_TCHAR *name,
                                  void *arg)
  : barrier_finished_ (lock, name, arg),
    running_threads_ (count)
{
// ACE_TRACE ("ACE_Sub_Barrier::ACE_Sub_Barrier");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Barrier)

void
ACE_Barrier::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Barrier::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("current_generation_ = %d"), this->current_generation_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\ncount_ = %d"), this->count_));
  this->sub_barrier_1_.dump ();
  this->sub_barrier_2_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Barrier::ACE_Barrier (u_int count,
                          const ACE_TCHAR *name,
                          void *arg)
  : lock_ (name, (ACE_mutexattr_t *) arg),
    current_generation_ (0),
    count_ (count),
    sub_barrier_1_ (count, lock_, name, arg),
    sub_barrier_2_ (count, lock_, name, arg)
{
// ACE_TRACE ("ACE_Barrier::ACE_Barrier");
  this->sub_barrier_[0] = &this->sub_barrier_1_;
  this->sub_barrier_[1] = &this->sub_barrier_2_;
}

int
ACE_Barrier::wait (void)
{
// ACE_TRACE ("ACE_Barrier::wait");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  ACE_Sub_Barrier *sbp =
    this->sub_barrier_[this->current_generation_];

  // Check for shutdown...
  if (sbp == 0)
    return -1;

  if (sbp->running_threads_ == 1)
    {
      // We're the last running thread, so swap generations and tell
      // all the threads waiting on the barrier to continue on their
      // way.

      sbp->running_threads_ = this->count_;
      // Swap generations.
      this->current_generation_ = 1 - this->current_generation_;
      sbp->barrier_finished_.broadcast ();
    }
  else
    {
      --sbp->running_threads_;

      // Block until all the other threads wait().
      while (sbp->running_threads_ != this->count_)
        sbp->barrier_finished_.wait ();
    }

  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Barrier)

ACE_Thread_Barrier::ACE_Thread_Barrier (u_int count, const ACE_TCHAR *name)
  : ACE_Barrier (count, name)
{
// ACE_TRACE ("ACE_Thread_Barrier::ACE_Thread_Barrier");
}

void
ACE_Thread_Barrier::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Barrier::dump");
  ACE_Barrier::dump ();
#endif /* ACE_HAS_DUMP */
}

#if 0
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Barrier)

ACE_Process_Barrier::ACE_Process_Barrier (u_int count, const ACE_TCHAR *name)
  : ACE_Barrier (count, USYNC_PROCESS, name)
{
// ACE_TRACE ("ACE_Process_Barrier::ACE_Process_Barrier");
}

void
ACE_Process_Barrier::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Process_Barrier::dump");
  ACE_Barrier::dump ();
#endif /* ACE_HAS_DUMP */
}
#endif /* 0 */

#endif /* ACE_HAS_THREADS */
