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
    No_Implicit_Implicit_Activation_Policy::~No_Implicit_Implicit_Activation_Policy (void)
    {
    }

    ::PortableServer::ImplicitActivationPolicyValue
    No_Implicit_Implicit_Activation_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::NO_IMPLICIT_ACTIVATION;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, No_Implicit_Implicit_Activation_Policy)

    ACE_STATIC_SVC_DEFINE (
        No_Implicit_Implicit_Activation_Policy,
        ACE_TEXT ("No_Implicit_Implicit_Activation_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (No_Implicit_Implicit_Activation_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
