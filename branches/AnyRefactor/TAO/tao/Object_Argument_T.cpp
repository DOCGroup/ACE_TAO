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
TAO::In_Object_Argument_T<S_ptr>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr>
void
TAO::In_Object_Argument_T<S_ptr>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_ptr, typename S_traits>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S_ptr, typename S_traits>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits>::demarshal (TAO_InputCDR & cdr)
{
  S_traits::release (this->x_);
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_traits>
void
TAO::Inout_Object_Argument_T<S_ptr,S_traits>::interceptor_value (
    CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_ptr, typename S_out>
CORBA::Boolean
TAO::Out_Object_Argument_T<S_ptr,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_out>
void
TAO::Out_Object_Argument_T<S_ptr,S_out>::interceptor_value (
    CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_ptr, typename S_var>
CORBA::Boolean
TAO::Ret_Object_Argument_T<S_ptr,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr, typename S_var>
void
TAO::Ret_Object_Argument_T<S_ptr,S_var>::interceptor_value (
    CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_OBJECT_ARGUMENT_T_C */
