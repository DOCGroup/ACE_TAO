// $Id$

#include "ThreadStrategySingle.h"

ACE_RCSID (PortableServer,
           ThreadStrategySingle,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadStrategySingle::~ThreadStrategySingle()
    {
    }

    int
    ThreadStrategySingle::enter ()
    {
      return lock_.acquire();
    }

    int
    ThreadStrategySingle::exit ()
    {
      return lock_.release();
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
