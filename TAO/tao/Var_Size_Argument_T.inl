// $Id$

template<typename S>
ACE_INLINE
TAO::In_Var_Size_Argument_T<S>::In_Var_Size_Argument_T (S const & x)
  : x_ (&x)
{}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Inout_Var_Size_Argument_T<S>::Inout_Var_Size_Argument_T (S & x)
  : x_ (&x)
{}

// ==========================================================================

template<typename S, typename S_out>
ACE_INLINE
TAO::Out_Var_Size_Argument_T<S,S_out>::Out_Var_Size_Argument_T (S_out x)
  : x_ (x.ptr ())
{}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Ret_Var_Size_Argument_T<S,S_var>::Ret_Var_Size_Argument_T (void)
{}

template<typename S, typename S_var>
ACE_INLINE
S *
TAO::Ret_Var_Size_Argument_T<S,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_var>
ACE_INLINE
S *
TAO::Ret_Var_Size_Argument_T<S,S_var>::retn (void)
{
  return this->x_._retn ();
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::In_Var_Size_SArgument_T<S>::In_Var_Size_SArgument_T (void)
{}

template<typename S>
const S &
TAO::In_Var_Size_SArgument_T<S>::arg (void) const
{
  return *this->x_;
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Inout_Var_Size_SArgument_T<S>::Inout_Var_Size_SArgument_T (void)
{}

template<typename S>
S &
TAO::Inout_Var_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Out_Var_Size_SArgument_T<S,S_var>::Out_Var_Size_SArgument_T (void)
{}

template<typename S, typename S_var>
S *&
TAO::Out_Var_Size_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Ret_Var_Size_SArgument_T<S,S_var>::Ret_Var_Size_SArgument_T (void)
{}

template<typename S, typename S_var>
ACE_INLINE
S *&
TAO::Ret_Var_Size_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

