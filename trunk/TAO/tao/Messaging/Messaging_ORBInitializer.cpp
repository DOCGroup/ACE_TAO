// -*- C++ -*-

#include "Messaging_ORBInitializer.h"

#include "Messaging_Policy_i.h"
#include "Connection_Timeout_Policy_i.h"
#include "Messaging_PolicyFactory.h"
#include "ExceptionHolder_i.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/Valuetype/ValueFactory.h"

ACE_RCSID (Messaging,
           Messaging_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Messaging_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)
  TAO_ORB_Core::set_timeout_hook
    (TAO_RelativeRoundtripTimeoutPolicy::hook);
#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
  TAO_ORB_Core::set_sync_scope_hook (TAO_Sync_Scope_Policy::hook);
#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)
  TAO_ORB_Core::connection_timeout_hook
    (TAO_ConnectionTimeoutPolicy::hook);
#endif  /* TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1 */

}

void
TAO_Messaging_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->register_value_factory (info
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Messaging_ORBInitializer::register_value_factory (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Messaging_ORBInitializer::register_value_factory:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  TAO::ExceptionHolderFactory *base_factory = 0;
  ACE_NEW (base_factory,
           TAO::ExceptionHolderFactory);
  CORBA::ValueFactory_var factory = base_factory;

  tao_info->orb_core()->orb ()->register_value_factory (
        Messaging::ExceptionHolder::_tao_obv_static_repository_id (),
        base_factory
        ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
}
void
TAO_Messaging_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
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
  ACE_CHECK;


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
      ACE_TRY
        {
          info->register_policy_factory (*i,
                                         policy_factory.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
        {
          if (ex.minor () == (CORBA::OMGVMCID | 16))
            {
              // The factory is already there, it happens because the
              // magic initializer in Messaging.cpp registers with the
              // ORB multiple times.  This is an indication that we
              // should do no more work in this ORBInitializer.
              return;
            }
          ACE_RE_THROW;
        }
      ACE_CATCHANY
        {
          // Rethrow any other exceptions...
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
