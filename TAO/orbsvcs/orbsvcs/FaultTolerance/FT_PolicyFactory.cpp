// -*- C++ -*-
//
// $Id$

#include "FT_PolicyFactory.h"

ACE_RCSID (FaultTolerance, FT_PolicyFactory, "$Id$")

#include "tao/PolicyC.h"


CORBA::Policy_ptr
TAO_FT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  TAO_ENV_ARG_DEFN;

  if (type ==  FT::REQUEST_DURATION_POLICY)
    return  TAO_FT_Request_Duration_Policy::create (val,
                                                    ACE_TRY_ENV);
  else if (type == FT::HEARTBEAT_POLICY)
    return TAO_FT_Heart_Beat_Policy::create (val,
                                             ACE_TRY_ENV);
  else if (type == FT::HEARTBEAT_ENABLED_POLICY)
    return TAO_FT_Heart_Beat_Enabled_Policy::create (val,
                                                     ACE_TRY_ENV);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
