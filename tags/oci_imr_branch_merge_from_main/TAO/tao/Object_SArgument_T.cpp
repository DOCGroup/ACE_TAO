// $Id$

#ifndef TAO_OBJECT_SARGUMENT_T_C
#define TAO_OBJECT_SARGUMENT_T_C

#include "tao/Object_SArgument_T.h"
#include "tao/Dynamic_ParameterC.h"

#if !defined (__ACE_INLINE__)
#include "tao/Object_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Object_SArgument_T,
           "$Id$")

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::In_Object_SArgument_T<S_ptr,S_var>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S_ptr, typename S_var>
void
TAO::In_Object_SArgument_T<S_ptr,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S_ptr, typename S_var>
void
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S_ptr, typename S_var, typename S_out>
CORBA::Boolean
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr, typename S_var, typename S_out>
void
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_.in ();
  p.mode = CORBA::PARAM_OUT;
}

// ============================================================

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr, typename S_var>
void
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::interceptor_result (
    CORBA::Any * any
  )
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_OBJECT_SARGUMENT_T_C */
