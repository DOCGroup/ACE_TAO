// $Id$

#ifndef TAO_FIXED_ARRAY_SARGUMENT_T_CPP
#define TAO_FIXED_ARRAY_SARGUMENT_T_CPP

#include "tao/PortableServer/Fixed_Array_SArgument_T.h"
#include "tao/Dynamic_ParameterC.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Fixed_Array_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::In_Fixed_Array_SArgument_T<S,
                                S_slice,
                                S_forany>::demarshal (TAO_InputCDR &cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::In_Fixed_Array_SArgument_T<S,
                                S_slice,
                                S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_IN;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_SArgument_T<S,
                                   S_slice,
                                   S_forany>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << S_forany (this->x_);
}

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_SArgument_T<S,
                                   S_slice,
                                   S_forany>::demarshal (TAO_InputCDR & cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::Inout_Fixed_Array_SArgument_T<S,
                                   S_slice,
                                   S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_INOUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Out_Fixed_Array_SArgument_T<S,
                                 S_slice,
                                 S_forany>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << S_forany (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::Out_Fixed_Array_SArgument_T<S,
                                 S_slice,
                                 S_forany>::interceptor_param (
    Dynamic::Parameter & p)
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << S_forany (this->x_.inout ());
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_slice, typename S_var, typename S_forany>
void
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::interceptor_result (
    CORBA::Any * any
  )
{
  (*any) <<= S_forany (this->x_.ptr ());
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_FIXED_ARRAY_SARGUMENT_T_CPP */
