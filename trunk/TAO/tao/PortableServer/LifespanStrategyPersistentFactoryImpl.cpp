// $Id$

#include "LifespanStrategyPersistentFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "LifespanStrategyPersistent.h"

ACE_RCSID (PortableServer,
           LifespanStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy*
    LifespanStrategyPersistentFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      LifespanStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
          ACE_NEW_RETURN (strategy, LifespanStrategyPersistent, 0);
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in LifespanStrategyPersistentFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    LifespanStrategyPersistentFactoryImpl::destroy (
      LifespanStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        LifespanStrategyPersistentFactoryImpl,
        ACE_TEXT ("LifespanStrategyPersistentFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanStrategyPersistentFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanStrategyPersistentFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanStrategyPersistentFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanStrategyPersistentFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

