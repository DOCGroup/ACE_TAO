// $Id$

#ifndef TAO_VAR_SIZE_ARGUMENT_T_C
#define TAO_VAR_SIZE_ARGUMENT_T_C

#include "tao/Var_Size_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Var_Size_Argument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_Var_Size_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
void
TAO::In_Var_Size_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S>
CORBA::Boolean
TAO::In_Var_Size_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::Inout_Var_Size_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S, typename S_out>
CORBA::Boolean
TAO::Out_Var_Size_Argument_T<S,S_out>::demarshal (TAO_InputCDR & cdr)
{
  ACE_NEW_RETURN (this->x_,
                  S,
                  0);
  return cdr >> *this->x_;
}

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_Var_Size_Argument_T<S,S_var>::demarshal (TAO_InputCDR & cdr)
{
  S * tmp = 0;
  ACE_NEW_RETURN (tmp,
                  S,
                  0);
  this->x_ = tmp;
  return cdr >> this->x_.inout ();
}

template<typename S, typename S_var>
void
TAO::Ret_Var_Size_Argument_T<S,S_var>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_.in ();
}

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_Var_Size_Argument_T<S,S_var>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_.out ();
}

// ============================================================

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

template<typename S>
CORBA::Boolean
TAO::In_Var_Size_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
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

template<typename S>
CORBA::Boolean
TAO::Inout_Var_Size_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_Var_Size_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
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

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_Var_Size_SArgument_T<S,S_var>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_.out ();
}

#endif /* TAO_VAR_SIZE_ARGUMENT_T_C */
