// -*- C++ -*-
//
// $Id$

#include "FT_ClientPolicyFactory.h"
#include "FT_ClientPolicy_i.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PolicyC.h"

ACE_RCSID (FaultTolerance,
           FT_ClientPolicyFactory,
           "$Id$")


CORBA::Policy_ptr
TAO_FT_ClientPolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &val
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{

  if (type ==  FT::REQUEST_DURATION_POLICY)
    return TAO_FT_Request_Duration_Policy::create (val
                                                   ACE_ENV_ARG_PARAMETER);
  else if (type == FT::HEARTBEAT_POLICY)
    return TAO_FT_Heart_Beat_Policy::create (val
                                             ACE_ENV_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
