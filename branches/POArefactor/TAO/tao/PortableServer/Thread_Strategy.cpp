// $Id$

#include "Thread_Strategy.h"

ACE_RCSID (PortableServer,
           Thread_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Thread_Strategy::~Thread_Strategy()
    {
    }

    void
    Thread_Strategy::strategy_init (TAO_POA *poa, CORBA::PolicyList *policy_list)
    {
      // dependent on type create the correct strategy.
    }

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

