#include "ServantRetentionPolicyValue.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ServantRetentionPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionPolicyValue::~ServantRetentionPolicyValue (void)
    {
    }

    Retain_Servant_Retention_Policy::~Retain_Servant_Retention_Policy (void)
    {
    }

    Non_Retain_Servant_Retention_Policy::~Non_Retain_Servant_Retention_Policy (void)
    {
    }

    ::PortableServer::ServantRetentionPolicyValue
    Non_Retain_Servant_Retention_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::NON_RETAIN;
    }

    ::PortableServer::ServantRetentionPolicyValue
    Retain_Servant_Retention_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::RETAIN;
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

    ACE_FACTORY_DEFINE (TAO_PortableServer, Non_Retain_Servant_Retention_Policy)

    ACE_STATIC_SVC_DEFINE (
        Non_Retain_Servant_Retention_Policy,
        ACE_TEXT ("Non_Retain_Servant_Retention_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Non_Retain_Servant_Retention_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionPolicyValue>;
    template class ACE_Dynamic_Service<Non_Retain_Servant_Retention_Policy>;
    template class ACE_Dynamic_Service<Retain_Servant_Retention_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionPolicyValue>
    #pragma instantiate ACE_Dynamic_Service<Non_Retain_Servant_Retention_Policy>
    #pragma instantiate ACE_Dynamic_Service<Retain_Servant_Retention_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
