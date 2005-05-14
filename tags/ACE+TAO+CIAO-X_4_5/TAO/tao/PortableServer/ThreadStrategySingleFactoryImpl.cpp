// $Id$

#include "ThreadStrategySingleFactoryImpl.h"
#include "ThreadStrategy.h"
#include "ThreadStrategySingle.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

ACE_RCSID (PortableServer,
           ThreadStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategy*
    ThreadStrategySingleFactoryImpl::create (
      ::PortableServer::ThreadPolicyValue value)
    {
      ThreadStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
          ACE_NEW_RETURN (strategy, ThreadStrategySingle, 0);
          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in ThreadStrategySingleFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ThreadStrategySingleFactoryImpl::destroy (
      ThreadStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        ThreadStrategySingleFactoryImpl,
        ACE_TEXT ("ThreadStrategySingleFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadStrategySingleFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadStrategySingleFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ThreadStrategySingleFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ThreadStrategySingleFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
