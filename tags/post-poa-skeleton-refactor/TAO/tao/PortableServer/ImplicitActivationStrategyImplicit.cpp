// -*- C++ -*-

#include "ImplicitActivationStrategyImplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyImplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyImplicit::allow_implicit_activation (void) const
    {
      return true;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationStrategyImplicit)

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationStrategyImplicit,
        ACE_TEXT ("ImplicitActivationStrategyImplicit"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationStrategyImplicit),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ImplicitActivationStrategyImplicit>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ImplicitActivationStrategyImplicit>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

