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
    ServantRetentionPolicyValueRetain::~ServantRetentionPolicyValueRetain (void)
    {
    }

    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionPolicyValueRetain::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::RETAIN;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionPolicyValueRetain)

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionPolicyValueRetain,
        ACE_TEXT ("ServantRetentionPolicyValueRetain"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionPolicyValueRetain),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionPolicyValueRetain>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionPolicyValueRetain>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
