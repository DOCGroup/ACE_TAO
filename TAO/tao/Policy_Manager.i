// $Id$

ACE_INLINE
TAO_Policy_Manager_Impl::TAO_Policy_Manager_Impl (void)
  : count_ (0)
{
  this->relative_roundtrip_timeout_ = 0;

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  this->client_priority_ = 0;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  this->sync_scope_ = 0;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  this->buffering_constraint_ = 0;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  this->priority_model_ = 0;
  this->threadpool_ = 0;
  this->private_connection_ = 0;
  this->priority_banded_connection_ = 0;
  this->server_protocol_ = 0;
  this->client_protocol_ = 0;

#endif /* TAO_HAS_RT_CORBA == 1 */
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
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->impl_.get_policy (policy, ACE_TRY_ENV);
}

ACE_INLINE CORBA::PolicyList *
TAO_Policy_Manager::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
                       CORBA::SystemException
                       ))
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->impl_.get_policy_overrides (ts, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Policy_Manager::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
                       CORBA::SystemException,
                       CORBA_InvalidPolicies
                       ))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->impl_.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::relative_roundtrip_timeout (void)
{
  // Double-checked locking.
  if (this->impl_.relative_roundtrip_timeout_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.relative_roundtrip_timeout ();
    }
  return 0;
}

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Manager::client_priority (void)
{
  // Double-checked locking.
  if (this->impl_.client_priority_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.client_priority ();
   }
  return 0;
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::sync_scope (void)
{
  // Double-checked locking.
  if (this->impl_.sync_scope_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.sync_scope ();
    }
  return 0;
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Manager::buffering_constraint (void)
{
  // Double-checked locking.
  if (this->impl_.buffering_constraint_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.buffering_constraint ();
    }
  return 0;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::priority_model (void)
{
  // Double-checked locking.
  if (this->impl_.priority_model_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.priority_model ();
    }
  return 0;
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::threadpool (void)
{
  // Double-checked locking.
  if (this->impl_.threadpool_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.threadpool ();
    }
  return 0;
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::private_connection (void)
{
  // Double-checked locking.
  if (this->impl_.private_connection_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.private_connection ();
    }
  return 0;
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::priority_banded_connection (void)
{
  // Double-checked locking.
  if (this->impl_.priority_banded_connection_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.priority_banded_connection ();
    }
  return 0;
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::server_protocol (void)
{
  // Double-checked locking.
  if (this->impl_.server_protocol_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.server_protocol ();
    }
  return 0;
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Manager::client_protocol (void)
{
  // Double-checked locking.
  if (this->impl_.client_protocol_)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
      return this->impl_.client_protocol ();
    }
  return 0;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

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

ACE_INLINE CORBA::Policy *
TAO_Policy_Current_Impl::relative_roundtrip_timeout (void) const
{
  return this->manager_impl_.relative_roundtrip_timeout ();
}

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Current_Impl::client_priority (void) const
{
  return this->manager_impl_.client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

ACE_INLINE CORBA::Policy *
TAO_Policy_Current_Impl::sync_scope (void) const
{
  return this->manager_impl_.sync_scope ();
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Current_Impl::buffering_constraint (void) const
{
  return this->manager_impl_.buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE CORBA::Policy *
TAO_Policy_Current_Impl::private_connection (void) const
{
  return this->manager_impl_.private_connection ();
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Current_Impl::priority_banded_connection (void) const
{
  return this->manager_impl_.priority_banded_connection ();
}


ACE_INLINE CORBA::Policy *
TAO_Policy_Current_Impl::client_protocol (void) const
{
  return this->manager_impl_.client_protocol ();
}

#endif /* TAO_HAS_RT_CORBA == 1 */

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
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_policy_overrides (ts, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Policy_Current::set_policy_overrides (const CORBA::PolicyList & policies,
                                          CORBA::SetOverrideType set_add,
                                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA_InvalidPolicies))
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  impl.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Current::relative_roundtrip_timeout (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.relative_roundtrip_timeout ();
}

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_INLINE TAO_Client_Priority_Policy *
TAO_Policy_Current::client_priority (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.client_priority ();
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

ACE_INLINE CORBA::Policy *
TAO_Policy_Current::sync_scope (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.sync_scope ();
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_INLINE TAO_Buffering_Constraint_Policy *
TAO_Policy_Current::buffering_constraint (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.buffering_constraint ();
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE CORBA::Policy *
TAO_Policy_Current::private_connection (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.private_connection ();
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Current::priority_banded_connection (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.priority_banded_connection ();
}

ACE_INLINE CORBA::Policy *
TAO_Policy_Current::client_protocol (void) const
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.client_protocol ();
}

#endif /* TAO_HAS_RT_CORBA == 1 */
