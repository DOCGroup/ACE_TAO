// $Id$

ACE_INLINE
TAO_Policy_Manager_Impl::TAO_Policy_Manager_Impl (void)
  :  count_ (0)
{
}

// ****************************************************************

ACE_INLINE
TAO_Policy_Manager::TAO_Policy_Manager (void)
{
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Manager::get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  return this->impl_.get_policy (policy, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_Policy_Current::TAO_Policy_Current (void)
{
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Current::get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  return this->impl_.get_policy (policy, ACE_TRY_ENV);
}

