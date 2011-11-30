// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Basic_Argument_T<S, Insert_Policy>::In_Basic_Argument_T (S const & x)
  : x_ (x)
{}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S
TAO::In_Basic_Argument_T<S, Insert_Policy>::arg (void) const
{
  return this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Basic_Clonable_Argument_T<S,Insert_Policy>::In_Basic_Clonable_Argument_T (S const & x)
  : In_Basic_Argument_T<S,Insert_Policy> (x),
    is_clone_ (false)
{}

// ================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::Inout_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S &
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Basic_Argument_T<S, Insert_Policy>::Out_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S &
TAO::Out_Basic_Argument_T<S, Insert_Policy>::arg (void)
{
  return this->x_;
}

// ================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::Ret_Basic_Argument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S &
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::arg (void)
{
  return this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::excp (void)
{
  return this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::retn (void)
{
  return this->x_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
