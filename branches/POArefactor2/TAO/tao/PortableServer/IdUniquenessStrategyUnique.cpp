// $Id$

#include "IdUniquenessStrategyUnique.h"
#include "ServantRetentionStrategy.h"
#include "POA.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategyUnique::IdUniquenessStrategyUnique (void) :
      poa_ (0)
    {
    }

    IdUniquenessStrategyUnique::~IdUniquenessStrategyUnique()
    {
    }

    void
    IdUniquenessStrategyUnique::strategy_init (
      TAO_POA *poa
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      poa_ = poa;
    }

    void
    IdUniquenessStrategyUnique::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      poa_ = 0;
    }

    bool
    IdUniquenessStrategyUnique::is_servant_activation_allowed (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call)
    {
      // If the specified servant is already in the Active Object Map, the
      // ServantAlreadyActive exception is raised.
      int result =
        this->poa_->is_servant_active (servant,
                                       wait_occurred_restart_call);

      if (result || wait_occurred_restart_call)
        {
          return false;
        }
      else
        {
          return true;
        }
    }

    bool
    IdUniquenessStrategyUnique::allow_multiple_activations (void) const
    {
      return false;
    }
  }
}

