// -*- C++ -*-

#include "tao/PortableServer/ImplicitActivationStrategy.h"

ACE_RCSID (PortableServer,
           ImplicitActivationStrategy,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL
