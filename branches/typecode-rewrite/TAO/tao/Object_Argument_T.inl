// -*- C++ -*-
//
// $Id$

template<typename S_ptr>
ACE_INLINE
TAO::In_Object_Argument_T<S_ptr>::In_Object_Argument_T (S_ptr x)
  : x_ (x)
{}

template<typename S_ptr>
ACE_INLINE
S_ptr
TAO::In_Object_Argument_T<S_ptr>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S_ptr, typename S_traits>
ACE_INLINE
TAO::Inout_Object_Argument_T<S_ptr,S_traits>::Inout_Object_Argument_T (
    S_ptr & x
  )
  : x_ (x)
{}

template<typename S_ptr, typename S_traits>
ACE_INLINE
S_ptr &
TAO::Inout_Object_Argument_T<S_ptr,S_traits>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_ptr, typename S_out>
ACE_INLINE
TAO::Out_Object_Argument_T<S_ptr,S_out>::Out_Object_Argument_T (S_out & x)
  : x_ (x.ptr ())
{}

template<typename S_ptr, typename S_out>
ACE_INLINE
S_out
TAO::Out_Object_Argument_T<S_ptr,S_out>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_ptr, typename S_var>
ACE_INLINE
TAO::Ret_Object_Argument_T<S_ptr,S_var>::Ret_Object_Argument_T (void)
{}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr &
TAO::Ret_Object_Argument_T<S_ptr,S_var>::arg (void)
{
  return this->x_.out ();
}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr
TAO::Ret_Object_Argument_T<S_ptr,S_var>::retn (void)
{
  return this->x_._retn ();
}

