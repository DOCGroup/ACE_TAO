// -*- C++ -*-

#include "ImplicitActivationStrategy.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    void
    ImplicitActivationStrategy::strategy_init (
      TAO_Root_POA * /*poa*/
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      // dependent on type create the correct strategy.
    }

    void
    ImplicitActivationStrategy::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }
  }
}

