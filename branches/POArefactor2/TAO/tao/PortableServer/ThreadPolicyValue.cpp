#include "ThreadPolicyValue.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ThreadPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadPolicyValue::~ThreadPolicyValue (void)
    {
    }

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ThreadPolicyValue>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ThreadPolicyValue>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
