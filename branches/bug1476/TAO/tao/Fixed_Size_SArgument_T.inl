// $Id$

template<typename S>
ACE_INLINE
TAO::In_Fixed_Size_SArgument_T<S>::In_Fixed_Size_SArgument_T (void)
{}

template<typename S>
ACE_INLINE
const S &
TAO::In_Fixed_Size_SArgument_T<S>::arg (void) const
{
  return *this->x_;
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Inout_Fixed_Size_SArgument_T<S>::Inout_Fixed_Size_SArgument_T (void)
{}

template<typename S>
S &
TAO::Inout_Fixed_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Out_Fixed_Size_SArgument_T<S>::Out_Fixed_Size_SArgument_T (void)
{}

template<typename S>
S &
TAO::Out_Fixed_Size_SArgument_T<S>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S>
ACE_INLINE
TAO::Ret_Fixed_Size_SArgument_T<S>::Ret_Fixed_Size_SArgument_T (void)
{}

template<typename S>
ACE_INLINE
S &
TAO::Ret_Fixed_Size_SArgument_T<S>::arg (void)
{
  return *this->x_;
}

