// $Id$

#include "Id_Uniqueness_Strategy.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Id_Uniqueness_Strategy::~Id_Uniqueness_Strategy()
    {
    }

    void
    Id_Uniqueness_Strategy::strategy_init(TAO_POA *poa, CORBA::PolicyList *policy_list)
    {
      // dependent on type create the correct strategy.
    }

    Unique_Id_Uniqueness_Strategy::~Unique_Id_Uniqueness_Strategy()
    {
    }

    Multiple_Id_Uniqueness_Strategy::~Multiple_Id_Uniqueness_Strategy()
    {
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

