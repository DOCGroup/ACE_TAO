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
    ServantRetentionStrategyFactoryImpl::~ServantRetentionStrategyFactoryImpl (void)
    {
    }
    ServantRetentionStrategy*
    ServantRetentionStrategyFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyRetainFactory");

          if (servantretention_strategy_factory == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic ServantRetentionStrategyFactory Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_ServantRetentionStrategyRetainFactoryImpl()"));

              servantretention_strategy_factory =
                ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyRetainFactory");
            }

          if (servantretention_strategy_factory != 0)
            {
              strategy = servantretention_strategy_factory->create (value);
            }
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyNonRetainFactory");

          if (servantretention_strategy_factory == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic ServantRetentionStrategyFactory Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_ServantRetentionStrategyNonRetainFactoryImpl()"));

              servantretention_strategy_factory =
                ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyNonRetainFactory");
            }

          if (servantretention_strategy_factory != 0)
            {
              strategy = servantretention_strategy_factory->create (value);
            }
          break;
        }
      }

      return strategy;
    }

    void
    ServantRetentionStrategyFactoryImpl::destroy (ServantRetentionStrategy *strategy)
    {
      switch (strategy->type ())
      {
        case ::PortableServer::RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyRetainFactory");

          if (servantretention_strategy_factory != 0)
            {
              servantretention_strategy_factory->destroy (strategy);
            }
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          ServantRetentionStrategyFactory *servantretention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyNonRetainFactory");

          if (servantretention_strategy_factory != 0)
            {
              servantretention_strategy_factory->destroy (strategy);
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

