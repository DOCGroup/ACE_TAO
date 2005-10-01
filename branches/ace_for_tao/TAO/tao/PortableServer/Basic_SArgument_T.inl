// -*- C++ -*-
//
// $Id$

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::In_Basic_SArgument_T<S,Insert_Policy>::In_Basic_SArgument_T (void)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
S
TAO::In_Basic_SArgument_T<S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

// ================================================================

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::Inout_Basic_SArgument_T<S,Insert_Policy>::Inout_Basic_SArgument_T (void)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
S &
TAO::Inout_Basic_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::Out_Basic_SArgument_T<S,Insert_Policy>::Out_Basic_SArgument_T (void)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
S &
TAO::Out_Basic_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S, typename Insert_Policy>
ACE_INLINE
TAO::Ret_Basic_SArgument_T<S,Insert_Policy>::Ret_Basic_SArgument_T (void)
{}

template<typename S, typename Insert_Policy>
ACE_INLINE
S &
TAO::Ret_Basic_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

