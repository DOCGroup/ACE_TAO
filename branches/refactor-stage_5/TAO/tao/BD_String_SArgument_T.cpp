// $Id$

#ifndef TAO_BD_STRING_SARGUMENT_T_C
#define TAO_BD_STRING_SARGUMENT_T_C

#include "tao/BD_String_SArgument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/BD_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           BD_String_SArgument_T,
           "$Id$")

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
void
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_.in (), BOUND);
  p.mode = CORBA::PARAM_IN;
}

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
void
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= from_S (this->x_.in (), BOUND);
  p.mode = CORBA::PARAM_INOUT;
}

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

template<typename S, 
         typename S_var, 
         typename S_out,
         typename to_S, 
         typename from_S, 
         size_t BOUND>
void
TAO::Out_BD_String_SArgument_T<S,S_var,S_out,to_S,from_S,BOUND>::
interceptor_param (Dynamic::Parameter & p)
{
  p.argument <<= from_S (this->x_.in (), BOUND);
  p.mode = CORBA::PARAM_OUT;
}

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
void
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_result (CORBA::Any * any)
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

#endif /* TAO_BD_STRING_SARGUMENT_T_C */
