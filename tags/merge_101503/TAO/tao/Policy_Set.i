// $Id$

ACE_INLINE
TAO_Policy_Set::TAO_Policy_Set (TAO_Policy_Scope scope)
  : scope_ (scope)
{
  for (int i = 0; i < TAO_CACHED_POLICY_MAX_CACHED; i++)
    this->cached_policies_[i] = 0;
}

ACE_INLINE CORBA::Boolean
TAO_Policy_Set::compatible_scope (TAO_Policy_Scope policy_scope) const
{
  return ((unsigned int) policy_scope & (unsigned int) this->scope_);
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Set::get_policy_by_index (CORBA::ULong index)
{
  return CORBA::Policy::_duplicate (this->policy_list_[index]);
}

ACE_INLINE CORBA::ULong
TAO_Policy_Set::num_policies (void) const
{
  return this->policy_list_.length();
}
