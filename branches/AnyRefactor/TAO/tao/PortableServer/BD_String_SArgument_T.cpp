// $Id$

#ifndef TAO_BD_STRING_SARGUMENT_T_CPP
#define TAO_BD_STRING_SARGUMENT_T_CPP

#include "tao/PortableServer/BD_String_SArgument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/BD_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_.out (), BOUND);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
void
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::interceptor_value (
  CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_.in (), BOUND);
}

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_.out (), BOUND);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
void
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_value (CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename S_var,
         typename S_out,
         typename to_S,
         typename from_S,
         size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_SArgument_T<S,S_var,S_out,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR &cdr
  )
{
  return cdr << from_S (this->x_.in (), BOUND);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_var,
         typename S_out,
         typename to_S,
         typename from_S,
         size_t BOUND>
void
TAO::Out_BD_String_SArgument_T<S,S_var,S_out,to_S,from_S,BOUND>::
interceptor_value (CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_.in (), BOUND);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND>
void
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_value (CORBA::Any *any) const
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_BD_STRING_SARGUMENT_T_CPP */
