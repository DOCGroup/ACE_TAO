/* -*- C++ -*- */
// $Id$

template<class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::remove (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::remove");

  // cond_destroy() is called in a loop if the condition variable is
  // BUSY.  This avoids a condition where a condition is signaled and
  // because of some timing problem, the thread that is to be signaled
  // has called the cond_wait routine after the signal call.  Since
  // the condition signal is not queued in any way, deadlock occurs.

  int result = 0;

#if defined (CHORUS)
  // Are we the owner?
  if (this->process_cond_ && this->condname_)
    {
      // Only destroy the condition if we're the ones who initialized
      // it.
      while ((result = ACE_OS::cond_destroy (this->process_cond_)) == -1
             && errno == EBUSY)
        {
          ACE_OS::cond_broadcast (this->process_cond_);
          ACE_OS::thr_yield ();
        }
      ACE_OS::munmap (this->process_cond_,
                      sizeof (ACE_cond_t));
      ACE_OS::shm_unlink (this->condname_);
      ACE_OS::free (ACE_static_cast (void *,
                                     ACE_const_cast (ACE_TCHAR *,
                                                     this->condname_)));
    }
  else if (this->process_cond_)
    {
      ACE_OS::munmap (this->process_cond_,
                      sizeof (ACE_cond_t));
      result = 0;
    }
  else
#endif /* CHORUS */

    while ((result = ACE_OS::cond_destroy (&this->cond_)) == -1
           && errno == EBUSY)
      {
        ACE_OS::cond_broadcast (&this->cond_);
        ACE_OS::thr_yield ();
      }

  return result;
}

template<class MUTEX> ACE_INLINE MUTEX &
ACE_Condition<MUTEX>::mutex (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::mutex");
  return this->mutex_;
}

template <class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::signal (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::signal");
#if defined (CHORUS)
  if (this->process_cond_ != 0)
    return ACE_OS::cond_signal (this->process_cond_);
#endif /* CHORUS */
  return ACE_OS::cond_signal (&this->cond_);
}

template <class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::broadcast (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::broadcast");
#if defined (CHORUS)
  if (this->process_cond_ != 0)
    return ACE_OS::cond_broadcast (this->process_cond_);
#endif /* CHORUS */
  return ACE_OS::cond_broadcast (&this->cond_);
}
