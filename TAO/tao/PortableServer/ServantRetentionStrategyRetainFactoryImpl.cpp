// $Id$

#include "ServantRetentionStrategyRetainFactoryImpl.h"
#include "ServantRetentionStrategy.h"
#include "ServantRetentionStrategyRetain.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ServantRetentionStrategyRetainFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyRetainFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          ACE_NEW_RETURN (strategy, ServantRetentionStrategyRetain, 0);
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in ServantRetentionStrategyNonRetainFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ServantRetentionStrategyRetainFactoryImpl::destroy (
      ServantRetentionStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionStrategyRetainFactoryImpl,
        ACE_TEXT ("ServantRetentionStrategyRetainFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionStrategyRetainFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionStrategyRetainFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ServantRetentionStrategyRetainFactoryImpl>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ServantRetentionStrategyRetainFactoryImpl>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

