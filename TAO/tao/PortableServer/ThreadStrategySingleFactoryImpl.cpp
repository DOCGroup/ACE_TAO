// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ThreadStrategySingleFactoryImpl.h"
#include "tao/PortableServer/ThreadStrategy.h"
#include "tao/PortableServer/ThreadStrategySingle.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in ThreadStrategySingleFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ThreadStrategySingleFactoryImpl::destroy (ThreadStrategy *strategy)
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  ThreadStrategySingleFactoryImpl,
  ACE_TEXT ("ThreadStrategySingleFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ThreadStrategySingleFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ThreadStrategySingleFactoryImpl,
  TAO::Portable_Server::ThreadStrategySingleFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
