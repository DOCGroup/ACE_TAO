// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
TAO::In_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::In_Special_Basic_Argument_T (
     S const & x
  )
  : x_ (x)
{}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S const &
TAO::In_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
TAO::In_Special_Basic_Clonable_Argument_T<S,to_S,from_S,Insert_Policy>::In_Special_Basic_Clonable_Argument_T (
     S const & x
  )
  : In_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy> (x),
    is_clone_ (false)
{}

// ===========================================================================

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::
Inout_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S &
TAO::Inout_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::
Out_Special_Basic_Argument_T (S & x)
  : x_ (x)
{}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S &
TAO::Out_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::
Ret_Special_Basic_Argument_T (void)
{
}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S &
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::arg (void)
{
  return this->x_;
}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::excp (void)
{
  return this->x_;
}

template<typename S,
         typename to_S,
         typename from_S,
         class Insert_Policy>
ACE_INLINE
S
TAO::Ret_Special_Basic_Argument_T<S,to_S,from_S,Insert_Policy>::retn (void)
{
  return this->x_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
