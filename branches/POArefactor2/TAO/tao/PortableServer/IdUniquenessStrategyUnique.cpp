// $Id$

#include "IdUniquenessStrategyUnique.h"
#include "ServantRetentionStrategy.h"
#include "POA.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Unique_Id_Uniqueness_Strategy::Unique_Id_Uniqueness_Strategy (void) :
      poa_ (0)
    {
    }

    Unique_Id_Uniqueness_Strategy::~Unique_Id_Uniqueness_Strategy()
    {
    }

    void
    Unique_Id_Uniqueness_Strategy::strategy_init (TAO_POA *poa)
    {
      poa_ = poa;
    }

    bool
    Unique_Id_Uniqueness_Strategy::validate (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      // If the specified servant is
      // already in the Active Object Map, the ServantAlreadyActive
      // exception is raised.
      int result =
        this->poa_->active_policy_strategies().servant_retention_strategy()->is_servant_in_map (servant,
                                       wait_occurred_restart_call);

      if (result)
        {
          ACE_THROW_RETURN (PortableServer::POA::ServantAlreadyActive (),
                            false);
        }
      else if (wait_occurred_restart_call)
        {
          // We ended up waiting on a condition variable, the POA
          // state may have changed while we are waiting.  Therefore,
          // we need to restart this call.
          return false;
        }

      return true;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
