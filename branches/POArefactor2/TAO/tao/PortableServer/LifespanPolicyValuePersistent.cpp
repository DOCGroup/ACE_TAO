#include "LifespanPolicyValuePersistent.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           LifespanPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicyValuePersistent::~LifespanPolicyValuePersistent (void)
    {
    }

    ::PortableServer::LifespanPolicyValue
    LifespanPolicyValuePersistent::policy_type (void)
    {
      return ::PortableServer::PERSISTENT;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanPolicyValuePersistent)

    ACE_STATIC_SVC_DEFINE (
        LifespanPolicyValuePersistent,
        ACE_TEXT ("LifespanPolicyValuePersistent"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanPolicyValuePersistent),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanPolicyValuePersistent>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanPolicyValuePersistent>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

