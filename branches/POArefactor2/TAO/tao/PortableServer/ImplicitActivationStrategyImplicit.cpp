// -*- C++ -*-

#include "ImplicitActivationStrategyImplicit.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategyImplicit,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationStrategyImplicit::~ImplicitActivationStrategyImplicit (void)
    {
    }

    bool
    ImplicitActivationStrategyImplicit::allow_implicit_activation (void) const
    {
      return true;
    }
  }
}

