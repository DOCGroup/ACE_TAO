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
    ImplicitActivationStrategyFactoryImpl::~ImplicitActivationStrategyFactoryImpl (void)
    {
    }
    ImplicitActivationStrategy*
    ImplicitActivationStrategyFactoryImpl::create (
      ::PortableServer::ImplicitActivationPolicyValue value)
    {
      ImplicitActivationStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::IMPLICIT_ACTIVATION :
        {
          strategy =
            ACE_Dynamic_Service<ImplicitActivationStrategy>::instance ("ImplicitActivationStrategyImplicit");

          if (strategy == 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("Unable to get ")
                        ACE_TEXT ("ImplicitActivationStrategyImplicit")));
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          strategy =
            ACE_Dynamic_Service<ImplicitActivationStrategy>::instance ("ImplicitActivationStrategyExplicit");

          if (strategy == 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("Unable to get ")
                        ACE_TEXT ("ImplicitActivationStrategyExplicit")));
          break;
        }
      }

      return strategy;
    }

    void
    ImplicitActivationStrategyFactoryImpl::destroy (
      ImplicitActivationStrategy * /*strategy*/
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      // Noop because both types are singletons
    }

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationStrategyFactoryImpl,
        ACE_TEXT ("ImplicitActivationStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ImplicitActivationStrategyFactoryImpl>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ImplicitActivationStrategyFactoryImpl>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

