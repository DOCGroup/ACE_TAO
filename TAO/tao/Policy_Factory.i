// $Id$

ACE_INLINE
CORBA::Policy * Policy_Factory::create_policy (CORBA::PolicyType ptype)
{
  CORBA::Policy_ptr policy = 0;

  if (ptype == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    ACE_NEW_RETURN(policy, TAO_PriorityModelPolicy, 0);

  if (ptype == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    ACE_NEW_RETURN(policy, TAO_PriorityBandedConnectionPolicy, 0);

  if (ptype == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
    ACE_NEW_RETURN(policy, TAO_ServerProtocolPolicy, 0);

  return policy;
}
