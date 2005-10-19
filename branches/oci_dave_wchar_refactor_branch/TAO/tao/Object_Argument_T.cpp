// $Id$

#ifndef TAO_OBJECT_ARGUMENT_T_C
#define TAO_OBJECT_ARGUMENT_T_C

#include "tao/Object_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Object_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Object_Argument_T,
           "$Id$")

template<typename S_ptr, typename Insert_Policy>
CORBA::Boolean
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename Insert_Policy>
void
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_ptr, typename S_traits, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S_ptr, typename S_traits, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  S_traits::release (this->x_);
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_traits, typename Insert_Policy>
void
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_ptr, typename S_out, typename Insert_Policy>
CORBA::Boolean
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_out, typename Insert_Policy>
void
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_ptr, typename S_var, typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_var, typename Insert_Policy>
void
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_OBJECT_ARGUMENT_T_C */
