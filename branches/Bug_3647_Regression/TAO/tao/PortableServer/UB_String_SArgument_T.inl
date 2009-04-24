// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL
