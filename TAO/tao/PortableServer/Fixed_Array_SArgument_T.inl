// -*- C++ -*-
//
// $Id$

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::In_Fixed_Array_SArgument_T<S,
                                S_slice,
                                S_forany,
                                Insert_Policy>::In_Fixed_Array_SArgument_T (void)
{
}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice const *
TAO::In_Fixed_Array_SArgument_T<S,
                                S_slice,
                                S_forany,
                                Insert_Policy>::arg (void) const
{
  S_forany tmp (this->x_);
  return tmp.in ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::Inout_Fixed_Array_SArgument_T<S,
                                   S_slice,
                                   S_forany,
                                   Insert_Policy>::
Inout_Fixed_Array_SArgument_T (void)
{
}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice *
TAO::Inout_Fixed_Array_SArgument_T<S,
                                   S_slice,
                                   S_forany,
                                   Insert_Policy>::arg (void)
{
  S_forany tmp (this->x_);
  return tmp.inout ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::Out_Fixed_Array_SArgument_T<S,
                                 S_slice,
                                 S_forany,
                                 Insert_Policy>::
Out_Fixed_Array_SArgument_T (void)
{
}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice *
TAO::Out_Fixed_Array_SArgument_T<S,
                                 S_slice,
                                 S_forany,
                                 Insert_Policy>::arg (void)
{
  S_forany tmp (this->x_);
  return tmp.inout ();
}

// ===========================================================================

template<typename S_slice,
         typename S_var,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::Ret_Fixed_Array_SArgument_T<S_slice,
                                 S_var,
                                 S_forany,
                                 Insert_Policy>::
Ret_Fixed_Array_SArgument_T (void)
{
}

template<typename S_slice,
         typename S_var,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice *&
TAO::Ret_Fixed_Array_SArgument_T<S_slice,
                                 S_var,
                                 S_forany,
                                 Insert_Policy>::arg (void)
{
  return this->x_._retn_arg ();
}
