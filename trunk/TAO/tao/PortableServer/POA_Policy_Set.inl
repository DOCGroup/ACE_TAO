// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::ULong
TAO_POA_Policy_Set::num_policies (void) const
{
  return this->impl_.num_policies ();
}

ACE_INLINE CORBA::Policy *
TAO_POA_Policy_Set::get_policy_by_index (CORBA::ULong index)
{
  return this->impl_.get_policy_by_index (index);
}

ACE_INLINE CORBA::Policy_ptr
TAO_POA_Policy_Set::get_cached_policy (TAO_Cached_Policy_Type type)
{
  return this->impl_.get_cached_policy (type);
}

ACE_INLINE void
TAO_POA_Policy_Set::merge_policies (const CORBA::PolicyList &policies)
{
  // Add the policies if they don't exist, override them if they do.
  this->impl_.set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
}

ACE_INLINE void
TAO_POA_Policy_Set::merge_policy (const CORBA::Policy_ptr policy)
{
  this->impl_.set_policy (policy);
}

ACE_INLINE CORBA::Policy_ptr
TAO_POA_Policy_Set::get_policy (CORBA::PolicyType policy)
{
  return this->impl_.get_policy (policy);
}

ACE_INLINE TAO_Policy_Set &
TAO_POA_Policy_Set::policies (void)
{
  return this->impl_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
