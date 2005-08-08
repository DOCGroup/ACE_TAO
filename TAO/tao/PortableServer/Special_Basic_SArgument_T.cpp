// $Id$

#ifndef TAO_SPECIAL_BASIC_SARGUMENT_T_CPP
#define TAO_SPECIAL_BASIC_SARGUMENT_T_CPP

#include "tao/PortableServer/Special_Basic_SArgument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Special_Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


// ============================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S, typename to_S, typename from_S>
void
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_SPECIAL_BASIC_SARGUMENT_T_CPP */
