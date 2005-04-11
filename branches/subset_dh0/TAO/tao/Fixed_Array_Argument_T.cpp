// $Id$

#ifndef TAO_FIXED_ARRAY_ARGUMENT_T_C
#define TAO_FIXED_ARRAY_ARGUMENT_T_C

#include "tao/Fixed_Array_Argument_T.h"
#include "tao/Dynamic_ParameterC.h"

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

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_slice, typename S_forany>
void
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_OUT;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
CORBA::Boolean
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::demarshal (
    TAO_InputCDR & cdr
  )
{
  typedef TAO::Array_Traits<S,S_slice,S_tag> ARRAY_TRAITS;
  S_slice * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::alloc (),
                        0);
  this->x_ = tmp_ptr;
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
void
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::
interceptor_result (CORBA::Any * any)
{
  (*any) <<= S_forany (this->x_.ptr ());
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_C */
