#include "IdUniquenessPolicyValueUnique.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyValueUnique::~IdUniquenessPolicyValueUnique (void)
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessPolicyValueUnique::policy_type (void)
    {
      return ::PortableServer::UNIQUE_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessPolicyValueUnique)

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessPolicyValueUnique,
        ACE_TEXT ("IdUniquenessPolicyValueUnique"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessPolicyValueUnique),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<IdUniquenessPolicyValueUnique>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyValueUnique>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
