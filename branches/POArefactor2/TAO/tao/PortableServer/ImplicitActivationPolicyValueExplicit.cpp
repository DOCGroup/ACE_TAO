#include "ImplicitActivationPolicyValueExplicit.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

ACE_RCSID (PortableServer,
           ImplicitActivationPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicyValueExplicit::~ImplicitActivationPolicyValueExplicit (void)
    {
    }

    ::PortableServer::ImplicitActivationPolicyValue
    ImplicitActivationPolicyValueExplicit::policy_type (void)
    {
      return ::PortableServer::NO_IMPLICIT_ACTIVATION;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationPolicyValueExplicit)

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationPolicyValueExplicit,
        ACE_TEXT ("ImplicitActivationPolicyValueExplicit"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationPolicyValueExplicit),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<ImplicitActivationPolicyValueExplicit>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ImplicitActivationPolicyValueExplicit>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
