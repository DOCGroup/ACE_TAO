#include "ThreadPolicyValueSingle.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ThreadPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadPolicyValueSingle::~ThreadPolicyValueSingle (void)
    {
    }

    ::PortableServer::ThreadPolicyValue
    ThreadPolicyValueSingle::policy_type ()
    {
      return ::PortableServer::SINGLE_THREAD_MODEL;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadPolicyValueSingle)

    ACE_STATIC_SVC_DEFINE (
        ThreadPolicyValueSingle,
        ACE_TEXT ("ThreadPolicyValueSingle"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadPolicyValueSingle),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )
    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ThreadPolicyValueSingle>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ThreadPolicyValueSingle>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
