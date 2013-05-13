// -*- C++ -*-
// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/RequestProcessingStrategyServantLocatorFI.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocator.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy*
    RequestProcessingStrategyServantLocatorFactoryImpl::create (
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
              TAOLIB_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyServantLocatorFactoryImpl"));
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
        default :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyServantLocatorFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    RequestProcessingStrategyServantLocatorFactoryImpl::destroy (
      RequestProcessingStrategy *strategy)
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  RequestProcessingStrategyServantLocatorFactoryImpl,
  ACE_TEXT ("RequestProcessingStrategyServantLocatorFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (RequestProcessingStrategyServantLocatorFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  RequestProcessingStrategyServantLocatorFactoryImpl,
  TAO::Portable_Server::RequestProcessingStrategyServantLocatorFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

