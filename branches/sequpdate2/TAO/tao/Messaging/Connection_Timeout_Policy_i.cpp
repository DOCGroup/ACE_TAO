//$Id$
#include "Connection_Timeout_Policy_i.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/AnyTypeCode/Any.h"

ACE_RCSID (Messaging,
           Connection_Timeout_Policy_i,
           "$Id$")


#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ConnectionTimeoutPolicy::TAO_ConnectionTimeoutPolicy (
  const TimeBase::TimeT& relative_expiry)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::CORBA::LocalObject ()
  , TAO::ConnectionTimeoutPolicy ()
  , TAO_Local_RefCounted_Object ()
  , relative_expiry_ (relative_expiry)
{
}

TAO_ConnectionTimeoutPolicy::TAO_ConnectionTimeoutPolicy (
  const TAO_ConnectionTimeoutPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::CORBA::LocalObject ()
  , TAO::ConnectionTimeoutPolicy ()
  , TAO_Local_RefCounted_Object ()
  , relative_expiry_ (rhs.relative_expiry_)
{
}

TimeBase::TimeT
TAO_ConnectionTimeoutPolicy::relative_expiry (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_ConnectionTimeoutPolicy::policy_type (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO::CONNECTION_TIMEOUT_POLICY_TYPE;
}

void
TAO_ConnectionTimeoutPolicy::hook (TAO_ORB_Core *orb_core,
                                   TAO_Stub *stub,
                                   bool &has_timeout,
                                   ACE_Time_Value &time_value)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Policy_var policy = 0;

      if (stub == 0)
        {
          policy =
            orb_core->get_cached_policy_including_current (
              TAO_CACHED_POLICY_CONNECTION_TIMEOUT
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          policy =
            stub->get_cached_policy (
              TAO_CACHED_POLICY_CONNECTION_TIMEOUT
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (CORBA::is_nil (policy.in ()))
        {
          has_timeout = 0;
          return;
        }

      TAO::ConnectionTimeoutPolicy_var p =
        TAO::ConnectionTimeoutPolicy::_narrow (
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
          CORBA::ULong msecs =
            static_cast<CORBA::ULong> (microseconds / 1000);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Timeout is <%u>\n"),
                      msecs));
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions...
    }
  ACE_ENDTRY;
}

CORBA::Policy_ptr
TAO_ConnectionTimeoutPolicy::create (const CORBA::Any& val
                                            ACE_ENV_ARG_DECL)
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                      CORBA::Policy::_nil ());

  TAO_ConnectionTimeoutPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ConnectionTimeoutPolicy (value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

TAO_ConnectionTimeoutPolicy *
TAO_ConnectionTimeoutPolicy::clone (void) const
{
  TAO_ConnectionTimeoutPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_ConnectionTimeoutPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
TAO_ConnectionTimeoutPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how the following code is
  // exception safe!
  TAO_ConnectionTimeoutPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ConnectionTimeoutPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_ConnectionTimeoutPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_Cached_Policy_Type
TAO_ConnectionTimeoutPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_CONNECTION_TIMEOUT;
}

void
TAO_ConnectionTimeoutPolicy::set_time_value (ACE_Time_Value &time_value)
{
  TimeBase::TimeT t = this->relative_expiry_;
  TimeBase::TimeT seconds = t / 10000000u;
  TimeBase::TimeT microseconds = (t % 10000000u) / 10;
  time_value.set (ACE_U64_TO_U32 (seconds),
                  ACE_U64_TO_U32 (microseconds));

  if (TAO_debug_level > 0)
    {
      CORBA::ULong const msecs = time_value.msec ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Timeout is <%u>\n"),
                  msecs));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */
