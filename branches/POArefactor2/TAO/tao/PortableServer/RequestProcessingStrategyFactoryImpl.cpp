// $Id$

#include "RequestProcessingStrategyFactoryImpl.h"
#include "RequestProcessingStrategyAOMOnly.h"
#include "RequestProcessingStrategyDefaultServant.h"
#include "RequestProcessingStrategyServantLocator.h"
#include "RequestProcessingStrategyServantActivator.h"
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

    RequestProcessingStrategy*
    RequestProcessingStrategyFactoryImpl::create (
        ::PortableServer::RequestProcessingPolicyValue value,
        ::PortableServer::ServantRetentionPolicyValue srvalue)
    {
      RequestProcessingStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          ACE_NEW_RETURN (strategy, RequestProcessingStrategyAOMOnly, 0);
          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          ACE_NEW_RETURN (strategy, RequestProcessingStrategyDefaultServant, 0);
          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
          switch (srvalue)
          {
            case ::PortableServer::RETAIN :
            {
              ACE_NEW_RETURN (strategy, RequestProcessingStrategyServantActivator, 0);
              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
              ACE_NEW_RETURN (strategy, RequestProcessingStrategyServantLocator, 0);
              break;
            }
          }
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingStrategyFactoryImpl,
        ACE_TEXT ("RequestProcessingStrategyFactory"),
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

