// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         class Insert_Policy>
ACE_INLINE
TAO::In_Var_Array_Argument_T<S_forany,Insert_Policy>::
In_Var_Array_Argument_T (const typename S_forany::_slice_type * x)
  : x_ (const_cast<typename S_forany::_slice_type *> (x))
{
}

template<typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type const *
TAO::In_Var_Array_Argument_T<S_forany,Insert_Policy>::arg (void) const
{
  return this->x_.in ();
}

template<typename S_forany,
         class Insert_Policy>
ACE_INLINE
TAO::In_Var_Array_Clonable_Argument_T<S_forany,Insert_Policy>::
In_Var_Array_Clonable_Argument_T (const typename S_forany::_slice_type * x)
  : In_Var_Array_Argument_T<S_forany,Insert_Policy> (x),
    is_clone_ (false)
{}

// ==========================================================================

template<typename S_forany,
         class Insert_Policy>
ACE_INLINE
TAO::Inout_Var_Array_Argument_T<S_forany,Insert_Policy>::
Inout_Var_Array_Argument_T (typename S_forany::_slice_type  *&x)
  : x_ (x)
{
}

template<typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Inout_Var_Array_Argument_T<S_forany,Insert_Policy>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S_out,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
TAO::Out_Var_Array_Argument_T<S_out,S_forany,Insert_Policy>::
Out_Var_Array_Argument_T (S_out x)
  : x_ (x.ptr ())
{
}

template<typename S_out,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *&
TAO::Out_Var_Array_Argument_T<S_out,S_forany,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::
Ret_Var_Array_Argument_T (void)
{}

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *&
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_var,
         typename S_forany,
         class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Ret_Var_Array_Argument_T<S_var,S_forany,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
