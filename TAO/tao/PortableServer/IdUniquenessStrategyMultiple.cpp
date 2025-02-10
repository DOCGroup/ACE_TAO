// -*- C++ -*-
#include "tao/PortableServer/IdUniquenessStrategyMultiple.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    IdUniquenessStrategyMultiple::strategy_init (TAO_Root_POA * /*poa*/)
    {
    }

    void
    IdUniquenessStrategyMultiple::strategy_cleanup ()
    {
    }

    bool
    IdUniquenessStrategyMultiple::is_servant_activation_allowed
      (PortableServer::Servant/*servant*/,
       bool &/*wait_occurred_restart_call*/)
    {
      // With the multiple id strategy we can always activate the servant
      // another time
      return true;
    }

    bool
    IdUniquenessStrategyMultiple::allow_multiple_activations () const
    {
      return true;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
