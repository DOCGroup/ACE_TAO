// $Id$

#ifndef TAO_BASIC_ARGUMENT_T_C
#define TAO_BASIC_ARGUMENT_T_C

#include "tao/Basic_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Basic_Argument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_Basic_Argument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::In_Basic_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Inout_Basic_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S>
CORBA::Boolean
TAO::Out_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Out_Basic_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Ret_Basic_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_BASIC_ARGUMENT_T_C */
