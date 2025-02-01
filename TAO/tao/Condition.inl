// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::wait ()
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

/// Peform an "alertable" timed wait.  If the argument ABSTIME == 0
/// then we do a regular cond_wait(), else we do a timed wait for up to
/// @a abstime
template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::wait (const ACE_Time_Value *abstime)
{
  return this->wait (*this->mutex_, abstime);
}

template<class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::remove ()
{
  return this->cond_->remove ();
}

template<class MUTEX> ACE_INLINE MUTEX *
TAO_Condition<MUTEX>::mutex ()
{
  return this->mutex_;
}

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::signal ()
{
  return this->cond_->signal ();
}

template <class MUTEX> ACE_INLINE int
TAO_Condition<MUTEX>::broadcast ()
{
  return this->cond_->broadcast ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
