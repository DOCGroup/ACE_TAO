// $Id$

#ifndef TAO_BD_STRING_ARGUMENT_T_C
#define TAO_BD_STRING_ARGUMENT_T_C

#include "tao/BD_String_Argument_T.h"

ACE_RCSID (tao,
           BD_String_Argument_T,
           "$Id$")

template<typename S, typename to_S, typename from_S, size_t BOUND>
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::In_BD_String_Argument_T (
    const S * x
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
void
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= to_S (this->x_, BOUND);
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
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= from_S (this->x_, BOUND);
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename to_S, typename from_S, size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= to_S (this->x_, BOUND);
}

// ==============================================================

template<typename S, 
         typename S_out, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::Out_BD_String_Argument_T<S,S_out,to_S,from_S,BOUND>::
Out_BD_String_Argument_T (S_out x)
  : x_ (x.ptr ())
{}

template<typename S, 
         typename S_out, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_Argument_T<S,S_out,to_S,from_S,BOUND>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_, BOUND);
}

// ============================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::
Ret_BD_String_Argument_T (void)
{
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::demarshal (
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
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::interceptor_result (
    CORBA::Any * any
  )
{
  (*any) <<= from_S (this->x_.in (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_replace (CORBA::Any & any)
{
  return any >>= to_S (this->x_.out (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
S *
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
S *
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::retn (void)
{
  return this->x_._retn ();
}

// ============================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
In_BD_String_SArgument_T (void)
{}

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_replace (CORBA::Any & any)
{
  return any >>= to_S (this->x_.out (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
const S *
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void) const
{
  return this->x_.in ();
}

// ===========================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Inout_BD_String_SArgument_T (void)
{}

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_replace (CORBA::Any & any)
{
  return any >>= to_S (this->x_.in (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
S *&
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.inout ();
}

// ==============================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::Out_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Out_BD_String_SArgument_T (void)
{}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Out_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::marshal (
    TAO_OutputCDR &cdr
  )
{
  return cdr << from_S (this->x_.in (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
S *&
TAO::Out_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.out ();
}

// ============================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Ret_BD_String_SArgument_T (void)
{
}

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

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
CORBA::Boolean
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
interceptor_replace (CORBA::Any & any)
{
  return any >>= to_S (this->x_.out (), BOUND);
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
S *&
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.out ();
}

#endif /* TAO_BD_STRING_ARGUMENT_T_C */
