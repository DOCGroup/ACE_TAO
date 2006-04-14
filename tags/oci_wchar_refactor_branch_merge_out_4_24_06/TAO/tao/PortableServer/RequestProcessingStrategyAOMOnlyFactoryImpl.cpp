// $Id$

#include "RequestProcessingStrategyAOMOnlyFactoryImpl.h"
#include "RequestProcessingStrategy.h"
#include "RequestProcessingStrategyAOMOnly.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           RequestProcessingStrategyAOMOnlyFactoryImpl,
           "$Id$")

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
          ACE_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyAOMOnlyFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    RequestProcessingStrategyAOMOnlyFactoryImpl::destroy (
      RequestProcessingStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingStrategyAOMOnlyFactoryImpl,
        ACE_TEXT ("RequestProcessingStrategyAOMOnlyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingStrategyAOMOnlyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (ACE_Local_Service, RequestProcessingStrategyAOMOnlyFactoryImpl)
  }
}

