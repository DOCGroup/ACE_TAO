// $Id$

#ifndef TAO_SPECIAL_BASIC_ARGUMENT_T_C
#define TAO_SPECIAL_BASIC_ARGUMENT_T_C

#include "tao/Special_Basic_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Special_Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Special_Basic_Argument_T,
           "$Id$")

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
CORBA::Boolean
TAO::In_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
void
TAO::In_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
void
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
CORBA::Boolean
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
void
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S, typename Insert_Policy>
void
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_SPECIAL_BASIC_ARGUMENT_T_C */

