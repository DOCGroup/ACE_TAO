// $Id$

#include "ThreadStrategyFactoryImpl.h"
#include "ThreadStrategySingle.h"
#include "ThreadStrategyORBControl.h"
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
          ACE_NEW_RETURN (strategy, Single_Thread_Strategy, 0);
          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          ACE_NEW_RETURN (strategy, ORBControl_Thread_Strategy, 0);
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        ThreadStrategyFactoryImpl,
        ACE_TEXT ("ThreadStrategyFactoryImpl"),
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

