// $Id$

#include "IdUniquenessStrategy.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategy::~IdUniquenessStrategy()
    {
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
