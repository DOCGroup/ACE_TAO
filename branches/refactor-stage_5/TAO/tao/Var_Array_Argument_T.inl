// $Id$

template<typename S, typename S_slice, typename S_forany>
ACE_INLINE
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::
In_Var_Array_Argument_T (const S x)
  : x_ ((S_slice *) x)
{}

template<typename S, typename S_slice, typename S_forany>
ACE_INLINE
const S_slice *
TAO::In_Var_Array_Argument_T<S,S_slice,S_forany>::arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S, typename S_slice, typename S_forany>
ACE_INLINE
TAO::Inout_Var_Array_Argument_T<S,S_slice,S_forany>::
Inout_Var_Array_Argument_T (S x)
  : x_ (x)
{}

template<typename S, typename S_slice, typename S_forany>
ACE_INLINE
S_slice *
TAO::Inout_Var_Array_Argument_T<S,S_slice,S_forany>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S, 
         typename S_slice, 
         typename S_var, 
         typename S_out, 
         typename S_forany>
ACE_INLINE
TAO::Out_Var_Array_Argument_T<S,S_slice,S_var,S_out,S_forany>::
Out_Var_Array_Argument_T (S_out x)
  : x_ (x.ptr ())
{
}

template<typename S, 
         typename S_slice, 
         typename S_var, 
         typename S_out, 
         typename S_forany>
ACE_INLINE
S_slice *&
TAO::Out_Var_Array_Argument_T<S,S_slice,S_var,S_out,S_forany>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::
Ret_Var_Array_Argument_T (void)
{}

template<typename S, typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
S_slice *&
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
S_slice *
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
S_slice *
TAO::Ret_Var_Array_Argument_T<S,S_slice,S_var,S_forany>::retn (void)
{
  return this->x_._retn ();
}

// ==========================================================================

template<typename S, typename S_forany>
ACE_INLINE
TAO::In_Var_Array_SArgument_T<S,S_forany>::In_Var_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
ACE_INLINE
const S &
TAO::In_Var_Array_SArgument_T<S,S_forany>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_forany>
ACE_INLINE
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::
Inout_Var_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
S &
TAO::Inout_Var_Array_SArgument_T<S,S_forany>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
TAO::Out_Var_Array_SArgument_T<S_slice,S_var,S_forany>::
Out_Var_Array_SArgument_T (void)
{}

template<typename S_slice, typename S_var, typename S_forany>
S_slice *&
TAO::Out_Var_Array_SArgument_T<S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::
Ret_Var_Array_SArgument_T (void)
{}

template<typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
S_slice *&
TAO::Ret_Var_Array_SArgument_T<S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}

