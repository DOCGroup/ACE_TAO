// $Id$

#include "IdAssignmentStrategySystem.h"

ACE_RCSID (PortableServer,
           Id_Assignment_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

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

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
