// -*- C++ -*-
//
// $Id$

#include "tao/Messaging_ORBInitializer.h"

ACE_RCSID (tao, Messaging_ORBInitializer, "$Id$")

#if TAO_HAS_CORBA_MESSAGING == 1

#include "tao/Messaging_PolicyFactory.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/ORB_Core.h"

void
TAO_Messaging_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORB_Core::set_timeout_hook
    (TAO_RelativeRoundtripTimeoutPolicy::hook);
  
  TAO_ORB_Core::set_sync_scope_hook
  (TAO_Sync_Scope_Policy::hook);

}

void
TAO_Messaging_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Messaging_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  TAO_ENV_ARG_DECL)
{
  /// Register the Messaging policy factories.
  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  TAO_ENV_ARG_DEFN;

  /// This policy factory is used for all Messaging related policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_Messaging_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;
  
  /// Bind the same policy factory to all Messaging related policy
  /// types since a single policy factory is used to create each of
  /// the different types of Messaging policies.

  CORBA::PolicyType type;
  
  type = TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  type = TAO_CLIENT_PRIORITY_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  type = TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  type = TAO_BUFFERING_CONSTRAINT_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
  
  type = TAO_MESSAGING_REBIND_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  

  type = TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_ROUTING_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_MAX_HOPS_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  type = TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  /// Transfer ownership of the policy factory to the registry.
  (void) policy_factory._retn ();
}

#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */
