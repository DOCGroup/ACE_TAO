// $Id$

#include "tao/Messaging_Policy_i.h"
#include "tao/Stub.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "tao/Messaging_Policy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, Messaging_Policy_i, "$Id$")

#if (TAO_HAS_ROUNDTRIP_TIMEOUT_POLICY == 1)

TAO_RelativeRoundtripTimeoutPolicy::TAO_RelativeRoundtripTimeoutPolicy (const TimeBase::TimeT& relative_expiry)
  :  relative_expiry_ (relative_expiry)
{
}

TAO_RelativeRoundtripTimeoutPolicy::TAO_RelativeRoundtripTimeoutPolicy (const TAO_RelativeRoundtripTimeoutPolicy &rhs)
  : Messaging::RelativeRoundtripTimeoutPolicy (),
    TAO_Local_RefCounted_Object (),
    relative_expiry_ (rhs.relative_expiry_)
{
}

TimeBase::TimeT
TAO_RelativeRoundtripTimeoutPolicy::relative_expiry (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_RelativeRoundtripTimeoutPolicy::policy_type (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE;
}

void
TAO_RelativeRoundtripTimeoutPolicy::hook (TAO_ORB_Core *orb_core,
                                          TAO_Stub *stub,
                                          int &has_timeout,
                                          ACE_Time_Value &time_value)
{
  CORBA::Policy_var policy = 0;

  if (stub == 0)
    policy = orb_core->stubless_relative_roundtrip_timeout ();
  else
    policy = stub->relative_roundtrip_timeout ();
   
  if (CORBA::is_nil (policy.in ()))
    {
      has_timeout = 0;
      return;
    }
  ACE_TRY_NEW_ENV
    {
      Messaging::RelativeRoundtripTimeoutPolicy_var p =
        Messaging::RelativeRoundtripTimeoutPolicy::_narrow (policy.in (),
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TimeBase::TimeT t = p->relative_expiry (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      TimeBase::TimeT seconds = t / 10000000u;
      TimeBase::TimeT microseconds = (t % 10000000u) / 10;
      time_value.set (ACE_U64_TO_U32 (seconds),
                      ACE_U64_TO_U32 (microseconds));

      // Set the flag once all operations complete successfully
      has_timeout = 1;

      if (TAO_debug_level > 0)
        {
          CORBA::ULong msecs =
            ACE_static_cast(CORBA::ULong, microseconds / 1000);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Timeout is <%u>\n"),
                      msecs));
        }
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // Ignore all exceptions...
    }
  ACE_ENDTRY;
}

CORBA::Policy_ptr
TAO_RelativeRoundtripTimeoutPolicy::create (const CORBA::Any& val,
                                            CORBA::Environment &ACE_TRY_ENV)
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
TAO_RelativeRoundtripTimeoutPolicy::copy (CORBA_Environment &ACE_TRY_ENV)
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
TAO_RelativeRoundtripTimeoutPolicy::destroy (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
      CORBA::ULong msecs =
        ACE_static_cast(CORBA::ULong, microseconds / 1000);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Timeout is <%u>\n"),
                  msecs));
    }
}

#endif /* TAO_HAS_ROUNDTRIP_TIMEOUT_POLICY == 1 */

// ****************************************************************

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

TAO_Sync_Scope_Policy::TAO_Sync_Scope_Policy (Messaging::SyncScope synchronization)
  : synchronization_ (synchronization)
{
}

TAO_Sync_Scope_Policy::TAO_Sync_Scope_Policy (const TAO_Sync_Scope_Policy &rhs)
  : Messaging::SyncScopePolicy (),
    TAO_Local_RefCounted_Object (),
    synchronization_ (rhs.synchronization_)
{
}

CORBA::PolicyType
TAO_Sync_Scope_Policy::policy_type (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Messaging::SYNC_SCOPE_POLICY_TYPE;
}

void
TAO_Sync_Scope_Policy::hook (TAO_ORB_Core *orb_core,
                             TAO_Stub *stub,
                             int &has_synchronization,
                             int &scope)
{
  CORBA::Policy_var policy = 0;

  if (stub == 0)
    orb_core->stubless_sync_scope (policy);
  else
    policy = stub->sync_scope ();

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
                CORBA_SystemException::_tao_minor_code (
                  TAO_POLICY_NARROW_CODE,
                  0),
                CORBA::COMPLETED_NO));

      has_synchronization = 1;
      scope = p->synchronization ();
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // Ignore all exceptions...
    }
  ACE_ENDTRY;
}

CORBA::Policy_ptr
TAO_Sync_Scope_Policy::create (const CORBA::Any& val,
                               CORBA::Environment &ACE_TRY_ENV)
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
TAO_Sync_Scope_Policy::synchronization (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->synchronization_;
}

CORBA::Policy_ptr
TAO_Sync_Scope_Policy::copy (CORBA_Environment &ACE_TRY_ENV)
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
TAO_Sync_Scope_Policy::destroy (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
