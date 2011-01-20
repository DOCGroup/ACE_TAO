// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Var_Array_SArgument_T<S_forany,
                              Insert_Policy>::In_Var_Array_SArgument_T (void)
{}

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type const *
TAO::In_Var_Array_SArgument_T<S_forany,
                              Insert_Policy>::arg (void) const
{
  S_forany tmp (this->x_);
  return tmp.in ();
}

// ==========================================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Var_Array_SArgument_T<S_forany,
                                 Insert_Policy>::
Inout_Var_Array_SArgument_T (void)
{
}

template<typename S_forany,
         template <typename> class Insert_Policy>
typename S_forany::_slice_type *
TAO::Inout_Var_Array_SArgument_T<S_forany,
                                 Insert_Policy>::arg (void)
{
  S_forany tmp (this->x_);
  return tmp.inout ();
}

// ==========================================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::
Out_Var_Array_SArgument_T (void)
{
}

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
typename S_forany::_slice_type *&
TAO::Out_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::
Ret_Var_Array_SArgument_T (void)
{
}

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *&
TAO::Ret_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
