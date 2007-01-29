// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Policy_Current_Impl::TAO_Policy_Current_Impl (void)
  : manager_impl_ (TAO_POLICY_THREAD_SCOPE)
{
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Current_Impl::get_policy (CORBA::PolicyType policy)
{
  return this->manager_impl_.get_policy (policy);
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Current_Impl::get_cached_policy (TAO_Cached_Policy_Type type)
{
  return this->manager_impl_.get_cached_policy (type);
}

TAO_END_VERSIONED_NAMESPACE_DECL
