// -*- C++ -*-
//
// $Id$

#include "orbsvcs/FaultTolerance/FT_PolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_Policy_i.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PolicyC.h"


ACE_RCSID (FaultTolerance, FT_PolicyFactory, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_FT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &val)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{

  if (type ==  FT::REQUEST_DURATION_POLICY)
    return  TAO_FT_Request_Duration_Policy::create (val);
  else if (type == FT::HEARTBEAT_POLICY)
    return TAO_FT_Heart_Beat_Policy::create (val);
  else if (type == FT::HEARTBEAT_ENABLED_POLICY)
    return TAO_FT_Heart_Beat_Enabled_Policy::create (val);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
