// $Id$

#ifndef TAO_FIXED_ARRAY_ARGUMENT_T_C
#define TAO_FIXED_ARRAY_ARGUMENT_T_C

#include "tao/Fixed_Array_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Fixed_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Fixed_Array_Argument_T,
           "$Id$")

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

template<typename S, typename S_slice, typename S_forany>
void
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_forany>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_forany>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S, typename S_forany>
void
TAO::Inout_Fixed_Array_Argument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Out_Fixed_Array_Argument_T<S,S_forany>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

// ============================================================

template<typename S, typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany>::demarshal (
    TAO_InputCDR & cdr
  )
{
  typedef TAO::Array_Traits<S,S_slice> ARRAY_TRAITS;
  S_slice * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::tao_alloc (),
                        0);
  this->x_ = tmp_ptr;
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
void
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany>::
interceptor_result (CORBA::Any * any)
{
  (*any) <<= S_forany (this->x_.ptr ());
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany>::
interceptor_replace (CORBA::Any & any)
{
  S_forany tmp (this->x_.ptr ());
  return any >>= tmp;
}

// ============================================================

template<typename S, typename S_forany>
CORBA::Boolean
TAO::In_Fixed_Array_SArgument_T<S,S_forany>::demarshal (TAO_InputCDR &cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

template<typename S, typename S_forany>
void
TAO::In_Fixed_Array_SArgument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::In_Fixed_Array_SArgument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  S_forany tmp (this->x_);
  return any >>= tmp;
}

// ===========================================================

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << S_forany (this->x_);
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::demarshal (TAO_InputCDR & cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

template<typename S, typename S_forany>
void
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  S_forany tmp (this->x_);
  return any >>= tmp;
}

// ==============================================================

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Out_Fixed_Array_SArgument_T<S,S_forany>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << S_forany (this->x_);
}

// ============================================================

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << S_forany (this->x_.inout ());
}

template<typename S_slice, typename S_var, typename S_forany>
void
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::interceptor_result (
    CORBA::Any * any
  )
{
  (*any) <<= S_forany (this->x_.ptr ());
}

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::
interceptor_replace (CORBA::Any & any)
{
  S_forany tmp (this->x_.inout ());
  return any >>= tmp;
}

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_C */
