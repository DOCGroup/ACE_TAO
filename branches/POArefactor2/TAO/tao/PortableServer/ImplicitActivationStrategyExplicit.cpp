// -*- C++ -*-

#include "ImplicitActivationStrategyExplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyExplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
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

