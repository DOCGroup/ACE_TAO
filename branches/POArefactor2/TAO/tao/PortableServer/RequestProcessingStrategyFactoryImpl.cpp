// $Id$

#include "RequestProcessingStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           RequestProcessingStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyFactoryImpl::~RequestProcessingStrategyFactoryImpl (void)
    {
    }
    RequestProcessingStrategy*
    RequestProcessingStrategyFactoryImpl::create (
      ::PortableServer::RequestProcessingPolicyValue value)
    {
      RequestProcessingStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
          break;
        }
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingStrategyFactoryImpl,
        ACE_TEXT ("RequestProcessingStrategyFactoryImpl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

