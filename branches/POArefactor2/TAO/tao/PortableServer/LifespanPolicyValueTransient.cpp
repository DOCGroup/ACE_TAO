#include "LifespanPolicyValueTransient.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           LifespanPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Transient_Lifespan_Policy::~Transient_Lifespan_Policy (void)
    {
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

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<Transient_Lifespan_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<Transient_Lifespan_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

