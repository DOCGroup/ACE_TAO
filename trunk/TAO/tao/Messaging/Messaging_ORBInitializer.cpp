// -*- C++ -*-

#include "Messaging_ORBInitializer.h"



#include "Messaging_Policy_i.h"
#include "Messaging_PolicyFactory.h"
#include "tao/ORB_Core.h"

ACE_RCSID (Messaging, Messaging_ORBInitializer, "$Id$")

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
}

void
TAO_Messaging_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  // Register the Messaging policy factories.

  // The Messaging policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  if (CORBA::is_nil (this->policy_factory_.in ()))
    {
      PortableInterceptor::PolicyFactory_ptr policy_factory;
      ACE_NEW_THROW_EX (policy_factory,
                        TAO_Messaging_PolicyFactory,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
      ACE_CHECK;

      this->policy_factory_ = policy_factory;
    }

  // Bind the same policy factory to all Messaging related policy
  // types since a single policy factory is used to create each of
  // the different types of Messaging policies.

  CORBA::PolicyType type[] = {
    TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE
    , TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    , TAO_BUFFERING_CONSTRAINT_POLICY_TYPE
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
    , TAO_MESSAGING_REBIND_POLICY_TYPE
    , TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE
    , TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE
    , TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE
    , TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE
    , TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE
    , TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE
    , TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE
    , TAO_MESSAGING_ROUTING_POLICY_TYPE
    , TAO_MESSAGING_MAX_HOPS_POLICY_TYPE
    , TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE
  };

  for (CORBA::PolicyType *i = type;
       i != type + sizeof(type)/sizeof(type[0]);
       ++i)
    {
      ACE_TRY
        {
          info->register_policy_factory (*i,
                                         this->policy_factory_.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
        {
          if (ex.minor () == (TAO_OMG_VMCID | 16))
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


