// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_ptr,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::In_Object_Argument_T (S_ptr x)
  : x_ (x)
{}

template<typename S_ptr,
         template <typename> class Insert_Policy>
ACE_INLINE
S_ptr
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::arg () const
{
  return this->x_;
}

template<typename S_ptr,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Object_Clonable_Argument_T<S_ptr,Insert_Policy>::In_Object_Clonable_Argument_T (S_ptr x)
  : In_Object_Argument_T<S_ptr,Insert_Policy> (x),
    is_clone_ (false)
{}

// ==========================================================================

template<typename S_ptr,
         typename S_traits,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::Inout_Object_Argument_T (
    S_ptr & x
  )
  : x_ (x)
{}

template<typename S_ptr,
         typename S_traits,
         template <typename> class Insert_Policy>
ACE_INLINE
S_ptr &
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::arg ()
{
  return this->x_;
}

// ==========================================================================

template<typename S_ptr,
         typename S_out,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::Out_Object_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S_ptr,
         typename S_out,
         template <typename> class Insert_Policy>
ACE_INLINE
S_out
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::arg ()
{
  return this->x_;
}

// ==========================================================================

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::Ret_Object_Argument_T ()
{}

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
S_ptr &
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::arg ()
{
  return this->x_.out ();
}

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::excp ()
{
  return this->x_.ptr ();
}

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
ACE_INLINE
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::retn ()
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
