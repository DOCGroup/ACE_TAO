#include "ImplicitActivationPolicyValueImplicit.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ImplicitActivationPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicyValueImplicit::~ImplicitActivationPolicyValueImplicit (void)
    {
    }

    ::PortableServer::ImplicitActivationPolicyValue
    ImplicitActivationPolicyValueImplicit::policy_type (void)
    {
      return ::PortableServer::IMPLICIT_ACTIVATION;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationPolicyValueImplicit)

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationPolicyValueImplicit,
        ACE_TEXT ("ImplicitActivationPolicyValueImplicit"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationPolicyValueImplicit),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<ImplicitActivationPolicyValueImplicit>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ImplicitActivationPolicyValueImplicit>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
