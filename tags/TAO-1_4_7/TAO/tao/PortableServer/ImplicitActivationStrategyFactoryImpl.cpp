// $Id$

#include "ImplicitActivationStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ImplicitActivationStrategyImplicit.h"
#include "ImplicitActivationStrategyExplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationStrategy*
    ImplicitActivationStrategyFactoryImpl::create (
      ::PortableServer::ImplicitActivationPolicyValue value)
    {
      ImplicitActivationStrategy* strategy = 0;
      const char * strategy_name = 0;

      switch (value)
      {
        case ::PortableServer::IMPLICIT_ACTIVATION :
        {
          strategy_name = "ImplicitActivationStrategyImplicit";
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          strategy_name = "ImplicitActivationStrategyExplicit";
          break;
        }
      }

      strategy =
        ACE_Dynamic_Service<ImplicitActivationStrategy>::instance (strategy_name);

      if (strategy == 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR, Unable to get %s\n"),
                    strategy_name));

      return strategy;
    }

    void
    ImplicitActivationStrategyFactoryImpl::destroy (
      ImplicitActivationStrategy * /*strategy*/
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      // Noop because both types are singletons
    }

    ACE_FACTORY_DEFINE (ACE_Local_Service, ImplicitActivationStrategyFactoryImpl)

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationStrategyFactoryImpl,
        ACE_TEXT ("ImplicitActivationStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )
  }
}

