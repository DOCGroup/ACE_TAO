// -*- C++ -*-

#include "Activation_Strategy.h"

ACE_RCSID (PortableServer,
           Activation_Strategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Activation_Strategy::~Activation_Strategy (void)
    {
    }

    void
    Activation_Strategy::strategy_init (TAO_POA *)
      {
        // dependent on type create the correct strategy.
      }

    Implicit_Activation_Strategy::~Implicit_Activation_Strategy (void)
    {
    }

    bool
    Implicit_Activation_Strategy::allow_implicit_activation (void) const
    {
      return true;
    }

    Explicit_Activation_Strategy::~Explicit_Activation_Strategy (void)
    {
    }

    bool
    Explicit_Activation_Strategy::allow_implicit_activation (void) const
    {
      return false;
    }
  }
}

