// $Id$

ACE_INLINE
TAO_Policy_Manager_Impl::TAO_Policy_Manager_Impl (void)
  : count_ (0)
{
#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  this->relative_roundtrip_timeout_ = 0;

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  this->client_priority_ = 0;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

  this->sync_scope_ = 0;

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  this->buffering_constraint_ = 0;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

ACE_INLINE TAO_RelativeRoundtripTimeoutPolicy *
TAO_Policy_Manager_Impl::relative_roundtrip_timeout (void) const
{
  return this->relative_roundtrip_timeout_;
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Manager_Impl::client_priority (void) const
{
  return this->client_priority_;
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

ACE_INLINE TAO_Sync_Scope_Policy *
TAO_Policy_Manager_Impl::sync_scope (void) const
{
  return this->sync_scope_;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Manager_Impl::buffering_constraint (void) const
{
  return this->buffering_constraint_;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

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

ACE_INLINE CORBA::PolicyList *
TAO_Policy_Manager::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
{
  return this->impl_.get_policy_overrides (ts, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Policy_Manager::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
{
  this->impl_.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

ACE_INLINE TAO_RelativeRoundtripTimeoutPolicy *
TAO_Policy_Manager::relative_roundtrip_timeout (void) const
{
  return this->impl_.relative_roundtrip_timeout ();
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Manager::client_priority (void) const
{
  return this->impl_.client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

ACE_INLINE TAO_Sync_Scope_Policy *
TAO_Policy_Manager::sync_scope (void) const
{
  return this->impl_.sync_scope ();
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Manager::buffering_constraint (void) const
{
  return this->impl_.buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

// ****************************************************************

ACE_INLINE
TAO_Policy_Current::TAO_Policy_Current (void)
{
}

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Current_Impl::get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  return this->manager_impl_.get_policy (policy, ACE_TRY_ENV);
}

ACE_INLINE CORBA::PolicyList *
TAO_Policy_Current_Impl::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
{
  return this->manager_impl_.get_policy_overrides (ts, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Policy_Current_Impl::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
{
  this->manager_impl_.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

ACE_INLINE TAO_RelativeRoundtripTimeoutPolicy *
TAO_Policy_Current_Impl::relative_roundtrip_timeout (void) const
{
  return this->manager_impl_.relative_roundtrip_timeout ();
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Current_Impl::client_priority (void) const
{
  return this->manager_impl_.client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

ACE_INLINE TAO_Sync_Scope_Policy *
TAO_Policy_Current_Impl::sync_scope (void) const
{
  return this->manager_impl_.sync_scope ();
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Current_Impl::buffering_constraint (void) const
{
  return this->manager_impl_.buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

// ****************************************************************

ACE_INLINE CORBA::Policy_ptr
TAO_Policy_Current::get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_policy (policy, ACE_TRY_ENV);
}

ACE_INLINE CORBA::PolicyList *
TAO_Policy_Current::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_policy_overrides (ts, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Policy_Current::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  impl.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

ACE_INLINE TAO_RelativeRoundtripTimeoutPolicy *
TAO_Policy_Current::relative_roundtrip_timeout (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.relative_roundtrip_timeout ();
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Current::client_priority (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

ACE_INLINE TAO_Sync_Scope_Policy *
TAO_Policy_Current::sync_scope (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.sync_scope ();
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Current::buffering_constraint (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
