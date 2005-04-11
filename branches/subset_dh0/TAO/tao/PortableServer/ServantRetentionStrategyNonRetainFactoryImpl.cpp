// $Id$

#include "ServantRetentionStrategyNonRetainFactoryImpl.h"
#include "ServantRetentionStrategy.h"
#include "ServantRetentionStrategyNonRetain.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ServantRetentionStrategyNonRetainFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyNonRetainFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::NON_RETAIN :
        {
          ACE_NEW_RETURN (strategy, ServantRetentionStrategyNonRetain, 0);
          break;
        }
        case ::PortableServer::RETAIN :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in ServantRetentionStrategyNonRetainFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ServantRetentionStrategyNonRetainFactoryImpl::destroy (
      ServantRetentionStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionStrategyNonRetainFactoryImpl,
        ACE_TEXT ("ServantRetentionStrategyNonRetainFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionStrategyNonRetainFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionStrategyNonRetainFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionStrategyNonRetainFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionStrategyNonRetainFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
