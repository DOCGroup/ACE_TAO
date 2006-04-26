// -*- C++ -*-
//
// $Id$

#include "orbsvcs/FaultTolerance/FT_ServerPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ServerPolicy_i.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PolicyC.h"


ACE_RCSID (FaultTolerance,
           FT_ServerPolicyFactory,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_FT_ServerPolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &val
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{

  if (type == FT::HEARTBEAT_ENABLED_POLICY)
    return TAO_FT_Heart_Beat_Enabled_Policy::create (val
                                                     ACE_ENV_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
