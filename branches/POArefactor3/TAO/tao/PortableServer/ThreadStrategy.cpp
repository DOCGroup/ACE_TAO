// $Id$

#include "ThreadStrategy.h"

ACE_RCSID (PortableServer,
           ThreadStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategy::~ThreadStrategy()
    {
    }

    void
    ThreadStrategy::strategy_init (TAO_Root_POA * /*poa*/ ACE_ENV_ARG_DECL_NOT_USED)
    {
    }

    void
    ThreadStrategy::strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
