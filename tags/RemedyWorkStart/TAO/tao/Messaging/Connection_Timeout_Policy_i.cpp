// -*- C++ -*-
// $Id$

#include "tao/Messaging/Connection_Timeout_Policy_i.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Truncate.h"

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ConnectionTimeoutPolicy::TAO_ConnectionTimeoutPolicy (
  const TimeBase::TimeT& relative_expiry)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , TAO::ConnectionTimeoutPolicy ()
  , ::CORBA::LocalObject ()
  , relative_expiry_ (relative_expiry)
{
}

TAO_ConnectionTimeoutPolicy::TAO_ConnectionTimeoutPolicy (
  const TAO_ConnectionTimeoutPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , TAO::ConnectionTimeoutPolicy ()
  , ::CORBA::LocalObject ()
  , relative_expiry_ (rhs.relative_expiry_)
{
}

TimeBase::TimeT
TAO_ConnectionTimeoutPolicy::relative_expiry (void)
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_ConnectionTimeoutPolicy::policy_type (void)
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
      CORBA::Policy_var policy = CORBA::Policy::_nil ();

      if (stub == 0)
        {
          policy =
            orb_core->get_cached_policy_including_current (
              TAO_CACHED_POLICY_CONNECTION_TIMEOUT);
        }
      else
        {
          policy =
            stub->get_cached_policy (TAO_CACHED_POLICY_CONNECTION_TIMEOUT);
        }

      if (CORBA::is_nil (policy.in ()))
        {
          has_timeout = false;
          return;
        }

      TAO::ConnectionTimeoutPolicy_var p =
        TAO::ConnectionTimeoutPolicy::_narrow (policy.in ());

      TimeBase::TimeT const t = p->relative_expiry ();
      TimeBase::TimeT const seconds = t / 10000000u;
      TimeBase::TimeT const microseconds = (t % 10000000u) / 10;
      time_value.set (ACE_Utils::truncate_cast<time_t> (seconds),
                      ACE_Utils::truncate_cast<suseconds_t> (microseconds));

      // Set the flag once all operations complete successfully
      has_timeout = true;

      if (TAO_debug_level > 0)
        {
          ACE_UINT64 msecs;
          const_cast<ACE_Time_Value const &> (time_value).msec (msecs);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Connect timeout is ")
                      ACE_TEXT ("%Q milliseconds\n"),
                      msecs));
        }
    }
  catch (const ::CORBA::Exception&)
    {
      // Ignore all exceptions...
    }
}

CORBA::Policy_ptr
TAO_ConnectionTimeoutPolicy::create (const CORBA::Any& val)
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_ConnectionTimeoutPolicy *tmp = 0;
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
  TimeBase::TimeT const t = this->relative_expiry_;
  TimeBase::TimeT const seconds = t / 10000000u;
  TimeBase::TimeT const microseconds = (t % 10000000u) / 10;
  time_value.set (ACE_Utils::truncate_cast<time_t> (seconds),
                  ACE_Utils::truncate_cast<suseconds_t> (microseconds));

  if (TAO_debug_level > 0)
    {
      ACE_UINT64 msecs;
      const_cast<ACE_Time_Value const &> (time_value).msec (msecs);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Connect timeout is ")
                  ACE_TEXT ("%Q milliseconds\n"),
                  msecs));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */
