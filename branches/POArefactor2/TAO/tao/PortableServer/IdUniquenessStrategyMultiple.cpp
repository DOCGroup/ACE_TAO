// $Id$

#include "IdUniquenessStrategyMultiple.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategyMultiple::~IdUniquenessStrategyMultiple()
    {
    }

    void
    IdUniquenessStrategyMultiple::strategy_init (
      TAO_POA */*poa*/
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      // dependent on type create the correct strategy.
    }

    void
    IdUniquenessStrategyMultiple::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    bool
    IdUniquenessStrategyMultiple::is_servant_activation_allowed (
      PortableServer::Servant /*servant*/,
      int &/*wait_occurred_restart_call*/)
    {
      // With the multiple id strategy we can always activate the servant
      // another time
      return true;
    }

    bool
    IdUniquenessStrategyMultiple::allow_multiple_activations (void) const
    {
      return true;
    }
  }
}

