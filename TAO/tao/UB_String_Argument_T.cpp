// $Id$

#ifndef TAO_UB_STRING_ARGUMENT_T_C
#define TAO_UB_STRING_ARGUMENT_T_C

#include "tao/UB_String_Argument_T.h"

ACE_RCSID (tao,
           String_Argument_T,
           "$Id$")

template<typename S>
TAO::In_UB_String_Argument_T<S>::In_UB_String_Argument_T (const S * x)
  : x_ (x)
{}

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

template<typename S>
CORBA::Boolean
TAO::In_UB_String_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_UB_String_Argument_T<S>::Inout_UB_String_Argument_T (S *& x)
  : x_ (x)
{}

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
  CORBA::string_free (this->x_);
  return cdr >> this->x_;
}

template<typename S>
void
TAO::Inout_UB_String_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S, typename S_out>
TAO::Out_UB_String_Argument_T<S,S_out>::Out_UB_String_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S, typename S_out>
CORBA::Boolean
TAO::Out_UB_String_Argument_T<S,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

// ============================================================

template<typename S, typename S_var>
TAO::Ret_UB_String_Argument_T<S,S_var>::Ret_UB_String_Argument_T (void)
{
}

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

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_Argument_T<S,S_var>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S, typename S_var>
S *
TAO::Ret_UB_String_Argument_T<S,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_var>
S *
TAO::Ret_UB_String_Argument_T<S,S_var>::retn (void)
{
  return this->x_._retn ();
}

// ============================================================

template<typename S, typename S_var>
TAO::In_UB_String_SArgument_T<S,S_var>::In_UB_String_SArgument_T (void)
{}

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

template<typename S, typename S_var>
CORBA::Boolean
TAO::In_UB_String_SArgument_T<S,S_var>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_.out ();
}

template<typename S, typename S_var>
const S *
TAO::In_UB_String_SArgument_T<S,S_var>::arg (void) const
{
  return this->x_.in ();
}

// ===========================================================

template<typename S, typename S_var>
TAO::Inout_UB_String_SArgument_T<S,S_var>::Inout_UB_String_SArgument_T (void)
{}

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

template<typename S, typename S_var>
CORBA::Boolean
TAO::Inout_UB_String_SArgument_T<S,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_.out ();
}

template<typename S, typename S_var>
S *&
TAO::Inout_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.inout ();
}

// ==============================================================

template<typename S, typename S_var>
TAO::Out_UB_String_SArgument_T<S,S_var>::Out_UB_String_SArgument_T (
    void
  )
{}

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
S *&
TAO::Out_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

// ============================================================

template<typename S, typename S_var>
TAO::Ret_UB_String_SArgument_T<S,S_var>::Ret_UB_String_SArgument_T (void)
{
}

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

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_SArgument_T<S,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_.out ();
}

template<typename S, typename S_var>
S *&
TAO::Ret_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

#endif /* TAO_UB_STRING_ARGUMENT_T_C */
