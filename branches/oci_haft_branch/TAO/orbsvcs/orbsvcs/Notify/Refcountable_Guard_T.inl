// $Id$

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::TAO_Notify_Refcountable_Guard_T (T *t)
  : t_ (t)
{
  this->t_->_incr_refcnt ();
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::~TAO_Notify_Refcountable_Guard_T ()
{
  this->t_->_decr_refcnt ();
}

template <class T> ACE_INLINE T*
TAO_Notify_Refcountable_Guard_T<T>::operator-> (void) const
{
  return this->t_;
}

template <class T> ACE_INLINE T&
TAO_Notify_Refcountable_Guard_T<T>::operator *(void) const
{
  return *this->t_;
}
