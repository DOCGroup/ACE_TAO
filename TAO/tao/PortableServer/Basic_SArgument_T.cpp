// $Id$

#ifndef TAO_BASIC_SARGUMENT_T_CPP
#define TAO_BASIC_SARGUMENT_T_CPP

#include "tao/PortableServer/Basic_SArgument_T.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/Dynamic_ParameterC.h"
#endif /* TAO_HAS_INTERCEPTORS */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


template<typename S>
CORBA::Boolean
TAO::In_Basic_SArgument_T<S>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::In_Basic_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_SArgument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Inout_Basic_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S>
CORBA::Boolean
TAO::Out_Basic_SArgument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Out_Basic_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Ret_Basic_SArgument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_BASIC_SARGUMENT_T_CPP */
