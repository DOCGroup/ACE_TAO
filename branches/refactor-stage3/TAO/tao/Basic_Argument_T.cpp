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
CORBA::Boolean
TAO::In_Basic_Argument_T<S>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<typename S>
void
TAO::In_Basic_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
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

// ==============================================================

template<typename S>
TAO::Out_Basic_Argument_T<S>::Out_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S>
CORBA::Boolean
TAO::Out_Basic_Argument_T<S>::marshal (TAO_OutputCDR &)
{
  return 1;
}

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
TAO::Ret_Basic_Argument_T<S>::marshal (TAO_OutputCDR &)
{
  return 1;
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
TAO::Ret_Basic_Argument_T<S>::operator S () const
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
TAO::In_Basic_SArgument_T<S>::marshal (TAO_OutputCDR &)
{
  return 1;
}

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
TAO::In_Basic_SArgument_T<S>::operator S () const
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
TAO::Inout_Basic_SArgument_T<S>::operator S & ()
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
CORBA::Boolean
TAO::Out_Basic_SArgument_T<S>::demarshal (TAO_InputCDR &)
{
  return 1;
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
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<S>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<typename S>
void
TAO::Ret_Basic_SArgument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_;
}

template<typename S>
TAO::Ret_Basic_SArgument_T<S> &
TAO::Ret_Basic_SArgument_T<S>::operator= (const S & rhs)
{
  this->x_ = rhs;
  return *this;
}

template<typename S>
TAO::Ret_Basic_SArgument_T<S>::operator S () const
{
  return this->x_;
}

#endif /* TAO_BASIC_ARGUMENT_T_C */
