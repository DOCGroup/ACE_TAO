// -*- C++ -*-

#include "ImplicitActivationStrategyImplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyImplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Implicit_Activation_Strategy::~Implicit_Activation_Strategy (void)
    {
    }

    bool
    Implicit_Activation_Strategy::allow_implicit_activation (void) const
    {
      return true;
    }
  }
}

