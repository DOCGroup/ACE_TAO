#include "RequestProcessingPolicyValue.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           RequestProcessingPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicyValue::~RequestProcessingPolicyValue (void)
    {
    }

    RequestProcessingPolicyValueDefaultServant::~RequestProcessingPolicyValueDefaultServant (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicyValueDefaultServant::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_DEFAULT_SERVANT;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingPolicyValueDefaultServant)

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingPolicyValueDefaultServant,
        ACE_TEXT ("RequestProcessingPolicyValueDefaultServant"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingPolicyValueDefaultServant),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    RequestProcessingPolicyValueServantManager::~RequestProcessingPolicyValueServantManager (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicyValueServantManager::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_SERVANT_MANAGER;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingPolicyValueServantManager)

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingPolicyValueServantManager,
        ACE_TEXT ("RequestProcessingPolicyValueServantManager"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingPolicyValueServantManager),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    RequestProcessingPolicyValueAOMOnly::~RequestProcessingPolicyValueAOMOnly (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicyValueAOMOnly::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingPolicyValueAOMOnly)

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingPolicyValueAOMOnly,
        ACE_TEXT ("RequestProcessingPolicyValueAOMOnly"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingPolicyValueAOMOnly),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingPolicyValue>;
    template class ACE_Dynamic_Service<RequestProcessingPolicyValueServantManager>;
    template class ACE_Dynamic_Service<RequestProcessingPolicyValueDefaultServant>;
    template class ACE_Dynamic_Service<RequestProcessingPolicyValueAOMOnly>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValue>
    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValueServantManager>
    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValueDefaultServant>
    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValueAOMOnly>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
