// $Id$

#include "ImplicitActivationStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ImplicitActivationStrategyImplicit.h"
#include "ImplicitActivationStrategyExplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

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
          ACE_NEW_RETURN (strategy, Implicit_Activation_Strategy, 0);
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          ACE_NEW_RETURN (strategy, Explicit_Activation_Strategy, 0);
          break;
        }
      }

      return strategy;
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

#endif /* TAO_HAS_MINIMUM_POA == 0 */

