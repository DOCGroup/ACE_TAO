// $Id$

#ifndef TAO_ARRAY_VAROUT_T_C
#define TAO_ARRAY_VAROUT_T_C

#include "tao/Array_VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Array_VarOut_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           Array_VarOut_T, 
           "$Id$")

template<typename T_slice, typename T_life>
TAO_FixedArray_Var_T<T_slice,T_life> &
TAO_FixedArray_Var_T<T_slice,T_life>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      T_life::tao_free (this->ptr_);
      this->ptr_ = p;
    }
  
  return *this;
}

template<typename T_slice, typename T_life>
TAO_FixedArray_Var_T<T_slice,T_life> &
TAO_FixedArray_Var_T<T_slice,T_life>::operator= (
    const TAO_FixedArray_Var_T<T_slice,T_life> & p
  )
{
  if (this != &p)
    {
      T_life::tao_free (this->ptr_);
      
      // Deep copy.
      this->ptr_ = T_life::tao_dup (p.in ()));
    }
  
  return *this;
}

// *************************************************************

template<typename T_slice, typename T_life>
TAO_VarArray_Var_T<T_slice,T_life> &
TAO_VarArray_Var_T<T_slice,T_life>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      T_life::tao_free (this->ptr_);
      this->ptr_ = p;
    }
  
  return *this;
}

template<typename T_slice, typename T_life>
TAO_VarArray_Var_T<T_slice,T_life> &
TAO_VarArray_Var_T<T_slice,T_life>::operator= (
    const TAO_VarArray_Var_T<T_slice,T_life> & p
  )
{
  if (this != &p)
    {
      T_life::tao_free (this->ptr_);
      
      // Deep copy.
      this->ptr_ = T_life::tao_dup (p.in ()));
    }
  
  return *this;
}

// *************************************************************

template<typename T_slice, typename T_life>
void
TAO_Array_Forany_T<T_slice,T_life>::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  T_slice * tmp = ACE_static_cast (T_slice *, _tao_void_pointer);
  T_life::tao_free (tmp);
}

#endif /* TAO_ARRAY_VAROUT_T_C */
