// $Id$

#ifndef TAO_OBJECT_ARGUMENT_T_C
#define TAO_OBJECT_ARGUMENT_T_C

#include "tao/Object_Argument_T.h"

ACE_RCSID (tao,
           Object_Argument_T,
           "$Id$")

template<typename S_ptr>
TAO::In_Object_Argument_T<S_ptr>::In_Object_Argument_T (S_ptr x)
  : x_ (x)
{}

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

template<typename S_ptr>
CORBA::Boolean
TAO::In_Object_Argument_T<S_ptr>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S_ptr>
TAO::Inout_Object_Argument_T<S_ptr>::Inout_Object_Argument_T (S_ptr & x)
  : x_ (x)
{}

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

template<typename S_ptr>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr>::interceptor_replace (CORBA::Any & any)
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S_ptr, typename S_out>
TAO::Out_Object_Argument_T<S_ptr,S_out>::Out_Object_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S_ptr, typename S_out>
CORBA::Boolean
TAO::Out_Object_Argument_T<S_ptr,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

// ============================================================

template<typename S_ptr, typename S_var>
TAO::Ret_Object_Argument_T<S_ptr,S_var>::Ret_Object_Argument_T (void)
{
}

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

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Ret_Object_Argument_T<S_ptr,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_;
}

template<typename S_ptr, typename S_var>
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_ptr, typename S_var>
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var>::retn (void)
{
  return this->x_._retn ();
}

// ============================================================

template<typename S_ptr, typename S_var>
TAO::In_Object_SArgument_T<S_ptr,S_var>::In_Object_SArgument_T (void)
{}

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

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::In_Object_SArgument_T<S_ptr,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_.out ();
}

template<typename S_ptr, typename S_var>
S_ptr
TAO::In_Object_SArgument_T<S_ptr,S_var>::arg (void) const
{
  return this->x_.in ();
}

// ===========================================================

template<typename S_ptr, typename S_var>
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::Inout_Object_SArgument_T (void)
{}

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

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_.out ();
}

template<typename S_ptr, typename S_var>
S_ptr&
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::arg (void)
{
  return this->x_.inout ();
}

// ==============================================================

template<typename S_ptr, typename S_var, typename S_out>
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::Out_Object_SArgument_T (
    void
  )
{}

template<typename S_ptr, typename S_var, typename S_out>
CORBA::Boolean
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr, typename S_var, typename S_out>
S_out
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::arg (void)
{
  return this->x_.out ();
}

// ============================================================

template<typename S_ptr, typename S_var>
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::Ret_Object_SArgument_T (void)
{
}

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

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_.out ();
}

template<typename S_ptr, typename S_var>
S_ptr &
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::arg (void)
{
  return this->x_.out ();
}

#endif /* TAO_OBJECT_ARGUMENT_T_C */
