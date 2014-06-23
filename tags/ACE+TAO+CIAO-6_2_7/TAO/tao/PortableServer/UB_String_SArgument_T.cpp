// $Id$

#ifndef TAO_UB_STRING_SARGUMENT_T_CPP
#define TAO_UB_STRING_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/UB_String_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/UB_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S, typename S_var>
CORBA::Boolean
TAO::In_UB_String_SArgument_T<S,S_var>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_var>
void
TAO::In_UB_String_SArgument_T<S,S_var>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Inout_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S, typename S_var>
CORBA::Boolean
TAO::Inout_UB_String_SArgument_T<S,S_var>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_var>
void
TAO::Inout_UB_String_SArgument_T<S,S_var>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Out_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_var>
void
TAO::Out_UB_String_SArgument_T<S,S_var>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S, typename S_var>
CORBA::Boolean
TAO::Ret_UB_String_SArgument_T<S,S_var>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename S_var>
void
TAO::Ret_UB_String_SArgument_T<S,S_var>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= this->x_.in ();
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_UB_STRING_SARGUMENT_T_CPP */
