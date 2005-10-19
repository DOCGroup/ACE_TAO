// $Id$

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::In_UB_String_Argument_T<S,Insert_Policy>::In_UB_String_Argument_T (const S * x)
  : x_ (x)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
const S *
TAO::In_UB_String_Argument_T<S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::Inout_UB_String_Argument_T<S,Insert_Policy>::Inout_UB_String_Argument_T (S *& x)
  : x_ (x)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
S *&
TAO::Inout_UB_String_Argument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_out, typename Insert_Policy>
ACE_INLINE
TAO::Out_UB_String_Argument_T<S,S_out,Insert_Policy>::Out_UB_String_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S, typename S_out, typename Insert_Policy>
ACE_INLINE
S *&
TAO::Out_UB_String_Argument_T<S,S_out,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_var, typename Insert_Policy>
ACE_INLINE
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::Ret_UB_String_Argument_T (void)
{}

template<typename S, typename S_var, typename Insert_Policy>
ACE_INLINE
S *&
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

template<typename S, typename S_var, typename Insert_Policy>
ACE_INLINE
S *
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_var, typename Insert_Policy>
ACE_INLINE
S *
TAO::Ret_UB_String_Argument_T<S,S_var,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

