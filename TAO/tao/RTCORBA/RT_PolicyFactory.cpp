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
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    return TAO_PriorityModelPolicy::create (value
                                            ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::THREADPOOL_POLICY_TYPE)
    return TAO_ThreadpoolPolicy::create (value
                                         ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
    return TAO_ServerProtocolPolicy::create (value
                                             ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    return TAO_ClientProtocolPolicy::create (value
                                             ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE)
    return TAO_PrivateConnectionPolicy::create (value
                                                ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    return TAO_PriorityBandedConnectionPolicy::create (value
                                                       ACE_ENV_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
