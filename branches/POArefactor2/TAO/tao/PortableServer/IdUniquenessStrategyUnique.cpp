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
      servant_retention_strategy_ (0)
    {
    }

    Unique_Id_Uniqueness_Strategy::~Unique_Id_Uniqueness_Strategy()
    {
    }

    void
    Unique_Id_Uniqueness_Strategy::strategy_init (
      ServantRetentionStrategy* servant_retention_strategy)
    {
      servant_retention_strategy_ = servant_retention_strategy;
    }

    bool
    Unique_Id_Uniqueness_Strategy::is_servant_activation_allowed (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call)
    {
      // If the specified servant is already in the Active Object Map, the
      // ServantAlreadyActive exception is raised.
      int result =
        this->servant_retention_strategy_->is_servant_in_map (servant,
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
    Unique_Id_Uniqueness_Strategy::allow_multiple_activations (void) const
    {
      return false;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
