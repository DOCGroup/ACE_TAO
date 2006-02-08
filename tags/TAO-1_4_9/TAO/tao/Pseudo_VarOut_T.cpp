// $Id$

#ifndef TAO_PSEUDO_VAROUT_T_CPP
#define TAO_PSEUDO_VAROUT_T_CPP

#include "tao/Pseudo_VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Pseudo_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
TAO_Pseudo_Var_T<T> &
TAO_Pseudo_Var_T<T>::operator= (const TAO_Pseudo_Var_T<T> & p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = T::_duplicate (p.ptr ());
    }

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_PSEUDO_VAROUT_T_CPP */
