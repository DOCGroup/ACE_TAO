// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Policy_Manager::TAO_Policy_Manager (void)
  : impl_ (TAO_POLICY_ORB_SCOPE)
{
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Manager::get_policy (
  CORBA::PolicyType policy
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->impl_.get_policy (policy ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Manager::get_cached_policy (TAO_Cached_Policy_Type type
                                       ACE_ENV_ARG_DECL)
{

  // @@ Previous code used a "double-checked locking hack" to check
  // if the policy was set before grabbing the lock to actually get
  // it, so that could save a lock operation.  This was removed, as
  // it is not as easy to do this anymore.

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);

  return this->impl_.get_cached_policy (type
                                        ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
