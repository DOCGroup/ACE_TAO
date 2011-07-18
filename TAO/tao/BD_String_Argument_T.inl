// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
TAO::In_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::In_BD_String_Argument_T (
    const typename S_var::s_traits::char_type * x
  )
  : x_ (x)
{}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
const typename S_var::s_traits::char_type *
TAO::In_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::arg (void) const
{
  return this->x_;
}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
TAO::In_BD_String_Clonable_Argument_T<S_var,BOUND,Insert_Policy>::In_BD_String_Clonable_Argument_T (
    const typename S_var::s_traits::char_type * x
  )
  : In_BD_String_Argument_T<S_var,BOUND,Insert_Policy> (x),
    is_clone_ (false)
{}

// ==========================================================================

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
TAO::Inout_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::
Inout_BD_String_Argument_T (typename S_var::s_traits::char_type *& x)
  : x_ (x)
{}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Inout_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
TAO::Out_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::
Out_BD_String_Argument_T (typename S_var::s_traits::string_out x)
  : x_ (x.ptr ())
{}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Out_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::
Ret_BD_String_Argument_T (void)
{
}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

template<typename S_var,
         size_t BOUND,
         class Insert_Policy>
ACE_INLINE
typename S_var::s_traits::char_type *&
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
