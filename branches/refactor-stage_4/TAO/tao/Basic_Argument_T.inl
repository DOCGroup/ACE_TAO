// $Id$

template<typename S>
ACE_INLINE
TAO::In_Basic_Argument_T<S>::In_Basic_Argument_T (S const & x)
  : x_ (x)
{}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Inout_Basic_Argument_T<S>::Inout_Basic_Argument_T (S & x)
  : x_ (x)
{}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Out_Basic_Argument_T<S>::Out_Basic_Argument_T (S & x)
  : x_ (x)
{}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Ret_Basic_Argument_T<S>::Ret_Basic_Argument_T (void)
{
}

template<typename S>
ACE_INLINE
S
TAO::Ret_Basic_Argument_T<S>::excp (void)
{
  return this->x_;
}

template<typename S>
ACE_INLINE
S
TAO::Ret_Basic_Argument_T<S>::retn (void)
{
  return this->x_;
}

// ================================================================

template<typename S>
ACE_INLINE
TAO::In_Basic_SArgument_T<S>::In_Basic_SArgument_T (void)
{}

template<typename S>
S
TAO::In_Basic_SArgument_T<S>::arg (void) const
{
  return this->x_;
}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Inout_Basic_SArgument_T<S>::Inout_Basic_SArgument_T (void)
{}

template<typename S>
ACE_INLINE
S &
TAO::Inout_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Out_Basic_SArgument_T<S>::Out_Basic_SArgument_T (void)
{}

template<typename S>
ACE_INLINE
S &
TAO::Out_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S>
ACE_INLINE
TAO::Ret_Basic_SArgument_T<S>::Ret_Basic_SArgument_T (void)
{
}

template<typename S>
ACE_INLINE
S &
TAO::Ret_Basic_SArgument_T<S>::arg (void)
{
  return this->x_;
}

