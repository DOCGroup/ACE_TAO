// $Id$

#include "IdAssignmentStrategySystem.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategySystem::id_assignment_key_type (void) const
    {
      return 'S';
    }

    bool
    IdAssignmentStrategySystem::has_system_id (void) const
    {
      return true;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentStrategySystem)

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentStrategySystem,
        ACE_TEXT ("IdAssignmentStrategySystem"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentStrategySystem),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<IdAssignmentStrategySystem>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<IdAssignmentStrategySystem>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

