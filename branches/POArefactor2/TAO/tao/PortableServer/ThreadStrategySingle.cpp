// $Id$

#include "ThreadStrategySingle.h"

ACE_RCSID (PortableServer,
           ThreadStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Single_Thread_Strategy::~Single_Thread_Strategy()
    {
    }

    int
    Single_Thread_Strategy::enter ()
    {
      return lock_.acquire();
    }

    int
    Single_Thread_Strategy::exit ()
    {
      return lock_.release();
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
