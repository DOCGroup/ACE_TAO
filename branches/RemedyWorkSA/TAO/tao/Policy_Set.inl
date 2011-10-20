// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_Policy_Set::compatible_scope (TAO_Policy_Scope policy_scope) const
{
  return
    ((static_cast<unsigned int> (policy_scope)
      & static_cast<unsigned int> (this->scope_)) > 0);
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Set::get_policy_by_index (CORBA::ULong index) const
{
  return CORBA::Policy::_duplicate (this->policy_list_[index]);
}

ACE_INLINE CORBA::ULong
TAO_Policy_Set::num_policies (void) const
{
  return this->policy_list_.length();
}

TAO_END_VERSIONED_NAMESPACE_DECL
