// $Id$

#ifndef TAO_OBJECT_ARGUMENT_T_C
#define TAO_OBJECT_ARGUMENT_T_C

#include "tao/Object_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Object_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Object_Argument_T,
           "$Id$")

template<typename S_ptr>
CORBA::Boolean
TAO::In_Object_Argument_T<S_ptr>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S_ptr>
void
TAO::In_Object_Argument_T<S_ptr>::interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S_ptr>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S_ptr>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr>::demarshal (TAO_InputCDR & cdr)
{
  CORBA::release (this->x_);
  return cdr >> this->x_;
}

template<typename S_ptr>
void
TAO::Inout_Object_Argument_T<S_ptr>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S_ptr, typename S_out>
CORBA::Boolean
TAO::Out_Object_Argument_T<S_ptr,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S_ptr, typename S_out>
void
TAO::Out_Object_Argument_T<S_ptr,S_out>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_OUT;
}

// ============================================================

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Ret_Object_Argument_T<S_ptr,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

template<typename S_ptr, typename S_var>
void
TAO::Ret_Object_Argument_T<S_ptr,S_var>::interceptor_result (CORBA::Any * any)
{
  (*any) <<= this->x_.in ();
}

// ============================================================

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

#endif /* TAO_OBJECT_ARGUMENT_T_C */
