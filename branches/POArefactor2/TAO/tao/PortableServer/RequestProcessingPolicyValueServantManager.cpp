#include "RequestProcessingPolicyValueServantManager.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           RequestProcessingPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicyValueServantManager::~RequestProcessingPolicyValueServantManager (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicyValueServantManager::policy_type (void)
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

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingPolicyValueServantManager>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValueServantManager>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
