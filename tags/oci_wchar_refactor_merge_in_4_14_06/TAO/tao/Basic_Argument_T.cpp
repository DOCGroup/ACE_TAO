// $Id$

#ifndef TAO_BASIC_ARGUMENT_T_C
#define TAO_BASIC_ARGUMENT_T_C

#include "tao/Basic_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Basic_Argument_T,
           "$Id$")

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::In_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::In_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Out_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::Out_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename Insert_Policy>
void
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_BASIC_ARGUMENT_T_C */
