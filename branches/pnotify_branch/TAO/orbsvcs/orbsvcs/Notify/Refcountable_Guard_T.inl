// $Id$

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::TAO_Notify_Refcountable_Guard_T (T *t)
  : t_ (t)
{
  if (this->t_ != 0)
    {
      this->t_->_incr_refcnt ();
    }
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::~TAO_Notify_Refcountable_Guard_T ()
{
  if (this->t_ != 0)
    {
      this->t_->_decr_refcnt ();
    }
}

template <class T> ACE_INLINE T*
TAO_Notify_Refcountable_Guard_T<T>::get (void) const
{
  return this->t_;
}

template <class T> ACE_INLINE T*
TAO_Notify_Refcountable_Guard_T<T>::operator-> (void) const
{
  ACE_ASSERT (this->t_ != 0);
  return this->t_;
}

template <class T> ACE_INLINE T&
TAO_Notify_Refcountable_Guard_T<T>::operator *(void) const
{
  ACE_ASSERT (this->t_ != 0);
  return *this->t_;
}
