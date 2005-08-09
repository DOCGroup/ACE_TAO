// $Id$

#ifndef TAO_UB_STRING_ARGUMENT_T_C
#define TAO_UB_STRING_ARGUMENT_T_C

#include "tao/UB_String_Argument_T.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           UB_String_Argument_T,
           "$Id$")

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::In_UB_String_Argument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::In_UB_String_Argument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  delete [] this->x_;
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::Inout_UB_String_Argument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename S_out, typename Insert_Policy>
CORBA::Boolean
TAO::Out_UB_String_Argument_T<S,S_out,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_out, typename Insert_Policy>
void
TAO::Out_UB_String_Argument_T<S,S_out,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename S_var, typename Insert_Policy>
CORBA::Boolean
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_var, typename Insert_Policy>
void
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_UB_STRING_ARGUMENT_T_C */
