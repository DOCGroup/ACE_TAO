//$Id$
#include "FT_Policy_i.h"


#if !defined (__ACE_INLINE__)
#include "FT_Policy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FaultTolerance, FT_Policy_i, "$Id$")

TimeBase::TimeT
TAO_FT_Request_Duration_Policy::request_duration_value (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_duration_;
}

CORBA::PolicyType
TAO_FT_Request_Duration_Policy::policy_type (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return FT::REQUEST_DURATION_POLICY;
}

CORBA::Policy_ptr
TAO_FT_Request_Duration_Policy::create (const CORBA::Any& val,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_FT_Request_Duration_Policy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_FT_Request_Duration_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

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
TAO_FT_Request_Duration_Policy::copy (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FT_Request_Duration_Policy* tmp;
  ACE_NEW_THROW_EX (tmp, TAO_FT_Request_Duration_Policy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_FT_Request_Duration_Policy::destroy (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
      CORBA::ULong msecs =
        ACE_static_cast(CORBA::ULong, microseconds / 1000);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Timeout is <%u>\n"),
                  msecs));
    }
}
