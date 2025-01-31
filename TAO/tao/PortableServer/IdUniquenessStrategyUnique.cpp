#include "tao/PortableServer/IdUniquenessStrategyUnique.h"
#include "tao/PortableServer/Root_POA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    IdUniquenessStrategyUnique::strategy_init (
      TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    IdUniquenessStrategyUnique::strategy_cleanup ()
    {
      poa_ = nullptr;
    }

    bool
    IdUniquenessStrategyUnique::is_servant_activation_allowed (
      PortableServer::Servant servant,
      bool &wait_occurred_restart_call)
    {
      // If the specified servant is already in the Active Object Map, the
      // ServantAlreadyActive exception is raised.
      int const result =
        this->poa_->is_servant_active (servant,
                                       wait_occurred_restart_call);

      return (!(result || wait_occurred_restart_call));
    }

    bool
    IdUniquenessStrategyUnique::allow_multiple_activations () const
    {
      return false;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

