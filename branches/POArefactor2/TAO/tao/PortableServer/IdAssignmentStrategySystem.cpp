// $Id$

#include "IdAssignmentStrategySystem.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentStrategySystem::~IdAssignmentStrategySystem()
    {
    }

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
  }
}

