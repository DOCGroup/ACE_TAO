// -*- C++ -*-

#include "ImplicitActivationStrategyExplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyExplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyExplicit::allow_implicit_activation (void) const
    {
      return false;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationStrategyExplicit)

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationStrategyExplicit,
        ACE_TEXT ("ImplicitActivationStrategyExplicit"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationStrategyExplicit),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ImplicitActivationStrategyExplicit>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ImplicitActivationStrategyExplicit>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

