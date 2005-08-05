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

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<>
void
TAO::In_Special_Basic_Argument_T<bool, ACE_InputCDR::to_boolean, ACE_OutputCDR::from_boolean>::
  interceptor_value (CORBA::Any *any) const
{
//  p.argument <<= from_S (this->x_);
//  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename to_S, typename from_S>
void
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<>
void
TAO::Ret_Special_Basic_Argument_T<bool, ACE_InputCDR::to_boolean, ACE_OutputCDR::from_boolean>::interceptor_value (CORBA::Any *any) const
{
  //(*any) <<= from_S (this->x_);
}


template<typename S, typename to_S, typename from_S>
void
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_SPECIAL_BASIC_ARGUMENT_T_C */

