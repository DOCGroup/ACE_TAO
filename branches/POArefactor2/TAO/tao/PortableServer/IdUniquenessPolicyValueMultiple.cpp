#include "IdUniquenessPolicyValueMultiple.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyValueMultiple::~IdUniquenessPolicyValueMultiple (void)
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessPolicyValueMultiple::policy_type (void)
    {
      return ::PortableServer::MULTIPLE_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessPolicyValueMultiple)

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessPolicyValueMultiple,
        ACE_TEXT ("IdUniquenessPolicyValueMultiple"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessPolicyValueMultiple),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<IdUniquenessPolicyValueMultiple>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyValueMultiple>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
