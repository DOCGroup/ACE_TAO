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
    ORB_CTRL_Thread_Policy::~ORB_CTRL_Thread_Policy (void)
    {
    }

    ::PortableServer::ThreadPolicyValue
    ORB_CTRL_Thread_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::ORB_CTRL_MODEL;
    }

    // @@ Johnny, probably you could use the namespace defines here at
    // a later stage.
    ACE_STATIC_SVC_DEFINE (
        ORB_CTRL_Thread_Policy,
        ACE_TEXT ("ORB_CTRL_Thread_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ORB_CTRL_Thread_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ORB_CTRL_Thread_Policy)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ORB_CTRL_Thread_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ORB_CTRL_Thread_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
