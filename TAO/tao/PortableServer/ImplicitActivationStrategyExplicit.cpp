// -*- C++ -*-
#include "tao/PortableServer/ImplicitActivationStrategyExplicit.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyExplicit::allow_implicit_activation () const
    {
      return false;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
