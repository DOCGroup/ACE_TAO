// $Id$

#include "tao/PortableServer/RequestProcessingStrategyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/RequestProcessingStrategyDefaultServant.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocator.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivator.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy*
    RequestProcessingStrategyFactoryImpl::create (
        ::PortableServer::RequestProcessingPolicyValue value,
        ::PortableServer::ServantRetentionPolicyValue srvalue)
    {
      RequestProcessingStrategy* strategy = 0;
      RequestProcessingStrategyFactory *strategy_factory = 0;

      switch (value)
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          strategy_factory =
            ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyAOMOnlyFactory");

          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          strategy_factory =
            ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyDefaultServantFactory");

          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
#if (TAO_HAS_MINIMUM_POA == 0)
          switch (srvalue)
          {
            case ::PortableServer::RETAIN :
            {
              strategy_factory =
                ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyServantActivatorFactory");

              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
              strategy_factory =
                ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyServantLocatorFactory");

              break;
            }
          }
#endif /* TAO_HAS_MINIMUM_POA == 0 */
          break;
        }
      }

      if (strategy_factory != 0)
        strategy = strategy_factory->create (value, srvalue);
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("ERROR, Unable to get ")
                    ACE_TEXT ("RequestProcessingStrategyFactory")));

      return strategy;
    }

    void
    RequestProcessingStrategyFactoryImpl::destroy (
      RequestProcessingStrategy *strategy
      )
    {
      RequestProcessingStrategyFactory *strategy_factory = 0;

      switch (strategy->type ())
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          strategy_factory =
            ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyAOMOnlyFactory");

          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          strategy_factory =
            ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyDefaultServantFactory");

          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
#if (TAO_HAS_MINIMUM_POA == 0)
          switch (strategy->sr_type ())
          {
            case ::PortableServer::RETAIN :
            {
              strategy_factory =
                ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyServantActivatorFactory");

              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
              strategy_factory =
                ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyServantLocatorFactory");
              break;
            }
          }
#endif /* TAO_HAS_MINIMUM_POA == 0 */
          break;
        }
      }

      if (strategy_factory != 0)
        {
          strategy_factory->destroy (strategy);
        }
    }
  }
}



ACE_STATIC_SVC_DEFINE (
  RequestProcessingStrategyFactoryImpl,
  ACE_TEXT ("RequestProcessingStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (RequestProcessingStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  RequestProcessingStrategyFactoryImpl,
  TAO::Portable_Server::RequestProcessingStrategyFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL
