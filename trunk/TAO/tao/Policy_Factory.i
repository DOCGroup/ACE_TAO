// $Id$

#if (TAO_HAS_RT_CORBA == 1)

// @@ Angelo: we rarely include files in the .i file, and generally
// try to avoid that.
# include "tao/RT_Policy_i.h"
# include "tao/RTCORBAC.h"

#endif /* (TAO_HAS_RT_CORBA == 1) */

// @@ Angelo: it looks like this function is going to grow over time,
// should go into the .cpp file.
ACE_INLINE CORBA::Policy *
TAO_Policy_Factory::create_policy (CORBA::PolicyType ptype)
{
  CORBA::Policy_ptr policy = 0;

#if (TAO_HAS_RT_CORBA == 1)

  if (ptype == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    ACE_NEW_RETURN (policy, TAO_PriorityModelPolicy, 0);

  if (ptype == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    ACE_NEW_RETURN (policy, TAO_PriorityBandedConnectionPolicy, 0);

  if (ptype == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    ACE_NEW_RETURN (policy, TAO_ClientProtocolPolicy, 0);

#else
  ACE_UNUSED_ARG (ptype);

#endif /* (TAO_HAS_RT_CORBA == 1) */

  return policy;
}
