// $Id$

#ifndef TAO_ARRAY_VAROUT_T_CPP
#define TAO_ARRAY_VAROUT_T_CPP

#include "tao/Array_VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Array_VarOut_T.inl"
#endif /* __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename array_traits>
TAO_FixedArray_Var_T<array_traits> &
TAO_FixedArray_Var_T<array_traits>::operator= (slice_type * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      array_traits::free (this->ptr_);
      this->ptr_ = p;
    }

  return *this;
}

template<typename array_traits>
TAO_FixedArray_Var_T<array_traits> &
TAO_FixedArray_Var_T<array_traits>::operator= (
    const TAO_FixedArray_Var_T<array_traits> & p
  )
{
  if (this != &p)
    {
      array_traits>:free (this->ptr_);

      // Deep copy.
      this->ptr_ = array_traits::dup (p.in ());
    }

  return *this;
}

// *************************************************************

template<typename array_traits>
TAO_VarArray_Var_T<array_traits> &
TAO_VarArray_Var_T<array_traits>::operator= (slice_type * p)
{
  // Is what we own the same that is being assigned to us?
  if (this->ptr_ != p)
    {
      // Delete our stuff and assume ownership of p.
      array_traits::free (this->ptr_);
      this->ptr_ = p;
    }

  return *this;
}

template<typename array_traits>
TAO_VarArray_Var_T<array_traits> &
TAO_VarArray_Var_T<array_traits>::operator= (
    const TAO_VarArray_Var_T<array_traits> & p
  )
{
  if (this != &p)
    {
      array_traits::free (this->ptr_);

      // Deep copy.
      this->ptr_ = array_traits::dup (p.in ());
    }

  return *this;
}

// *************************************************************

template<typename T_array_traits>
void
TAO_Array_Forany_T<T_array_traits>::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  slice_type * tmp = static_cast<slice_type *> (_tao_void_pointer);
  TAO::details::array_traits<T_array_traits>::free (tmp);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ARRAY_VAROUT_T_CPP */
