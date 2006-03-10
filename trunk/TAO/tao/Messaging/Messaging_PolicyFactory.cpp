#include "tao/Messaging/Messaging_PolicyFactory.h"

ACE_RCSID (Messaging,
           Messaging_PolicyFactory,
           "$Id$")

#include "tao/Messaging/Messaging_Policy_i.h"
#include "tao/Messaging/Connection_Timeout_Policy_i.h"

#include "tao/PolicyC.h"
#include "tao/Buffering_Constraint_Policy.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

CORBA::Policy_ptr
TAO_Messaging_PolicyFactory::create_buffering_constraint_policy (
    const CORBA::Any& val
    ACE_ENV_ARG_DECL)
{
  TAO::BufferingConstraint *buffering_constraint = 0;
  if ((val >>= buffering_constraint) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                      CORBA::Policy::_nil ());

  TAO_Buffering_Constraint_Policy *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Buffering_Constraint_Policy (*buffering_constraint),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

CORBA::Policy_ptr
TAO_Messaging_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)
  if (type == Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE)
    return TAO_RelativeRoundtripTimeoutPolicy::create (value
                                                       ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)
  if (type == TAO::CONNECTION_TIMEOUT_POLICY_TYPE)
    return TAO_ConnectionTimeoutPolicy::create (value
                                                ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
  if (type == Messaging::SYNC_SCOPE_POLICY_TYPE)
    return TAO_Sync_Scope_Policy::create (value
                                          ACE_ENV_ARG_PARAMETER);
#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
  if (type == TAO::BUFFERING_CONSTRAINT_POLICY_TYPE)
    return this->create_buffering_constraint_policy (value
                                                     ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  if (
#if (TAO_HAS_REBIND_POLICY == 1)
      type == Messaging::REBIND_POLICY_TYPE ||
#endif  /* TAO_HAS_REBIND_POLICY == 1 */
#if (TAO_HAS_PRIORITY_POLICIES == 1)
      type == Messaging::REQUEST_PRIORITY_POLICY_TYPE ||
      type == Messaging::REPLY_PRIORITY_POLICY_TYPE ||
#endif  /* TAO_HAS_PRIORITY_POLICIES == 1 */
#if (TAO_HAS_REQUEST_START_TIME_POLICY == 1)
      type == Messaging::REQUEST_START_TIME_POLICY_TYPE ||
#endif  /* TAO_HAS_REQUEST_START_TIME_POLICY == 1 */
#if (TAO_HAS_REQUEST_END_TIME_POLICY == 1)
      type == Messaging::REQUEST_END_TIME_POLICY_TYPE ||
#endif  /* TAO_HAS_REQUEST_END_TIME_POLICY == 1 */
#if (TAO_HAS_REPLY_START_TIME_POLICY == 1)
      type == Messaging::REPLY_START_TIME_POLICY_TYPE ||
#endif  /* TAO_HAS_REPLY_START_TIME_POLICY == 1 */
#if (TAO_HAS_REPLY_END_TIME_POLICY == 1)
      type == Messaging::REPLY_END_TIME_POLICY_TYPE ||
#endif  /* TAO_HAS_REPLY_END_TIME_POLICY == 1 */
#if (TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1)
      type == Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE ||
#endif  /* TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1 */
#if (TAO_HAS_ROUTING_POLICY == 1)
      type == Messaging::ROUTING_POLICY_TYPE ||
#endif  /* TAO_HAS_ROUTING_POLICY == 1 */
#if (TAO_HAS_MAX_HOPS_POLICY == 1)
      type == Messaging::MAX_HOPS_POLICY_TYPE ||
#endif  /* TAO_HAS_MAX_HOPS_POLICY == 1 */
      type == Messaging::QUEUE_ORDER_POLICY_TYPE)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY),
                      CORBA::Policy::_nil ());

  ACE_UNUSED_ARG (value);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
