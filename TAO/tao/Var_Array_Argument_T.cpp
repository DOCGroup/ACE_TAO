// $Id$

#ifndef TAO_VAR_ARRAY_ARGUMENT_T_C
#define TAO_VAR_ARRAY_ARGUMENT_T_C

#include "tao/Var_Array_Argument_T.h"

ACE_RCSID (tao,
           Var_Array_Argument_T,
           "$Id$")

template<typename S, typename S_slice, typename S_forany>
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::
In_Var_Array_Argument_T (const S x)
  : x_ ((S_slice *) x)
{}

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

template<typename S, typename S_slice, typename S_forany>
void
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename S_slice, typename S_forany>
CORBA::Boolean
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_;
}

// ===========================================================

template<typename S, typename S_forany>
TAO::Inout_Var_Array_Argument_T<S,S_forany>::Inout_Var_Array_Argument_T (
    S x
  )
  : x_ (x)
{}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_Argument_T<S,S_forany>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_Argument_T<S,S_forany>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

template<typename S, typename S_forany>
void
TAO::Inout_Var_Array_Argument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= this->x_;
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_Argument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  return any >>= this->x_;
}

// ==============================================================

template<typename S, 
         typename S_slice, 
         typename S_var, 
         typename S_out, 
         typename S_forany>
TAO::Out_Var_Array_Argument_T<S,S_slice,S_var,S_out,S_forany>::
Out_Var_Array_Argument_T (S_out x)
{
  typedef TAO::Array_Traits<S,S_slice> ARRAY_TRAITS;
  ACE_ALLOCATOR (x.ptr (),
                 ARRAY_TRAITS::tao_alloc ());
  this->x_ = x.ptr ();
}

template<typename S, 
         typename S_slice, 
         typename S_var, 
         typename S_out, 
         typename S_forany>
CORBA::Boolean
TAO::Out_Var_Array_Argument_T<S,S_slice,S_var,S_out,S_forany>::demarshal (
    TAO_InputCDR & cdr
  )
{
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

// ============================================================

template<typename S, typename S_slice, typename S_var, typename S_forany>
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::
Ret_Var_Array_Argument_T (void)
{
  typedef TAO::Array_Traits<S,S_slice> ARRAY_TRAITS;
  S_slice * tmp = 0;
  ACE_ALLOCATOR (tmp,
                 ARRAY_TRAITS::tao_alloc ());
  this->x_ = tmp;
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::demarshal (
    TAO_InputCDR & cdr
  )
{
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
void
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::
interceptor_result (CORBA::Any * any)
{
  (*any) <<= S_forany (this->x_.ptr ());
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::
interceptor_replace (CORBA::Any & any)
{
  S_forany tmp (this->x_.ptr ());
  return any >>= tmp;
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
S_slice *
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
S_slice *
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::retn (void)
{
  return this->x_._retn ();
}

// ============================================================

template<typename S, typename S_forany>
TAO::In_Var_Array_SArgument_T<S,S_forany>::In_Var_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::In_Var_Array_SArgument_T<S,S_forany>::demarshal (TAO_InputCDR &cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

template<typename S, typename S_forany>
void
TAO::In_Var_Array_SArgument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_IN;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::In_Var_Array_SArgument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  S_forany tmp (this->x_);
  return any >>= tmp;
}

template<typename S, typename S_forany>
const S &
TAO::In_Var_Array_SArgument_T<S,S_forany>::arg (void) const
{
  return this->x_;
}

// ===========================================================

template<typename S, typename S_forany>
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::
Inout_Var_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << S_forany (this->x_);
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::demarshal (TAO_InputCDR & cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

template<typename S, typename S_forany>
void
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::interceptor_param (
    Dynamic::Parameter & p
  )
{
  p.argument <<= S_forany (this->x_);
  p.mode = CORBA::PARAM_INOUT;
}

template<typename S, typename S_forany>
CORBA::Boolean
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::interceptor_replace (
    CORBA::Any & any
  )
{
  S_forany tmp (this->x_);
  return any >>= tmp;
}

template<typename S, typename S_forany>
S &
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::arg (void)
{
  return this->x_;
}

// ==============================================================

template<typename S_slice, typename S_var, typename S_forany>
TAO::Out_Var_Array_SArgument_T<S_slice,S_var,S_forany>::
Out_Var_Array_SArgument_T (void)
{}

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Out_Var_Array_SArgument_T<S_slice,S_var,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << S_forany (this->x_.ptr ());
}

template<typename S_slice, typename S_var, typename S_forany>
S_slice *&
TAO::Out_Var_Array_SArgument_T<S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}

// ============================================================

template<typename S_slice, typename S_var, typename S_forany>
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::
Ret_Var_Array_SArgument_T (void)
{
}

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << S_forany (this->x_.ptr ());
}

template<typename S_slice, typename S_var, typename S_forany>
void
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::interceptor_result (
    CORBA::Any * any
  )
{
  (*any) <<= S_forany (this->x_.ptr ());
}

template<typename S_slice, typename S_var, typename S_forany>
CORBA::Boolean
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::
interceptor_replace (CORBA::Any & any)
{
  S_forany tmp (this->x_.ptr ());
  return any >>= tmp;
}

template<typename S_slice, typename S_var, typename S_forany>
S_slice *&
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}

#endif /* TAO_VAR_ARRAY_ARGUMENT_T_C */
