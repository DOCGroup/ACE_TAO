// $Id$

#include "LifespanStrategyTransientFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "LifespanStrategyTransient.h"

ACE_RCSID (PortableServer,
           LifespanStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy*
    LifespanStrategyTransientFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      LifespanStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in LifespanStrategyTransientFactoryImpl"));
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          ACE_NEW_RETURN (strategy, LifespanStrategyTransient, 0);
          break;
        }
      }

      return strategy;
    }

    void
    LifespanStrategyTransientFactoryImpl::destroy (
      LifespanStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        LifespanStrategyTransientFactoryImpl,
        ACE_TEXT ("LifespanStrategyTransientFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanStrategyTransientFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanStrategyTransientFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanStrategyTransientFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanStrategyTransientFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

