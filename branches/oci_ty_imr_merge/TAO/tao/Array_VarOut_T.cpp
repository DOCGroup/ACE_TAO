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

template<typename T, typename T_slice, typename TAG>
TAO_FixedArray_Var_T<T,T_slice,TAG> &
TAO_FixedArray_Var_T<T,T_slice,TAG>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);
      this->ptr_ = p;
    }

  return *this;
}

template<typename T, typename T_slice, typename TAG>
TAO_FixedArray_Var_T<T,T_slice,TAG> &
TAO_FixedArray_Var_T<T,T_slice,TAG>::operator= (
    const TAO_FixedArray_Var_T<T,T_slice,TAG> & p
  )
{
  if (this != &p)
    {
      TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);

      // Deep copy.
      this->ptr_ = TAO::Array_Traits<T,T_slice,TAG>::dup (p.in ());
    }

  return *this;
}

// *************************************************************

template<typename T, typename T_slice, typename TAG>
TAO_VarArray_Var_T<T,T_slice,TAG> &
TAO_VarArray_Var_T<T,T_slice,TAG>::operator= (T_slice * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);
      this->ptr_ = p;
    }

  return *this;
}

template<typename T, typename T_slice, typename TAG>
TAO_VarArray_Var_T<T,T_slice,TAG> &
TAO_VarArray_Var_T<T,T_slice,TAG>::operator= (
    const TAO_VarArray_Var_T<T,T_slice,TAG> & p
  )
{
  if (this != &p)
    {
      TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);

      // Deep copy.
      this->ptr_ = TAO::Array_Traits<T,T_slice,TAG>::dup (p.in ());
    }

  return *this;
}

// *************************************************************

template<typename T, typename T_slice, typename TAG>
void
TAO_Array_Forany_T<T,T_slice,TAG>::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  T_slice * tmp = static_cast<T_slice *> (_tao_void_pointer);
  TAO::Array_Traits<T,T_slice,TAG>::free (tmp);
}

#endif /* TAO_ARRAY_VAROUT_T_C */
