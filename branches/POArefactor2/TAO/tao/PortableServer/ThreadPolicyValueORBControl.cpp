#include "ThreadPolicyValueORBControl.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ThreadPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadPolicyValueORBControl::~ThreadPolicyValueORBControl (void)
    {
    }

    ::PortableServer::ThreadPolicyValue
    ThreadPolicyValueORBControl::policy_type (void)
    {
      return ::PortableServer::ORB_CTRL_MODEL;
    }

    // @@ Johnny, probably you could use the namespace defines here at
    // a later stage.
    ACE_STATIC_SVC_DEFINE (
        ThreadPolicyValueORBControl,
        ACE_TEXT ("ThreadPolicyValueORBControl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadPolicyValueORBControl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

      ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadPolicyValueORBControl)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<ThreadPolicyValueORBControl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ThreadPolicyValueORBControl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
