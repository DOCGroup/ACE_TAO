// $Id$

#include "IdAssignmentStrategyUser.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    User_Id_Assignment_Strategy::~User_Id_Assignment_Strategy()
    {
    }

    char
    User_Id_Assignment_Strategy::id_assignment_key_type (void) const
    {
      return 'U';
    }

    bool
    User_Id_Assignment_Strategy::has_system_id (void) const
    {
      return false;
    }
  }
}

