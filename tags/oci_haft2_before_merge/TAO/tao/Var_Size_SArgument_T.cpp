// $Id$

#ifndef TAO_VAR_SIZE_SARGUMENT_T_C
#define TAO_VAR_SIZE_SARGUMENT_T_C

#include "tao/Var_Size_SArgument_T.h"
#include "tao/Dynamic_ParameterC.h"

#if !defined (__ACE_INLINE__)
#include "tao/Var_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Var_Size_SArgument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_Var_Size_SArgument_T<S>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::In_Var_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_SArgument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::Inout_Var_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_Var_Size_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
void
TAO::Out_Var_Size_SArgument_T<S,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_OUT;
}

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_Var_Size_SArgument_T<S,S_var>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
void
TAO::Ret_Var_Size_SArgument_T<S,S_var>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_VAR_SIZE_SARGUMENT_T_C */
