// -*- C++ -*-
//
// $Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_ptr,
         typename S_var,
         class Insert_Policy>
ACE_INLINE
S_ptr
TAO::In_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S_ptr,
         typename S_var,
         class Insert_Policy>
ACE_INLINE
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::
Inout_Object_SArgument_T (void)
{
}

template<typename S_ptr,
         typename S_var,
         class Insert_Policy>
ACE_INLINE
S_ptr&
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S_ptr,
         typename S_var,
         typename S_out,
         class Insert_Policy>
ACE_INLINE
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::
Out_Object_SArgument_T (void)
{
}

template<typename S_ptr,
         typename S_var,
         typename S_out,
         class Insert_Policy>
ACE_INLINE
S_out
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S_ptr,
         typename S_var,
         class Insert_Policy>
ACE_INLINE
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::
Ret_Object_SArgument_T (void)
{
}

template<typename S_ptr,
         typename S_var,
         class Insert_Policy>
ACE_INLINE
S_ptr &
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
