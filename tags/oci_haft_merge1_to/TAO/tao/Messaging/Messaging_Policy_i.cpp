// $Id$

#include "tao/Messaging/Messaging_Policy_i.h"
#include "tao/Stub.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Messaging_Policy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Messaging,
           Messaging_Policy_i,
           "$Id$")

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

TAO_RelativeRoundtripTimeoutPolicy::TAO_RelativeRoundtripTimeoutPolicy (
  const TimeBase::TimeT& relative_expiry)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , Messaging::RelativeRoundtripTimeoutPolicy ()
  , TAO_Local_RefCounted_Object ()
  , relative_expiry_ (relative_expiry)
{
}

TAO_RelativeRoundtripTimeoutPolicy::TAO_RelativeRoundtripTimeoutPolicy (
  const TAO_RelativeRoundtripTimeoutPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , Messaging::RelativeRoundtripTimeoutPolicy ()
  , TAO_Local_RefCounted_Object ()
  , relative_expiry_ (rhs.relative_expiry_)
{
}

TimeBase::TimeT
TAO_RelativeRoundtripTimeoutPolicy::relative_expiry (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_RelativeRoundtripTimeoutPolicy::policy_type (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE;
}

void
TAO_RelativeRoundtripTimeoutPolicy::hook (TAO_ORB_Core *orb_core,
                                          TAO_Stub *stub,
                                          int &has_timeout,
                                          ACE_Time_Value &time_value)
{
  CORBA::Policy_var policy =
    (stub == 0
     ? orb_core->stubless_relative_roundtrip_timeout ()
     : stub->relative_roundtrip_timeout ());

  if (CORBA::is_nil (policy.in ()))
    {
      has_timeout = 0;
      return;
    }
  ACE_TRY_NEW_ENV
    {
      Messaging::RelativeRoundtripTimeoutPolicy_var p =
        Messaging::RelativeRoundtripTimeoutPolicy::_narrow (
          policy.in ()
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT t = p->relative_expiry (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      TimeBase::TimeT seconds = t / 10000000u;
      TimeBase::TimeT microseconds = (t % 10000000u) / 10;
      time_value.set (ACE_U64_TO_U32 (seconds),
                      ACE_U64_TO_U32 (microseconds));

      // Set the flag once all operations complete successfully
      has_timeout = 1;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Timeout is <%u>\n"),
                      time_value.msec ()));
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions...
    }
  ACE_ENDTRY;
}

CORBA::Policy_ptr
TAO_RelativeRoundtripTimeoutPolicy::create (const CORBA::Any& val
                                            ACE_ENV_ARG_DECL)
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_RelativeRoundtripTimeoutPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_RelativeRoundtripTimeoutPolicy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

TAO_RelativeRoundtripTimeoutPolicy *
TAO_RelativeRoundtripTimeoutPolicy::clone (void) const
{
  TAO_RelativeRoundtripTimeoutPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_RelativeRoundtripTimeoutPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
TAO_RelativeRoundtripTimeoutPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO_RelativeRoundtripTimeoutPolicy* tmp;
  ACE_NEW_THROW_EX (tmp, TAO_RelativeRoundtripTimeoutPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_RelativeRoundtripTimeoutPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_Cached_Policy_Type
TAO_RelativeRoundtripTimeoutPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT;
}

void
TAO_RelativeRoundtripTimeoutPolicy::set_time_value (ACE_Time_Value &time_value)
{
  TimeBase::TimeT t = this->relative_expiry_;
  TimeBase::TimeT seconds = t / 10000000u;
  TimeBase::TimeT microseconds = (t % 10000000u) / 10;
  time_value.set (ACE_U64_TO_U32 (seconds),
                  ACE_U64_TO_U32 (microseconds));

  if (TAO_debug_level > 0)
    {
      CORBA::ULong msecs = time_value.msec ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Timeout is <%u>\n"),
                  msecs));
    }
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

// ****************************************************************

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

TAO_Sync_Scope_Policy::TAO_Sync_Scope_Policy (Messaging::SyncScope synchronization)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , Messaging::SyncScopePolicy ()
  , TAO_Local_RefCounted_Object ()
  , synchronization_ (synchronization)
{
}

TAO_Sync_Scope_Policy::TAO_Sync_Scope_Policy (const TAO_Sync_Scope_Policy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , Messaging::SyncScopePolicy ()
  , TAO_Local_RefCounted_Object ()
  , synchronization_ (rhs.synchronization_)
{
}

CORBA::PolicyType
TAO_Sync_Scope_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Messaging::SYNC_SCOPE_POLICY_TYPE;
}

void
TAO_Sync_Scope_Policy::hook (TAO_ORB_Core *orb_core,
                             TAO_Stub *stub,
                             int &has_synchronization,
                             Messaging::SyncScope &scope)
{
  CORBA::Policy_var policy =
    (stub == 0 ? orb_core->stubless_sync_scope () : stub->sync_scope ());

  if (CORBA::is_nil (policy.in ()))
    {
      has_synchronization = 0;
      return;
    }

  ACE_TRY_NEW_ENV
    {
      Messaging::SyncScopePolicy_var p =
        Messaging::SyncScopePolicy::_narrow (policy.in ());
      ACE_TRY_CHECK;

      if (CORBA::is_nil (p.in ()))
        ACE_THROW (CORBA::INTERNAL (
                CORBA::SystemException::_tao_minor_code (
                  TAO_POLICY_NARROW_CODE,
                  0),
                CORBA::COMPLETED_NO));

      has_synchronization = 1;
      scope = p->synchronization (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions...
    }
  ACE_ENDTRY;
}

CORBA::Policy_ptr
TAO_Sync_Scope_Policy::create (const CORBA::Any& val
                               ACE_ENV_ARG_DECL)
{
  Messaging::SyncScope synchronization;
  if ((val >>= synchronization) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Sync_Scope_Policy *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Sync_Scope_Policy (synchronization),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

TAO_Sync_Scope_Policy *
TAO_Sync_Scope_Policy::clone (void) const
{
  TAO_Sync_Scope_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_Sync_Scope_Policy (*this),
                  0);
  return copy;
}

Messaging::SyncScope
TAO_Sync_Scope_Policy::synchronization (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->synchronization_;
}

CORBA::Policy_ptr
TAO_Sync_Scope_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Sync_Scope_Policy *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Sync_Scope_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

void
TAO_Sync_Scope_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
TAO_Cached_Policy_Type
TAO_Sync_Scope_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_SYNC_SCOPE;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */
