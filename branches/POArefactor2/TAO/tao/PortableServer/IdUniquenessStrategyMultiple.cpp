// $Id$

#include "IdUniquenessStrategyMultiple.h"

ACE_RCSID (PortableServer,
           Id_Uniqueness_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Multiple_Id_Uniqueness_Strategy::~Multiple_Id_Uniqueness_Strategy()
    {
    }

    void
    Multiple_Id_Uniqueness_Strategy::strategy_init (TAO_POA *)
    {
      // dependent on type create the correct strategy.
    }

    bool
    Multiple_Id_Uniqueness_Strategy::validate (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      ACE_UNUSED_ARG (servant);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      // With the multiple id strategy we can always activate the servant
      // another time
      return true;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
