// $Id$

#ifndef TAO_EC_BUSY_LOCK_CPP
#define TAO_EC_BUSY_LOCK_CPP

#include "EC_Busy_Lock.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Busy_Lock.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Busy_Lock, "$Id$")

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::remove (void)
{
  return 0;
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::acquire (void)
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::tryacquire (void)
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::release (void)
{
  return this->adaptee_->idle ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::acquire_read (void)
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::acquire_write (void)
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::tryacquire_read (void)
{
  return this->adaptee_->busy ();
}

template<class T>
int TAO_EC_Busy_Lock_Adapter<T>::tryacquire_write (void)
{
  return this->adaptee_->busy ();
}

#endif /* TAO_EC_BUSY_LOCK_CPP */
