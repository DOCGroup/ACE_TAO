// $Id$

template<typename S, typename S_forany>
ACE_INLINE
TAO::In_Fixed_Array_SArgument_T<S,S_forany>::In_Fixed_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
ACE_INLINE
const S &
TAO::In_Fixed_Array_SArgument_T<S,S_forany>::arg (void) const
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename S_forany>
ACE_INLINE
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::
Inout_Fixed_Array_SArgument_T (void)
{}

template<typename S, typename S_forany>
ACE_INLINE
S &
TAO::Inout_Fixed_Array_SArgument_T<S,S_forany>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S, typename S_forany>
ACE_INLINE
TAO::Out_Fixed_Array_SArgument_T<S,S_forany>::Out_Fixed_Array_SArgument_T (
    void
  )
{}

template<typename S, typename S_forany>
ACE_INLINE
S &
TAO::Out_Fixed_Array_SArgument_T<S,S_forany>::arg (void)
{
  return this->x_;
}

// ===========================================================================

template<typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::
Ret_Fixed_Array_SArgument_T (void)
{}

template<typename S_slice, typename S_var, typename S_forany>
ACE_INLINE
S_slice *&
TAO::Ret_Fixed_Array_SArgument_T<S_slice,S_var,S_forany>::arg (void)
{
  return this->x_.out ();
}
