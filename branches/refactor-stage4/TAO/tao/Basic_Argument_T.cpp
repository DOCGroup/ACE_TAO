// $Id$

#ifndef TAO_BASIC_ARGUMENT_T_C
#define TAO_BASIC_ARGUMENT_T_C

#include "tao/Basic_Argument_T.h"

ACE_RCSID (tao,
           Basic_Argument_T,
           "$Id$")

template<typename S>
TAO::In_Basic_Argument_T<S>::In_Basic_Argument_T (S const & x)
  : x_ (x)
{}

template<typename S>
CORBA::Boolean
TAO::In_Basic_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
void
TAO::In_Basic_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S>
CORBA::Boolean
TAO::In_Basic_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_Basic_Argument_T<S>::Inout_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S>
void
TAO::Inout_Basic_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S>
TAO::Out_Basic_Argument_T<S>::Out_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S>
CORBA::Boolean
TAO::Out_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

// ============================================================

template<typename S>
TAO::Ret_Basic_Argument_T<S>::Ret_Basic_Argument_T (void)
{
}

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S>
void
TAO::Ret_Basic_Argument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S
TAO::Ret_Basic_Argument_T<S>::excp (void)
{
  return this->x_;
}

template<typename S>
S
TAO::Ret_Basic_Argument_T<S>::retn (void)
{
  return this->x_;
}

// ============================================================

template<typename S>
TAO::In_Basic_SArgument_T<S>::In_Basic_SArgument_T (void)
{}

template<typename S>
CORBA::Boolean
TAO::In_Basic_SArgument_T<S>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_;
}

template<typename S>
void
TAO::In_Basic_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S>
CORBA::Boolean
TAO::In_Basic_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S
TAO::In_Basic_SArgument_T<S>::arg (void) const
{
  return this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_Basic_SArgument_T<S>::Inout_Basic_SArgument_T (void)
{}

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

template<typename S>
void
TAO::Inout_Basic_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S &
TAO::Inout_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ==============================================================

template<typename S>
TAO::Out_Basic_SArgument_T<S>::Out_Basic_SArgument_T (void)
{}

template<typename S>
CORBA::Boolean
TAO::Out_Basic_SArgument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S>
S &
TAO::Out_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ============================================================

template<typename S>
TAO::Ret_Basic_SArgument_T<S>::Ret_Basic_SArgument_T (void)
{
}

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
void
TAO::Ret_Basic_SArgument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S &
TAO::Ret_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

#endif /* TAO_BASIC_ARGUMENT_T_C */
