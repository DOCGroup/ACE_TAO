// $Id$

#include "IdAssignmentStrategyUser.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

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
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
