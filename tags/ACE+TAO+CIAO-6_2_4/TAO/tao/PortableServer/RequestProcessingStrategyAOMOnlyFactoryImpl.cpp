// -*- C++ -*-
// $Id$

#include "tao/PortableServer/RequestProcessingStrategyAOMOnlyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy*
    RequestProcessingStrategyAOMOnlyFactoryImpl::create (
      ::PortableServer::RequestProcessingPolicyValue value,
      ::PortableServer::ServantRetentionPolicyValue /*srvalue*/)
    {
      RequestProcessingStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          ACE_NEW_RETURN (strategy, RequestProcessingStrategyAOMOnly, 0);
          break;
        }
        default :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyAOMOnlyFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    RequestProcessingStrategyAOMOnlyFactoryImpl::destroy (
      RequestProcessingStrategy *strategy
      )
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  RequestProcessingStrategyAOMOnlyFactoryImpl,
  ACE_TEXT ("RequestProcessingStrategyAOMOnlyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (RequestProcessingStrategyAOMOnlyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  RequestProcessingStrategyAOMOnlyFactoryImpl,
  TAO::Portable_Server::RequestProcessingStrategyAOMOnlyFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL
