// $Id$

#include "ServantRetentionStrategyFactoryImpl.h"
#include "ServantRetentionStrategy.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ServantRetentionStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategyFactory *strategy_factory = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyRetainFactory");

          if (strategy_factory == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("Unable to get ")
                               ACE_TEXT ("ServantRetentionStrategyRetainFactory")),
                               0);

          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyNonRetainFactory");

          if (strategy_factory == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("Unable to get ")
                               ACE_TEXT ("ServantRetentionStrategyNonRetainFactory")),
                               0);

          break;
        }
      }

      return strategy_factory->create (value);
    }

    void
    ServantRetentionStrategyFactoryImpl::destroy (
      ServantRetentionStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      switch (strategy->type ())
      {
        case ::PortableServer::RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyRetainFactory");

          if (servantretention_strategy_factory != 0)
            {
              servantretention_strategy_factory->destroy (strategy ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

            }
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyNonRetainFactory");

          if (servantretention_strategy_factory != 0)
            {
              servantretention_strategy_factory->destroy (strategy ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          break;
        }
      }
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionStrategyFactoryImpl,
        ACE_TEXT ("ServantRetentionStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ServantRetentionStrategyFactoryImpl>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ServantRetentionStrategyFactoryImpl>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

