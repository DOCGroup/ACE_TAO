// $Id$
#ifndef TAO_OBJREF_VAROUT_T_CPP
#define TAO_OBJREF_VAROUT_T_CPP

#include "tao/Objref_VarOut_T.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
#include "tao/Objref_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

template <typename T>
TAO_Objref_Var_T<T>::TAO_Objref_Var_T (
    const TAO_Objref_Var_T<T> & p
  )
  : TAO_Base_var (),
    ptr_ (TAO::Objref_Traits<T>::duplicate (p.ptr ()))
{
}

template <typename T>
TAO_Objref_Var_T<T> &
TAO_Objref_Var_T<T>::operator= (T * p)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
TAO_Objref_Var_T<T> &
TAO_Objref_Var_T<T>::operator= (
    const TAO_Objref_Var_T<T> & p
  )
{
  if (this != &p)
    {
      TAO::Objref_Traits<T>::release (this->ptr_);
      this->ptr_ = TAO::Objref_Traits<T>::duplicate (p.ptr ());
    }

  return *this;
}

#endif /* TAO_OBJREF_VAROUT_T_CPP */
