// $Id$

#include "orbsvcs/FaultTolerance/FT_ClientPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ClientPolicy_i.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_FT_ClientPolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &val)
{

  if (type ==  FT::REQUEST_DURATION_POLICY)
    return TAO_FT_Request_Duration_Policy::create (val);
  else if (type == FT::HEARTBEAT_POLICY)
    return TAO_FT_Heart_Beat_Policy::create (val);

  throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
