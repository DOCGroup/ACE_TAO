// $Id$

#ifndef TAO_BD_STRING_ARGUMENT_T_C
#define TAO_BD_STRING_ARGUMENT_T_C

#include "tao/BD_String_Argument_T.h"

ACE_RCSID (tao,
           String_Argument_T,
           "$Id$")

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::In_BD_String_Argument_T (
    S * const & x
  )
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR &
  )
{
  return 1;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::
Inout_BD_String_Argument_T (S *& x)
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Out_BD_String_Argument_T<S,to_S,from_S,BOUND>::Out_BD_String_Argument_T (
    S *& x
  )
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_Argument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR &
  )
{
  return 1;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_Argument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Out_BD_String_Argument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter &
  )
{
}

// ============================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Ret_BD_String_Argument_T<S,to_S,from_S,BOUND>::Ret_BD_String_Argument_T (
    void
  )
{
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_Argument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Ret_BD_String_Argument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    CORBA::Any * any
  )
{
  (*any) <<= from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Ret_BD_String_Argument_T<S,to_S,from_S,BOUND>::operator S * () const
{
  return this->x_;
}

// ============================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::In_BD_String_SArgument_T<S,to_S,from_S,BOUND>::In_BD_String_SArgument_T (
    void
  )
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_SArgument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR &
  )
{
  return 1;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_SArgument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::In_BD_String_SArgument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::In_BD_String_SArgument_T<S,to_S,from_S,BOUND>::operator S * () const
{
  return this->x_;
}

// ===========================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Inout_BD_String_SArgument_T<S,to_S,from_S,BOUND>::
Inout_BD_String_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Inout_BD_String_SArgument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Inout_BD_String_SArgument_T<S,to_S,from_S,BOUND>::operator S *& ()
{
  return this->x_;
}

// ==============================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Out_BD_String_SArgument_T<S,to_S,from_S,BOUND>::
Out_BD_String_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_SArgument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR &cdr
  )
{
  return cdr << from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_SArgument_T<S,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR &
  )
{
  return 1;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Out_BD_String_SArgument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    Dynamic::Parameter &
  )
{
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Out_BD_String_SArgument_T<S,to_S,from_S,BOUND>::operator S *& ()
{
  return this->x_;
}

// ============================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Ret_BD_String_SArgument_T<S,to_S,from_S,BOUND>::
Ret_BD_String_SArgument_T (void)
{
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_SArgument_T<S,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
void
TAO::Ret_BD_String_SArgument_T<S,to_S,from_S,BOUND>::add_to_interceptor (
    CORBA::Any * any
  )
{
  (*any) <<= from_S (this->x_, BOUND);
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Ret_BD_String_SArgument_T<S,to_S,from_S,BOUND>::operator S * () const
{
  return this->x_;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::Ret_BD_String_SArgument_T<S,to_S,from_S,BOUND>::operator S *& ()
{
  return this->x_;
}

#endif /* TAO_BD_STRING_ARGUMENT_T_C */
