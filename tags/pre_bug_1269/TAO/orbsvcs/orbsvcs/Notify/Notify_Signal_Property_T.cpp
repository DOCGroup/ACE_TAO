// $Id$

#ifndef TAO_NOTIFY_SIGNAL_PROPERTY_T_C
#define TAO_NOTIFY_SIGNAL_PROPERTY_T_C

#include "Notify_Signal_Property_T.h"

ACE_RCSID (Notify, Notify_Signal_Property_T, "$Id$")

template <class ACE_LOCK, class TYPE>
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::TAO_Notify_Signal_Property (void)
  : value_ (),
    changed_ (this->value_.mutex ())
{
}

template <class ACE_LOCK, class TYPE>
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::TAO_Notify_Signal_Property (
                                                            const TYPE& c)
  : value_ (c),
    changed_ (this->value_.mutex ())
{
}

template <class ACE_LOCK, class TYPE>
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::~TAO_Notify_Signal_Property (void)
{
}

template <class ACE_LOCK, class TYPE> TYPE
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::operator++ (int)
{
  // Wake up the condition waiters
  this->changed_.broadcast ();
  return this->value_++;
}

template <class ACE_LOCK, class TYPE> TYPE
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::operator-- (int)
{
  // Wake up the condition waiters
  this->changed_.broadcast ();
  return this->value_--;
}

template <class ACE_LOCK, class TYPE> TYPE
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::value (void) const
{
  return this->value_.value ();
}

template <class ACE_LOCK, class TYPE> int
TAO_Notify_Signal_Property<ACE_LOCK, TYPE>::wait_for_change (
                                        const ACE_Time_Value* abstime)
{
  // Wait for the signal that the value has changed.
  int status = this->changed_.wait (abstime);

  // Unlock the mutex since the call to wait () always
  // returns the mutex to us in a locked state.
  this->value_.mutex ().release ();

  return status;
}

#endif /* TAO_NOTIFY_SIGNAL_PROPERTY_T_C */
