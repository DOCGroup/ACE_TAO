// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/RequestProcessingStrategyServantActivatorFI.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivator.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy*
    RequestProcessingStrategyServantActivatorFactoryImpl::create (
      ::PortableServer::RequestProcessingPolicyValue value,
      ::PortableServer::ServantRetentionPolicyValue srvalue)
    {
      RequestProcessingStrategy* strategy = 0;

      switch (value)
      {
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
              TAOLIB_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyServantActivatorFactoryImpl"));
              break;
            }
          }
          break;
        }
        default :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyServantActivatorFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    RequestProcessingStrategyServantActivatorFactoryImpl::destroy (
      RequestProcessingStrategy *strategy
      )
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  RequestProcessingStrategyServantActivatorFactoryImpl,
  ACE_TEXT ("RequestProcessingStrategyServantActivatorFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (RequestProcessingStrategyServantActivatorFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  RequestProcessingStrategyServantActivatorFactoryImpl,
  TAO::Portable_Server::RequestProcessingStrategyServantActivatorFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_MINIMUM_POA == 0 */
