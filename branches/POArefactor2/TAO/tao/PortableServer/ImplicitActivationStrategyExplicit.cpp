// -*- C++ -*-

#include "ImplicitActivationStrategyExplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyExplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationStrategyExplicit::~ImplicitActivationStrategyExplicit (void)
    {
    }

    bool
    ImplicitActivationStrategyExplicit::allow_implicit_activation (void) const
    {
      return false;
    }
  }
}

