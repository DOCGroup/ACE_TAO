// $Id$

#ifndef TAO_PSEUDO_VAROUT_T_C
#define TAO_PSEUDO_VAROUT_T_C

#include "tao/Pseudo_VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Pseudo_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Pseudo_VarOut_T,
           "$Id$")

template <typename T>
ACE_INLINE
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
#endif /* TAO_PSEUDO_VAROUT_T_C */
