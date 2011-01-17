// $Id$

#include "tao/PortableServer/ThreadStrategyFactoryImpl.h"
#include "tao/PortableServer/ThreadStrategy.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategy*
    ThreadStrategyFactoryImpl::create (
      ::PortableServer::ThreadPolicyValue value)
    {
      ThreadStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
          ThreadStrategyFactory *strategy_factory =
            ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategySingleFactory");

          if (strategy_factory != 0)
            strategy = strategy_factory->create (value);
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("ERROR, Unable to get ")
                        ACE_TEXT ("ThreadStrategySingleFactory")));

          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          strategy =
            ACE_Dynamic_Service<ThreadStrategy>::instance ("ThreadStrategyORBControl");

          if (strategy == 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("ERROR, Unable to get ")
                        ACE_TEXT ("ThreadStrategyORBControl")));

          break;
        }
      }

      return strategy;
    }

    void
    ThreadStrategyFactoryImpl::destroy (
      ThreadStrategy *strategy
      )
    {
      switch (strategy->type ())
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
          ThreadStrategyFactory *strategy_factory =
            ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategySingleFactory");

          if (strategy_factory != 0)
            {
              strategy_factory->destroy (strategy);
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
  }
}


ACE_STATIC_SVC_DEFINE (
  ThreadStrategyFactoryImpl,
  ACE_TEXT ("ThreadStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ThreadStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ThreadStrategyFactoryImpl,
  TAO::Portable_Server::ThreadStrategyFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL
