// $Id$

#include "ace/Log_Msg.h"

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
TAO_Notify_Refcountable_Guard_T<T>::TAO_Notify_Refcountable_Guard_T (const TAO_Notify_Refcountable_Guard_T<T> &rhs)
  : t_ (rhs.t_)
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

template <class T> ACE_INLINE
T&
TAO_Notify_Refcountable_Guard_T<T>::operator *(void) const
{
  ACE_ASSERT (this->t_ != 0);
  return *this->t_;
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T> &
TAO_Notify_Refcountable_Guard_T<T>::operator = (
  const TAO_Notify_Refcountable_Guard_T<T> & rhs)
{
  // note exception safe assignment. see Sutter's "Exceptional C++"
  // note it's worth the following optimization to avoid threadsafe increment/decrement refcounters
  if (this->t_ != rhs.t_) {
    TAO_Notify_Refcountable_Guard_T<T> temp(rhs.t_);
    swap (temp);
}
  return *this;
}


template <class T> ACE_INLINE
void
TAO_Notify_Refcountable_Guard_T<T>::swap (TAO_Notify_Refcountable_Guard_T & rhs)
{
  T* pt = rhs.t_;
  rhs.t_ = this->t_;
  this->t_ = pt;
}
