// $Id$

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::In_Special_Basic_Argument_T (
     S const & x
  )
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S const &
TAO::In_Special_Basic_Argument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::
Inout_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S &
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::
Out_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S &
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::
Ret_Special_Basic_Argument_T (void)
{
}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S &
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::excp (void)
{
  return this->x_;
}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S>::retn (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::
In_Special_Basic_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S>::arg (void) const
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::
Inout_Special_Basic_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S &
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::
Out_Special_Basic_SArgument_T (void)
{}

template<typename S, typename to_S, typename from_S>
S &
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename to_S, typename from_S>
ACE_INLINE
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::
Ret_Special_Basic_SArgument_T (void)
{
}

template<typename S, typename to_S, typename from_S>
ACE_INLINE
S &
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S>::arg (void)
{
  return this->x_;
}

