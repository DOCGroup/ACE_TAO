// $Id$

#ifndef TAO_FIXED_SIZE_ARGUMENT_T_C
#define TAO_FIXED_SIZE_ARGUMENT_T_C

#include "tao/Fixed_Size_Argument_T.h"

ACE_RCSID (tao,
           Fixed_Size_Argument_T,
           "$Id$")

template<typename S>
TAO::In_Fixed_Size_Argument_T<S>::In_Fixed_Size_Argument_T (S const & x)
  : x_ (&x)
{}

template<typename S>
CORBA::Boolean
TAO::In_Fixed_Size_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
void
TAO::In_Fixed_Size_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S>
CORBA::Boolean
TAO::In_Fixed_Size_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_Fixed_Size_Argument_T<S>::Inout_Fixed_Size_Argument_T (S & x)
  : x_ (&x)
{}

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::Inout_Fixed_Size_Argument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S>
TAO::Out_Fixed_Size_Argument_T<S>::Out_Fixed_Size_Argument_T (S & x)
  : x_ (x)
{}

template<typename S>
CORBA::Boolean
TAO::Out_Fixed_Size_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

// ============================================================

template<typename S>
TAO::Ret_Fixed_Size_Argument_T<S>::Ret_Fixed_Size_Argument_T (void)
{
}

template<typename S>
CORBA::Boolean
TAO::Ret_Fixed_Size_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::Ret_Fixed_Size_Argument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Ret_Fixed_Size_Argument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S
TAO::Ret_Fixed_Size_Argument_T<S>::excp (void)
{
  return *this->x_;
}

template<typename S>
S
TAO::Ret_Fixed_Size_Argument_T<S>::retn (void)
{
  return *this->x_;
}

// ============================================================

template<typename S>
TAO::In_Fixed_Size_SArgument_T<S>::In_Fixed_Size_SArgument_T (void)
{}

template<typename S>
CORBA::Boolean
TAO::In_Fixed_Size_SArgument_T<S>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> *this->x_;
}

template<typename S>
void
TAO::In_Fixed_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S>
CORBA::Boolean
TAO::In_Fixed_Size_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
const S &
TAO::In_Fixed_Size_SArgument_T<S>::arg (void) const
{
  return *this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_Fixed_Size_SArgument_T<S>::Inout_Fixed_Size_SArgument_T (void)
{}

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

template<typename S>
void
TAO::Inout_Fixed_Size_SArgument_T<S>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= *this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Fixed_Size_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S &
TAO::Inout_Fixed_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

// ==============================================================

template<typename S>
TAO::Out_Fixed_Size_SArgument_T<S>::Out_Fixed_Size_SArgument_T (void)
{}

template<typename S>
CORBA::Boolean
TAO::Out_Fixed_Size_SArgument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S>
S &
TAO::Out_Fixed_Size_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ============================================================

template<typename S>
TAO::Ret_Fixed_Size_SArgument_T<S>::Ret_Fixed_Size_SArgument_T (void)
{
}

template<typename S>
CORBA::Boolean
TAO::Ret_Fixed_Size_SArgument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << *this->x_;
}

template<typename S>
void
TAO::Ret_Fixed_Size_SArgument_T<S>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= *this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Ret_Fixed_Size_SArgument_T<S>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

template<typename S>
S &
TAO::Ret_Fixed_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

#endif /* TAO_FIXED_SIZE_ARGUMENT_T_C */
