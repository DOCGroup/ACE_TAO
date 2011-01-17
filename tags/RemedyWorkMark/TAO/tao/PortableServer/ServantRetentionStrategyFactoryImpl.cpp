// -*- C++ -*-
// $Id$

#include "tao/PortableServer/ServantRetentionStrategyFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategyFactory *strategy_factory = 0;
      const char *strategy_factory_name = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          strategy_factory_name = "ServantRetentionStrategyRetainFactory";
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          strategy_factory_name = "ServantRetentionStrategyNonRetainFactory";
          break;
        }
      }

      strategy_factory =
        ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance (strategy_factory_name);

      if (strategy_factory == 0)
        {
          if (TAO_debug_level > 1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) ERROR, Unable to get %s\n"),
                        strategy_factory_name));

          return 0;
        }

      return strategy_factory->create (value);
    }

    void
    ServantRetentionStrategyFactoryImpl::destroy (
      ServantRetentionStrategy *strategy)
    {
      const char *strategy_factory_name = 0;

      switch (strategy->type ())
      {
        case ::PortableServer::RETAIN :
        {
          strategy_factory_name = "ServantRetentionStrategyRetainFactory";
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          strategy_factory_name = "ServantRetentionStrategyNonRetainFactory";
          break;
        }
      }

      ServantRetentionStrategyFactory *servantretention_strategy_factory =
        ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance (strategy_factory_name);

      if (servantretention_strategy_factory != 0)
        {
          servantretention_strategy_factory->destroy (strategy);
        }
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  ServantRetentionStrategyFactoryImpl,
  ACE_TEXT ("ServantRetentionStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ServantRetentionStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ServantRetentionStrategyFactoryImpl,
  TAO::Portable_Server::ServantRetentionStrategyFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL

