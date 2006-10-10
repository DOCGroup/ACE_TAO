// $Id$

#include "tao/PortableServer/ThreadStrategy.h"

ACE_RCSID (PortableServer,
           ThreadStrategy,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    ThreadStrategy::strategy_init (TAO_Root_POA * /*poa*/
				   ACE_ENV_ARG_DECL_NOT_USED)
    {
    }

    void
    ThreadStrategy::strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
