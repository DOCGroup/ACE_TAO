// $Id$

#ifndef TAO_BASIC_ARGUMENT_T_C
#define TAO_BASIC_ARGUMENT_T_C

#include "tao/Special_Basic_Argument_T.h"

ACE_RCSID (tao,
           Special_Basic_Argument_T,
           "$Id$")

template<typename S, typename to_S, typename from_S>
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::In_Special_Basic_Argument_T (
    const S & x
  )
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<typename S, typename to_S, typename from_S>
void
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_);
  p.mode = CORBA::PARAM_IN;
}

// ===========================================================

template<typename S, typename to_S, typename from_S>
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::
Inout_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
void
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_);
  p.mode = CORBA::PARAM_INOUT;
}

// ==============================================================

template<typename S, typename to_S, typename from_S>
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::
Out_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::marshal (TAO_OutputCDR &)
{
  return 1;
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
void
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter &
  )
{
}

// ============================================================

template<typename S, typename to_S, typename from_S>
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::
Ret_Special_Basic_Argument_T (void)
{
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
void
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::add_to_interceptor (
    CORBA::Any * any
  )
{
  (*any) <<= this->x_;
}

// ============================================================

template<typename S, typename to_S, typename from_S>
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::
In_Special_Basic_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (TAO_OutputCDR &)
{
  return 1;
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
void
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_);
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename to_S, typename from_S>
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::operator S () const
{
  return this->x_;
}

// ===========================================================

template<typename S, typename to_S, typename from_S>
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::
Inout_Special_Basic_SArgument_T (void)
{}

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

template<typename S, typename to_S, typename from_S>
void
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_);
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename to_S, typename from_S>
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::operator S & ()
{
  return this->x_;
}

// ==============================================================

template<typename S, typename to_S, typename from_S>
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::
Out_Special_Basic_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::demarshal (TAO_InputCDR &)
{
  return 1;
}

template<typename S, typename to_S, typename from_S>
void
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::add_to_interceptor (
    Dynamic::Parameter &
  )
{
}

template<typename S, typename to_S, typename from_S>
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::operator S & ()
{
  return this->x_;
}

// ==============================================================

template<typename S, typename to_S, typename from_S>
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::
Ret_Special_Basic_SArgument_T (void)
{
}

template<typename S, typename to_S, typename from_S>
CORBA::Boolean
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S, typename to_S, typename from_S>
void
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::add_to_interceptor (
    CORBA::Any * any
  )
{
  (*any) <<= this->x_;
}

template<typename S, typename to_S, typename from_S>
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::operator S () const
{
  return this->x_;
}

template<typename S, typename to_S, typename from_S>
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::operator S & ()
{
  return this->x_;
}

#endif /* TAO_BASIC_ARGUMENT_T_C */

