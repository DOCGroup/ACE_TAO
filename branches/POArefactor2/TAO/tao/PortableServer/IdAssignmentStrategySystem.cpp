// $Id$

#include "IdAssignmentStrategySystem.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    System_Id_Assignment_Strategy::~System_Id_Assignment_Strategy()
    {
    }

    char
    System_Id_Assignment_Strategy::id_assignment_key_type (void) const
    {
      return 'S';
    }

    bool
    System_Id_Assignment_Strategy::has_system_id (void) const
    {
      return false;
    }
  }
}

