// $Id$

#ifndef TAO_UB_STRING_ARGUMENT_T_C
#define TAO_UB_STRING_ARGUMENT_T_C

#include "tao/UB_String_Argument_T.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           UB_String_Argument_T,
           "$Id$")

template<typename S>
CORBA::Boolean
TAO::In_UB_String_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<>
void
TAO::In_UB_String_Argument_T<char>::interceptor_value (CORBA::Any *any) const
{
  //p.argument <<= this->x_;
  //p.mode = CORBA::PARAM_IN;
}

template<typename S>
void
TAO::In_UB_String_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S>::demarshal (TAO_InputCDR & cdr)
{
  delete [] this->x_;
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S>
void
TAO::Inout_UB_String_Argument_T<S>::interceptor_value (CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename S_out>
CORBA::Boolean
TAO::Out_UB_String_Argument_T<S,S_out>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_out>
void
TAO::Out_UB_String_Argument_T<S,S_out>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_;
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_Argument_T<S,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<>
void
TAO::Ret_UB_String_Argument_T<char, CORBA::String_var>::interceptor_value (CORBA::Any *any) const
{
  //(*any) <<= this->x_.in ();
}


template<typename S, typename S_var>
void
TAO::Ret_UB_String_Argument_T<S,S_var>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_UB_STRING_ARGUMENT_T_C */
