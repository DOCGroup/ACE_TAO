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
    ThreadStrategy::strategy_init (TAO_POA *)
    {
      // dependent on type create the correct strategy.
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
