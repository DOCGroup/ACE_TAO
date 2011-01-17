// -*- C++ -*-
// $Id$

#include "orbsvcs/FaultTolerance/FT_ServerPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ServerPolicy_i.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_FT_ServerPolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &val)
{

  if (type == FT::HEARTBEAT_ENABLED_POLICY)
    return TAO_FT_Heart_Beat_Enabled_Policy::create (val);

  throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
