// $Id$

#include "RequestProcessingStrategyDefaultServantFI.h"
#include "RequestProcessingStrategy.h"
#include "RequestProcessingStrategyDefaultServant.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           RequestProcessingStrategyDefaultServantFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyDefaultServantFactoryImpl::~RequestProcessingStrategyDefaultServantFactoryImpl (void)
    {
    }

    RequestProcessingStrategy*
    RequestProcessingStrategyDefaultServantFactoryImpl::create (
      ::PortableServer::RequestProcessingPolicyValue value,
      ::PortableServer::ServantRetentionPolicyValue /*srvalue*/)
    {
      RequestProcessingStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          ACE_NEW_RETURN (strategy, RequestProcessingStrategyDefaultServant, 0);
          break;
        }
        default :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in RequestProcessingStrategyDefaultServantFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    RequestProcessingStrategyDefaultServantFactoryImpl::destroy (
      RequestProcessingStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingStrategyDefaultServantFactoryImpl,
        ACE_TEXT ("RequestProcessingStrategyDefaultServantFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingStrategyDefaultServantFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingStrategyDefaultServantFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingStrategyDefaultServantFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingStrategyDefaultServantFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

