// $Id$

#ifndef TAO_UB_STRING_ARGUMENT_T_C
#define TAO_UB_STRING_ARGUMENT_T_C

#include "tao/UB_String_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           UB_String_Argument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_UB_String_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
void
TAO::In_UB_String_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  delete [] this->x_;
  return cdr >> this->x_;
}

template<typename S>
void
TAO::Inout_UB_String_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S, typename S_out>
CORBA::Boolean
TAO::Out_UB_String_Argument_T<S,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S, typename S_out>
void
TAO::Out_UB_String_Argument_T<S,S_out>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_OUT;
}

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_Argument_T<S,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S, typename S_var>
void
TAO::Ret_UB_String_Argument_T<S,S_var>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_.in ();
}

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::In_UB_String_SArgument_T<S,S_var>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S, typename S_var>
void
TAO::In_UB_String_SArgument_T<S,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Inout_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
CORBA::Boolean
TAO::Inout_UB_String_SArgument_T<S,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S, typename S_var>
void
TAO::Inout_UB_String_SArgument_T<S,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
void
TAO::Out_UB_String_SArgument_T<S,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_OUT;
}

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
void
TAO::Ret_UB_String_SArgument_T<S,S_var>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_UB_STRING_ARGUMENT_T_C */
