// $Id$

#ifndef TAO_VAR_SIZE_ARGUMENT_T_C
#define TAO_VAR_SIZE_ARGUMENT_T_C

#include "tao/Var_Size_Argument_T.h"

ACE_RCSID (tao,
           Var_Size_Argument_T,
           "$Id$")

template<typename S>
TAO::In_Var_Size_Argument_T<S>::In_Var_Size_Argument_T (S const & x)
  : x_ (&x)
{}

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
TAO::Inout_Var_Size_Argument_T<S>::Inout_Var_Size_Argument_T (S & x)
  : x_ (&x)
{}

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
TAO::Out_Var_Size_Argument_T<S,S_out>::Out_Var_Size_Argument_T (S_out x)
  : x_ (x.ptr ())
{
}

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
TAO::Ret_Var_Size_Argument_T<S,S_var>::Ret_Var_Size_Argument_T (void)
{
}

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

template<typename S, typename S_var>
S *
TAO::Ret_Var_Size_Argument_T<S,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_var>
S *
TAO::Ret_Var_Size_Argument_T<S,S_var>::retn (void)
{
  return this->x_._retn ();
}

// ============================================================

template<typename S>
TAO::In_Var_Size_SArgument_T<S>::In_Var_Size_SArgument_T (void)
{}

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

template<typename S>
const S &
TAO::In_Var_Size_SArgument_T<S>::arg (void) const
{
  return *this->x_;
}

// ===========================================================

template<typename S>
TAO::Inout_Var_Size_SArgument_T<S>::Inout_Var_Size_SArgument_T (void)
{}

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

template<typename S>
S &
TAO::Inout_Var_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

// ==============================================================

template<typename S, typename S_var>
TAO::Out_Var_Size_SArgument_T<S,S_var>::Out_Var_Size_SArgument_T (void)
{}

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_Var_Size_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
S *&
TAO::Out_Var_Size_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

// ============================================================

template<typename S, typename S_var>
TAO::Ret_Var_Size_SArgument_T<S,S_var>::Ret_Var_Size_SArgument_T (void)
{
}

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

template<typename S, typename S_var>
S *&
TAO::Ret_Var_Size_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

#endif /* TAO_VAR_SIZE_ARGUMENT_T_C */
