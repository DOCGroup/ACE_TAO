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

template<typename T, typename T_slice>
TAO_FixedArray_Var_T<T,T_slice> &
TAO_FixedArray_Var_T<T,T_slice>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      TAO::Array_Traits<T,T_slice>::tao_free (this->ptr_);
      this->ptr_ = p;
    }
  
  return *this;
}

template<typename T, typename T_slice>
TAO_FixedArray_Var_T<T,T_slice> &
TAO_FixedArray_Var_T<T,T_slice>::operator= (
    const TAO_FixedArray_Var_T<T,T_slice> & p
  )
{
  if (this != &p)
    {
      TAO::Array_Traits<T,T_slice>::tao_free (this->ptr_);
      
      // Deep copy.
      this->ptr_ = TAO::Array_Traits<T,T_slice>::tao_dup (p.in ());
    }
  
  return *this;
}

// *************************************************************

template<typename T, typename T_slice>
TAO_VarArray_Var_T<T,T_slice> &
TAO_VarArray_Var_T<T,T_slice>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      TAO::Array_Traits<T,T_slice>::tao_free (this->ptr_);
      this->ptr_ = p;
    }
  
  return *this;
}

template<typename T, typename T_slice>
TAO_VarArray_Var_T<T,T_slice> &
TAO_VarArray_Var_T<T,T_slice>::operator= (
    const TAO_VarArray_Var_T<T,T_slice> & p
  )
{
  if (this != &p)
    {
      TAO::Array_Traits<T,T_slice>::tao_free (this->ptr_);
      
      // Deep copy.
      this->ptr_ = TAO::Array_Traits<T,T_slice>::tao_dup (p.in ());
    }
  
  return *this;
}

// *************************************************************

template<typename T, typename T_slice>
void
TAO_Array_Forany_T<T,T_slice>::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  T_slice * tmp = ACE_static_cast (T_slice *, _tao_void_pointer);
  TAO::Array_Traits<T,T_slice>::tao_free (tmp);
}

#endif /* TAO_ARRAY_VAROUT_T_C */
