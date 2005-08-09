// $Id$

#ifndef TAO_FIXED_ARRAY_ARGUMENT_T_C
#define TAO_FIXED_ARRAY_ARGUMENT_T_C

#include "tao/Fixed_Array_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Fixed_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Fixed_Array_Argument_T,
           "$Id$")

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
CORBA::Boolean
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
void
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
void
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
CORBA::Boolean
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
void
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  typedef TAO::Array_Traits<S,S_slice,S_tag> ARRAY_TRAITS;
  S_slice * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::alloc (),
                        0);
  this->x_ = tmp_ptr;
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
void
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, S_forany (this->x_.ptr ()));
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_C */
