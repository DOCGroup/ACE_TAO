//$Id$
#include "orbsvcs/FaultTolerance/FT_ClientPolicy_i.h"

#include "tao/debug.h"
#include "orbsvcs/Log_Macros.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/FaultTolerance/FT_ClientPolicy_i.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TimeBase::TimeT
TAO_FT_Request_Duration_Policy::request_duration_policy_value (void)
{
  return this->request_duration_;
}

CORBA::PolicyType
TAO_FT_Request_Duration_Policy::policy_type (void)
{
  return FT::REQUEST_DURATION_POLICY;
}

CORBA::Policy_ptr
TAO_FT_Request_Duration_Policy::create (const CORBA::Any& val)
{
  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_FT_Request_Duration_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_FT_Request_Duration_Policy (value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

TAO_FT_Request_Duration_Policy *
TAO_FT_Request_Duration_Policy::clone (void) const
{
  TAO_FT_Request_Duration_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_FT_Request_Duration_Policy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
TAO_FT_Request_Duration_Policy::copy (void)
{
  TAO_FT_Request_Duration_Policy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, TAO_FT_Request_Duration_Policy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_FT_Request_Duration_Policy::destroy (void)
{
}

void
TAO_FT_Request_Duration_Policy::set_time_value (ACE_Time_Value &time_value)
{
  TimeBase::TimeT t = this->request_duration_;
  TimeBase::TimeT seconds = t / 10000000u;
  TimeBase::TimeT microseconds = (t % 10000000u) / 10;
  time_value.set (ACE_U64_TO_U32 (seconds),
                  ACE_U64_TO_U32 (microseconds));

  if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT (%P|%t) - Timeout is <%dms>\n"),
                  time_value.msec ()));
    }
}


/*****************************************************************/

FT::HeartbeatPolicyValue
TAO_FT_Heart_Beat_Policy::heartbeat_policy_value (void)
{
  FT::HeartbeatPolicyValue val;
  val.heartbeat = this->heartbeat_;
  val.heartbeat_interval = this->heartbeat_interval_;
  val.heartbeat_timeout = this->heartbeat_timeout_;

  return val;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Policy::create (const CORBA::Any& val)
{
  FT::HeartbeatPolicyValue *value = 0;
  if ((val >>= value) == 0)
    throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_FT_Heart_Beat_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_FT_Heart_Beat_Policy (value->heartbeat,
                                              value->heartbeat_interval,
                                              value->heartbeat_timeout),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}


CORBA::PolicyType
TAO_FT_Heart_Beat_Policy::policy_type (void)
{
  return FT::HEARTBEAT_POLICY;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Policy::copy (void)
{
  TAO_FT_Heart_Beat_Policy * tmp = 0;
  ACE_NEW_THROW_EX (tmp, TAO_FT_Heart_Beat_Policy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

TAO_FT_Heart_Beat_Policy *
TAO_FT_Heart_Beat_Policy::clone (void) const
{
  TAO_FT_Heart_Beat_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_FT_Heart_Beat_Policy (*this),
                  0);
  return copy;
}

void
TAO_FT_Heart_Beat_Policy::destroy (void)
{
}

void
TAO_FT_Heart_Beat_Policy::set_time_value (ACE_Time_Value &time_value,
                                          const TimeBase::TimeT &timebase)
{
  TimeBase::TimeT t = timebase;
  TimeBase::TimeT seconds = t / 10000000u;
  TimeBase::TimeT microseconds = (t % 10000000u) / 10;
  time_value.set (ACE_U64_TO_U32 (seconds),
                  ACE_U64_TO_U32 (microseconds));

  if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT (%P|%t) - Timeout is <%dms>\n"),
                  time_value.msec ()));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
