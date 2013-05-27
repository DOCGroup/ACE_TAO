// $Id$

#include "tao/PortableServer/LifespanStrategyFactoryImpl.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy*
    LifespanStrategyFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      LifespanStrategy *strategy = 0;
      const char *strategy_name = 0;

      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
          strategy_name = "LifespanStrategyPersistentFactory";
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          strategy_name = "LifespanStrategyTransientFactory";
          break;
        }
      }

      LifespanStrategyFactory *strategy_factory =
        ACE_Dynamic_Service<LifespanStrategyFactory>::instance (strategy_name);

      if (strategy_factory != 0)
        strategy = strategy_factory->create (value);
      else
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR, Unable to get %s\n"),
                    strategy_name));


      return strategy;
    }

    void
    LifespanStrategyFactoryImpl::destroy (LifespanStrategy *strategy)
    {
      switch (strategy->type ())
      {
        case ::PortableServer::PERSISTENT :
        {
          LifespanStrategyFactory *strategy_factory =
            ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyPersistentFactory");

          if (strategy_factory != 0)
            {
              strategy_factory->destroy (strategy);
            }
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          LifespanStrategyFactory *strategy_factory =
            ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyTransientFactory");

          if (strategy_factory != 0)
            {
              strategy_factory->destroy (strategy);
            }
          break;
        }
      }
    }
  }
}



ACE_STATIC_SVC_DEFINE (
  LifespanStrategyFactoryImpl,
  ACE_TEXT ("LifespanStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (LifespanStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  LifespanStrategyFactoryImpl,
  TAO::Portable_Server::LifespanStrategyFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL
