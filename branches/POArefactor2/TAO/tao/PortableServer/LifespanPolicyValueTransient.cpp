#include "LifespanPolicyValueTransient.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           LifespanPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicyValueTransient::~LifespanPolicyValueTransient (void)
    {
    }

    ::PortableServer::LifespanPolicyValue
    LifespanPolicyValueTransient::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::TRANSIENT;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanPolicyValueTransient)

    ACE_STATIC_SVC_DEFINE (
        LifespanPolicyValueTransient,
        ACE_TEXT ("LifespanPolicyValueTransient"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanPolicyValueTransient),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanPolicyValueTransient>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanPolicyValueTransient>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

