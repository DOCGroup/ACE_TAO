// -*- C++ -*-
// $Id$

#include "tao/Messaging/Messaging_ORBInitializer.h"
#include "tao/Messaging/Messaging_Policy_i.h"
#include "tao/Messaging/Connection_Timeout_Policy_i.h"
#include "tao/Messaging/Messaging_PolicyFactory.h"
#include "tao/Messaging/ExceptionHolder_i.h"
#include "tao/Messaging/Messaging_Queueing_Strategies.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Queueing_Strategies.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/Valuetype/ValueFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Messaging_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
#if ((TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1) || \
     (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1) || \
     (TAO_HAS_SYNC_SCOPE_POLICY == 1))
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Messaging_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }
#else
  ACE_UNUSED_ARG (info);
#endif

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
  TAO::Transport_Queueing_Strategy* queuing_strategy = 0;
  ACE_NEW (queuing_strategy,
           TAO::Eager_Transport_Queueing_Strategy);
  tao_info->orb_core ()->set_eager_transport_queueing_strategy (queuing_strategy);

  ACE_NEW (queuing_strategy,
           TAO::Delayed_Transport_Queueing_Strategy);
  tao_info->orb_core ()->set_delayed_transport_queueing_strategy (queuing_strategy);
#endif

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)
  tao_info->orb_core ()->set_timeout_hook (TAO_RelativeRoundtripTimeoutPolicy::hook);
#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
  tao_info->orb_core ()->set_sync_scope_hook (TAO_Sync_Scope_Policy::hook);
#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)
  TAO_ORB_Core::connection_timeout_hook (TAO_ConnectionTimeoutPolicy::hook);
#endif  /* TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1 */

}

void
TAO_Messaging_ORBInitializer::post_init (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);

  this->register_value_factory (info);
}

void
TAO_Messaging_ORBInitializer::register_value_factory (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Messaging_ORBInitializer::register_value_factory:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  TAO::ExceptionHolderFactory *base_factory = 0;
  ACE_NEW (base_factory,
           TAO::ExceptionHolderFactory);
  CORBA::ValueFactory_var factory = base_factory;

  CORBA::ValueFactory_var old_factory =
    tao_info->orb_core()->orb ()->register_value_factory (
        Messaging::ExceptionHolder::_tao_obv_static_repository_id (),
        base_factory);
}
void
TAO_Messaging_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Register the Messaging policy factories.

  PortableInterceptor::PolicyFactory_ptr policy_factory_ptr;
  ACE_NEW_THROW_EX (policy_factory_ptr,
                    TAO_Messaging_PolicyFactory,
                      CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                          TAO::VMCID,
                          ENOMEM),
                        CORBA::COMPLETED_NO));


  PortableInterceptor::PolicyFactory_var policy_factory =
    policy_factory_ptr;

  // Bind the same policy factory to all Messaging related policy
  // types since a single policy factory is used to create each of
  // the different types of Messaging policies.

  CORBA::PolicyType type[] = {
#if (TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1)
    Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
#endif  /* TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1 */
#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
    Messaging::SYNC_SCOPE_POLICY_TYPE,
#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
#endif  /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
#if (TAO_HAS_REBIND_POLICY == 1)
    Messaging::REBIND_POLICY_TYPE,
#endif  /* TAO_HAS_REBIND_POLICY == 1 */
#if (TAO_HAS_PRIORITY_POLICIES == 1)
    Messaging::REQUEST_PRIORITY_POLICY_TYPE,
    Messaging::REPLY_PRIORITY_POLICY_TYPE,
#endif  /* TAO_HAS_PRIORITY_POLICIES == 1 */
#if (TAO_HAS_REQUEST_START_TIME_POLICY == 1)
    Messaging::REQUEST_START_TIME_POLICY_TYPE,
#endif  /* TAO_HAS_REQUEST_START_TIME_POLICY == 1 */
#if (TAO_HAS_REQUEST_END_TIME_POLICY == 1)
    Messaging::REQUEST_END_TIME_POLICY_TYPE,
#endif  /*TAO_HAS_REQUEST_END_TIME_POLICY == 1 */
#if (TAO_HAS_REPLY_START_TIME_POLICY == 1)
    Messaging::REPLY_START_TIME_POLICY_TYPE,
#endif  /* TAO_HAS_REPLY_START_TIME_POLICY == 1 */
#if (TAO_HAS_REPLY_END_TIME_POLICY == 1)
    Messaging::REPLY_END_TIME_POLICY_TYPE,
#endif  /* TAO_HAS_REPLY_END_TIME_POLICY == 1 */
#if (TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1)
    Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE,
#endif  /* TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY == 1 */
#if (TAO_HAS_ROUTING_POLICY == 1)
    Messaging::ROUTING_POLICY_TYPE,
#endif  /* TAO_HAS_ROUTING_POLICY == 1 */
#if (TAO_HAS_MAX_HOPS_POLICY == 1)
    Messaging::MAX_HOPS_POLICY_TYPE,
#endif  /* TAO_HAS_MAX_HOPS_POLICY == 1 */
    Messaging::QUEUE_ORDER_POLICY_TYPE,
#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)
    TAO::CONNECTION_TIMEOUT_POLICY_TYPE
#endif  /* TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1 */
  };

  const CORBA::PolicyType *end = type + sizeof (type) / sizeof (type[0]);
  for (CORBA::PolicyType *i = type; i != end; ++i)
    {
      try
        {
          info->register_policy_factory (*i, policy_factory.in ());
        }
      catch (const ::CORBA::BAD_INV_ORDER& ex)
        {
          if (ex.minor () == (CORBA::OMGVMCID | 16))
            {
              // The factory is already there, it happens because the
              // magic initializer in Messaging.cpp registers with the
              // ORB multiple times.  This is an indication that we
              // should do no more work in this ORBInitializer.
              return;
            }
          throw;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
