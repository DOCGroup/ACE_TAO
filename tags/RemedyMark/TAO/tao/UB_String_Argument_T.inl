// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_UB_String_Argument_T<S_var,Insert_Policy>::In_UB_String_Argument_T (
  const typename S_var::s_traits::char_type * x)
  : x_ (x)
{
}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
const typename S_var::s_traits::char_type *
TAO::In_UB_String_Argument_T<S_var,Insert_Policy>::arg (void) const
{
  return this->x_;
}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_UB_String_Clonable_Argument_T<S_var,Insert_Policy>::In_UB_String_Clonable_Argument_T (
  const typename S_var::s_traits::char_type * x)
  : In_UB_String_Argument_T<S_var,Insert_Policy> (x),
    is_clone_ (false)
{
}

// ==========================================================================

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_UB_String_Argument_T<S_var,Insert_Policy>::Inout_UB_String_Argument_T (
    typename S_var::s_traits::char_type *& x)
  : x_ (x)
{}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Inout_UB_String_Argument_T<S_var,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_UB_String_Argument_T<S_var,Insert_Policy>::Out_UB_String_Argument_T (
    typename S_var::s_traits::string_out & x)
  : x_ (x.ptr ())
{}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Out_UB_String_Argument_T<S_var,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::Ret_UB_String_Argument_T (void)
{
}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
