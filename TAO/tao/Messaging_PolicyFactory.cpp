// -*- C++ -*-
//
// $Id$

#include "tao/Messaging_PolicyFactory.h"

ACE_RCSID (tao, Messaging_PolicyFactory, "$Id$")

#if TAO_HAS_CORBA_MESSAGING == 1

#include "tao/PolicyC.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Buffering_Constraint_Policy.h"

CORBA::Policy_ptr
TAO_Messaging_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  switch (type)
    {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  case TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      return TAO_RelativeRoundtripTimeoutPolicy::create (
               value
               TAO_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */


#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

    case TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE:
      return TAO_Sync_Scope_Policy::create (value
                                            TAO_ENV_ARG_PARAMETER);

#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

    case TAO_BUFFERING_CONSTRAINT_POLICY_TYPE:
      return TAO_Buffering_Constraint_Policy::create (value
                                                      TAO_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

    case TAO_MESSAGING_REBIND_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
    case TAO_MESSAGING_ROUTING_POLICY_TYPE:
    case TAO_MESSAGING_MAX_HOPS_POLICY_TYPE:
    case TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE:
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY),
                        CORBA::Policy::_nil ());

    default:
      break;
    }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */
