// $Id$

#include "ThreadStrategyORBControl.h"

ACE_RCSID (PortableServer,
           ThreadStrategyORBControl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategyORBControl::~ThreadStrategyORBControl()
    {
    }

    int
    ThreadStrategyORBControl::enter ()
    {
      return 0;
    }

    int
    ThreadStrategyORBControl::exit ()
    {
      return 0;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
