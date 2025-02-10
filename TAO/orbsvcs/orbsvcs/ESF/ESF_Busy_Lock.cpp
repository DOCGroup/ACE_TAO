#ifndef TAO_ESF_BUSY_LOCK_CPP
#define TAO_ESF_BUSY_LOCK_CPP

#include "orbsvcs/ESF/ESF_Busy_Lock.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Busy_Lock.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::remove ()
{
  return 0;
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::acquire ()
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::tryacquire ()
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::release ()
{
  return this->adaptee_->idle ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::acquire_read ()
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::acquire_write ()
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::tryacquire_read ()
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_ESF_Busy_Lock_Adapter<T>::tryacquire_write ()
{
  return this->adaptee_->busy ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_BUSY_LOCK_CPP */
