// $Id$

template<typename S>
ACE_INLINE
TAO::In_UB_String_Argument_T<S>::In_UB_String_Argument_T (const S * x)
  : x_ (x)
{}

template<typename S>
ACE_INLINE
const S *
TAO::In_UB_String_Argument_T<S>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Inout_UB_String_Argument_T<S>::Inout_UB_String_Argument_T (S *& x)
  : x_ (x)
{}

template<typename S>
ACE_INLINE
S *&
TAO::Inout_UB_String_Argument_T<S>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_out>
ACE_INLINE
TAO::Out_UB_String_Argument_T<S,S_out>::Out_UB_String_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S, typename S_out>
ACE_INLINE
S *&
TAO::Out_UB_String_Argument_T<S,S_out>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Ret_UB_String_Argument_T<S,S_var>::Ret_UB_String_Argument_T (void)
{}

template<typename S, typename S_var>
ACE_INLINE
S *&
TAO::Ret_UB_String_Argument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

template<typename S, typename S_var>
ACE_INLINE
S *
TAO::Ret_UB_String_Argument_T<S,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, typename S_var>
ACE_INLINE
S *
TAO::Ret_UB_String_Argument_T<S,S_var>::retn (void)
{
  return this->x_._retn ();
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::In_UB_String_SArgument_T<S,S_var>::In_UB_String_SArgument_T (void)
{}

template<typename S, typename S_var>
const S *
TAO::In_UB_String_SArgument_T<S,S_var>::arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Inout_UB_String_SArgument_T<S,S_var>::Inout_UB_String_SArgument_T (void)
{}

template<typename S, typename S_var>
ACE_INLINE
S *&
TAO::Inout_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Out_UB_String_SArgument_T<S,S_var>::Out_UB_String_SArgument_T (
    void
  )
{}

template<typename S, typename S_var>
ACE_INLINE
S *&
TAO::Out_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S, typename S_var>
ACE_INLINE
TAO::Ret_UB_String_SArgument_T<S,S_var>::Ret_UB_String_SArgument_T (void)
{}

template<typename S, typename S_var>
S *&
TAO::Ret_UB_String_SArgument_T<S,S_var>::arg (void)
{
  return this->x_.out ();
}

