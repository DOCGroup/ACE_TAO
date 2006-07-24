// $Id$

#ifndef TAO_VAR_ARRAY_ARGUMENT_T_CPP
#define TAO_VAR_ARRAY_ARGUMENT_T_CPP

#include "tao/Var_Array_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Var_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         class Insert_Policy>
CORBA::Boolean
TAO::In_Var_Array_Argument_T<S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         class Insert_Policy>
void
TAO::In_Var_Array_Argument_T<S_forany,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_forany,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Array_Argument_T<S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

template<typename S_forany,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Array_Argument_T<S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         class Insert_Policy>
void
TAO::Inout_Var_Array_Argument_T<S_forany,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_out,
         typename S_forany,
         class Insert_Policy>
CORBA::Boolean
TAO::Out_Var_Array_Argument_T<S_out,S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  typedef TAO::Array_Traits<S_forany> ARRAY_TRAITS;
  ACE_ALLOCATOR_RETURN (this->x_,
                        ARRAY_TRAITS::alloc (),
                        0);
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_out,
         typename S_forany,
         class Insert_Policy>
void
TAO::Out_Var_Array_Argument_T<S_out,S_forany,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  S_forany tmp (this->x_);
  Insert_Policy::any_insert (any, tmp);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
CORBA::Boolean
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  typedef TAO::Array_Traits<S_forany> ARRAY_TRAITS;
  typename S_forany::_slice_type * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::alloc (),
                        0);
  this->x_ = tmp_ptr;
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
void
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, S_forany (this->x_.ptr ()));
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_VAR_ARRAY_ARGUMENT_T_CPP */
