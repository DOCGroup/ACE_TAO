// $Id$

#ifndef TAO_FIXED_SIZE_SARGUMENT_T_C
#define TAO_FIXED_SIZE_SARGUMENT_T_C

#include "tao/Fixed_Size_SArgument_T.h"
#include "tao/Dynamic_ParameterC.h"

#if !defined (__ACE_INLINE__)
#include "tao/Fixed_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Fixed_Size_SArgument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_Fixed_Size_SArgument_T<S>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::In_Fixed_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_IN;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_SArgument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Inout_Fixed_Size_SArgument_T<S>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S>
CORBA::Boolean
TAO::Out_Fixed_Size_SArgument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Out_Fixed_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S>
CORBA::Boolean
TAO::Ret_Fixed_Size_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Ret_Fixed_Size_SArgument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= *this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_FIXED_SIZE_SARGUMENT_T_C */
