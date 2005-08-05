// $Id$

#include "IdAssignmentStrategyUser.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategyUser::id_assignment_key_type (void) const
    {
      return 'U';
    }

    bool
    IdAssignmentStrategyUser::has_system_id (void) const
    {
      return false;
    }

    ACE_FACTORY_DEFINE (ACE_Local_Service, IdAssignmentStrategyUser)

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentStrategyUser,
        ACE_TEXT ("IdAssignmentStrategyUser"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentStrategyUser),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )
  }
}

