// $Id$

template<typename S_ptr, typename S_var>
ACE_INLINE
TAO::In_Object_SArgument_T<S_ptr,S_var>::In_Object_SArgument_T (void)
{}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr
TAO::In_Object_SArgument_T<S_ptr,S_var>::arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S_ptr, typename S_var>
ACE_INLINE
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::Inout_Object_SArgument_T (void)
{}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr&
TAO::Inout_Object_SArgument_T<S_ptr,S_var>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S_ptr, typename S_var, typename S_out>
ACE_INLINE
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::Out_Object_SArgument_T (
    void
  )
{}

template<typename S_ptr, typename S_var, typename S_out>
ACE_INLINE
S_out
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S_ptr, typename S_var>
ACE_INLINE
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::Ret_Object_SArgument_T (void)
{}

template<typename S_ptr, typename S_var>
ACE_INLINE
S_ptr &
TAO::Ret_Object_SArgument_T<S_ptr,S_var>::arg (void)
{
  return this->x_.out ();
}

