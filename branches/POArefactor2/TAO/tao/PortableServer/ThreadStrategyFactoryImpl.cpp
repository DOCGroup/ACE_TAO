// $Id$

#include "ThreadStrategyFactoryImpl.h"
#include "ThreadStrategy.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ThreadStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategyFactoryImpl::~ThreadStrategyFactoryImpl (void)
    {
    }

    ThreadStrategy*
    ThreadStrategyFactoryImpl::create (
      ::PortableServer::ThreadPolicyValue value)
    {
      ThreadStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
          ThreadStrategyFactory *thread_strategy_factory =
            ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategySingleFactory");

          if (thread_strategy_factory == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic ThreadStrategyFactory Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_ThreadStrategySingleFactoryImpl()"));

              thread_strategy_factory =
                ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategySingleFactory");
            }

          if (thread_strategy_factory != 0)
            {
              strategy = thread_strategy_factory->create (value);
            }

          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          strategy =
            ACE_Dynamic_Service<ThreadStrategy>::instance ("ThreadStrategyORBControl");

          if (strategy == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic ThreadStrategy Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_ThreadStrategyORBControl()"));

              strategy =
                ACE_Dynamic_Service<ThreadStrategy>::instance ("ThreadStrategyORBControl");
            }
          break;
        }
      }

      return strategy;
    }

    void
    ThreadStrategyFactoryImpl::destroy (ThreadStrategy *strategy)
    {
      switch (strategy->type ())
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
          ThreadStrategyFactory *thread_strategy_factory =
            ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategySingleFactory");

          if (thread_strategy_factory != 0)
            {
              thread_strategy_factory->destroy (strategy);
            }
          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          // Noop
          break;
        }
      }
    }

    ACE_STATIC_SVC_DEFINE (
        ThreadStrategyFactoryImpl,
        ACE_TEXT ("ThreadStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ThreadStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ThreadStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
