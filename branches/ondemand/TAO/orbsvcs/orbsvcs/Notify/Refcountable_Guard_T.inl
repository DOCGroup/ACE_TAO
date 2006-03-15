// -*- C++ -*-
//
// $Id$

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::TAO_Notify_Refcountable_Guard_T (T *t)
  : t_ (t)
{
  if ( this->t_ != static_cast< T* >( 0 ) ) this->t_->_incr_refcnt();
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::TAO_Notify_Refcountable_Guard_T (const TAO_Notify_Refcountable_Guard_T<T> &rhs)
  : t_ (rhs.t_)
{
  if ( this->t_ != static_cast< T* >( 0 ) ) this->t_->_incr_refcnt();
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T>::~TAO_Notify_Refcountable_Guard_T ()
{
  if ( this->t_ != static_cast< T* >( 0 ) ) this->t_->_decr_refcnt();
}

template <class T> ACE_INLINE
T*
TAO_Notify_Refcountable_Guard_T<T>::get (void) const
{
  return this->t_;
}

template <class T> ACE_INLINE
bool
TAO_Notify_Refcountable_Guard_T<T>::isSet (void) const
{
  return ( this->t_ != static_cast< T* >( 0 ) );
}


template <class T> ACE_INLINE
T*
TAO_Notify_Refcountable_Guard_T<T>::operator-> (void) const
{
  ACE_ASSERT ( this->t_ != static_cast< T* >( 0 ) );
  return this->t_;
}

template <class T> ACE_INLINE
T&
TAO_Notify_Refcountable_Guard_T<T>::operator* (void) const
{
  ACE_ASSERT ( this->t_ != static_cast< T* >( 0 ) );
  return *this->t_;
}

template <class T> ACE_INLINE
TAO_Notify_Refcountable_Guard_T<T> &
TAO_Notify_Refcountable_Guard_T<T>::operator = (
  const TAO_Notify_Refcountable_Guard_T<T> & rhs)
{
  reset( rhs.t_ );
  return *this;
}


template <class T> ACE_INLINE
void
TAO_Notify_Refcountable_Guard_T<T>::reset (T* t)
{
  if (this->t_ != t)
  {
    TAO_Notify_Refcountable_Guard_T<T> temp( t );
    swap( temp );
  }
}

template <class T> ACE_INLINE
void
TAO_Notify_Refcountable_Guard_T<T>::swap(
  TAO_Notify_Refcountable_Guard_T<T>& rhs )
{
  T* temp = this->t_;
  this->t_ = rhs.t_;
  rhs.t_ = temp;
}

TAO_END_VERSIONED_NAMESPACE_DECL
