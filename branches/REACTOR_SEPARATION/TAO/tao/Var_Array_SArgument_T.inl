// $Id$


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

