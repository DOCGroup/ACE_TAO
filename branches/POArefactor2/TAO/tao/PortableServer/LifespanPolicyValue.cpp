#include "LifespanPolicyValue.h"
#include "PortableServerC.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           LifespanPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicyValue::~LifespanPolicyValue (void)
    {
    }

    Transient_Lifespan_Policy::~Transient_Lifespan_Policy (void)
    {
    }

    Persistent_Lifespan_Policy::~Persistent_Lifespan_Policy (void)
    {
    }

    ::PortableServer::LifespanPolicyValue
    Persistent_Lifespan_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::PERSISTENT;
    }

    ::PortableServer::LifespanPolicyValue
    Transient_Lifespan_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::TRANSIENT;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Transient_Lifespan_Policy)

    ACE_STATIC_SVC_DEFINE (
        Transient_Lifespan_Policy,
        ACE_TEXT ("Transient_Lifespan_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Transient_Lifespan_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, Persistent_Lifespan_Policy)

    ACE_STATIC_SVC_DEFINE (
        Persistent_Lifespan_Policy,
        ACE_TEXT ("Persistent_Lifespan_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Persistent_Lifespan_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanPolicyValue>;
    template class ACE_Dynamic_Service<Persistent_Lifespan_Policy>;
    template class ACE_Dynamic_Service<Transient_Lifespan_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanPolicyValue>
    #pragma instantiate ACE_Dynamic_Service<Persistent_Lifespan_Policy>
    #pragma instantiate ACE_Dynamic_Service<Transient_Lifespan_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
