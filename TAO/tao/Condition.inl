/* -*- C++ -*- */
//$Id$
template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::wait (void)
{
  return this->cond_->wait ();
}

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::wait (MUTEX &mutex,
                            const ACE_Time_Value *abstime)
{
  return this->cond_->wait (mutex,
                           abstime);
}

// Peform an "alertable" timed wait.  If the argument ABSTIME == 0
// then we do a regular cond_wait(), else we do a timed wait for up to
// ABSTIME using the Solaris cond_timedwait() function.

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::wait (const ACE_Time_Value *abstime)
{
  return this->wait (this->mutex_, abstime);
}

template<class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::remove (void)
{
  return this->cond_->remove ();
}

template<class MUTEX> ACE_INLINE MUTEX *
TAO_Condition<MUTEX>::mutex (void)
{
  return this->mutex_;
}

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::signal (void)
{
  return this->cond_->signal ();
}

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::broadcast (void)
{
  return this->cond_->broadcast ();
}
