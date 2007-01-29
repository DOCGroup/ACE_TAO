//$Id$
#include "tao/Messaging/Connection_Timeout_Policy_i.h"
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
    void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_ConnectionTimeoutPolicy::policy_type (
    void)
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
  try
    {
      CORBA::Policy_var policy = 0;

      if (stub == 0)
        {
          policy =
            orb_core->get_cached_policy_including_current (
              TAO_CACHED_POLICY_CONNECTION_TIMEOUT
             );
        }
      else
        {
          policy =
            stub->get_cached_policy (
              TAO_CACHED_POLICY_CONNECTION_TIMEOUT
             );
        }

      if (CORBA::is_nil (policy.in ()))
        {
          has_timeout = 0;
          return;
        }

      TAO::ConnectionTimeoutPolicy_var p =
        TAO::ConnectionTimeoutPolicy::_narrow (
          policy.in ()
         );

      TimeBase::TimeT t = p->relative_expiry ();
      TimeBase::TimeT seconds = t / 10000000u;
      TimeBase::TimeT microseconds = (t % 10000000u) / 10;
      time_value.set (ACE_U64_TO_U32 (seconds),
                      ACE_U64_TO_U32 (microseconds));

      // Set the flag once all operations complete successfully
      has_timeout = 1;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Connect timeout is <%dms>\n"),
                      time_value.msec ()));
        }
    }
  catch (const ::CORBA::Exception&)
    {
      // Ignore all exceptions...
    }
}

CORBA::Policy_ptr
TAO_ConnectionTimeoutPolicy::create (const CORBA::Any& val
                                            )
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
TAO_ConnectionTimeoutPolicy::copy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how the following code is
  // exception safe!
  TAO_ConnectionTimeoutPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ConnectionTimeoutPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_ConnectionTimeoutPolicy::destroy (void)
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
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Connect timeout is <%dms>\n"),
                  time_value.msec ()));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */
