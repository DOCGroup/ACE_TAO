// -*- C++ -*-

#include "ImplicitActivationStrategy.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationStrategy::~ImplicitActivationStrategy (void)
    {
    }

    void
    ImplicitActivationStrategy::strategy_init (TAO_POA *)
    {
      // dependent on type create the correct strategy.
    }
  }
}

