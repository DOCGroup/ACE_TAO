// $Id$

#include "ThreadStrategyORBControl.h"

ACE_RCSID (PortableServer,
           ThreadStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ORBControl_Thread_Strategy::~ORBControl_Thread_Strategy()
    {
    }

    int
    ORBControl_Thread_Strategy::enter ()
    {
      return 0;
    }

    int
    ORBControl_Thread_Strategy::exit ()
    {
      return 0;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
