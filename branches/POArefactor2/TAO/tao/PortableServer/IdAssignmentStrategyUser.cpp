// $Id$

#include "IdAssignmentStrategyUser.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentStrategyUser::~IdAssignmentStrategyUser()
    {
    }

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
  }
}

