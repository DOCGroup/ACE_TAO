// -*- C++ -*-
//
// $Id$

#include "RT_PolicyFactory.h"

ACE_RCSID (tao, RT_PolicyFactory, "$Id$")

#include "tao/PolicyC.h"
#include "RT_Policy_i.h"

CORBA::Policy_ptr
TAO_RT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityModelPolicy,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  else if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityBandedConnectionPolicy,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  else if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_ClientProtocolPolicy,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
