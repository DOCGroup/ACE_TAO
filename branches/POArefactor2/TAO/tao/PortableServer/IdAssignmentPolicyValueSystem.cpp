// $Id$
#include "IdAssignmentPolicyValueSystem.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicyValueSystem,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicyValueSystem::~IdAssignmentPolicyValueSystem (void)
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    IdAssignmentPolicyValueSystem::policy_type (void)
    {
      return ::PortableServer::SYSTEM_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentPolicyValueSystem)

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentPolicyValueSystem,
        ACE_TEXT ("IdAssignmentPolicyValueSystem"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentPolicyValueSystem),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<IdAssignmentPolicyValueSystem>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<IdAssignmentPolicyValueSystem>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
