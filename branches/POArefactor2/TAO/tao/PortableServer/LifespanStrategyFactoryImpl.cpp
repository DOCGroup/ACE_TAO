// $Id$

#include "LifespanStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "LifespanStrategyTransient.h"
#include "LifespanStrategyPersistent.h"

ACE_RCSID (PortableServer,
           LifespanStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategyFactoryImpl::~LifespanStrategyFactoryImpl (void)
    {
    }
    LifespanStrategy*
    LifespanStrategyFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      LifespanStrategy* strategy = 0;
// @todo make these loadable
      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
          ACE_NEW_RETURN (strategy, LifespanStrategyPersistent, 0);
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
    LifespanStrategyFactoryImpl::destroy (
      LifespanStrategy *strategy
      ACE_ENV_ARG_DECL)
    {
      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        LifespanStrategyFactoryImpl,
        ACE_TEXT ("LifespanStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

