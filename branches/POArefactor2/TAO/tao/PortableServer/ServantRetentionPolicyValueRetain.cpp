#include "ServantRetentionPolicyValueRetain.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ServantRetentionPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Retain_Servant_Retention_Policy::~Retain_Servant_Retention_Policy (void)
    {
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Retain_Servant_Retention_Policy)

    ACE_STATIC_SVC_DEFINE (
        Retain_Servant_Retention_Policy,
        ACE_TEXT ("Retain_Servant_Retention_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Retain_Servant_Retention_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<Retain_Servant_Retention_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<Retain_Servant_Retention_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
