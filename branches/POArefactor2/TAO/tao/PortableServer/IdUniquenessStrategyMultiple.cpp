// $Id$

#include "IdUniquenessStrategyMultiple.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Multiple_Id_Uniqueness_Strategy::~Multiple_Id_Uniqueness_Strategy()
    {
    }

    void
    Multiple_Id_Uniqueness_Strategy::strategy_init (TAO_POA * /*poa*/)
    {
      // dependent on type create the correct strategy.
    }

    bool
    Multiple_Id_Uniqueness_Strategy::is_servant_activation_allowed (
      PortableServer::Servant /*servant*/,
      int &/*wait_occurred_restart_call*/)
    {
      // With the multiple id strategy we can always activate the servant
      // another time
      return true;
    }

    bool
    Multiple_Id_Uniqueness_Strategy::allow_multiple_activations (void) const
    {
      return true;
    }
  }
}

