#include "Thread_Policy_Value.h"
#include "PortableServerC.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_ThreadPolicy,
           "$Id$")

namespace TAO
{
  Thread_Policy_Value::~Thread_Policy_Value (void)
  {
  }

  SINGLE_THREAD_Thread_Policy::~SINGLE_THREAD_Thread_Policy (void)
  {
  }

  ORB_CTRL_Thread_Policy::~ORB_CTRL_Thread_Policy (void)
  {
  }

  PortableServer::ThreadPolicyValue
  ORB_CTRL_Thread_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::ORB_CTRL_MODEL;
  }

  PortableServer::ThreadPolicyValue
  SINGLE_THREAD_Thread_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::SINGLE_THREAD_MODEL;
  }

  ACE_FACTORY_DEFINE (TAO_PortableServer, SINGLE_THREAD_Thread_Policy)

  ACE_STATIC_SVC_DEFINE (
      SINGLE_THREAD_Thread_Policy,
      ACE_TEXT ("SINGLE_THREAD_Thread_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (SINGLE_THREAD_Thread_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, ORB_CTRL_Thread_Policy)

  ACE_STATIC_SVC_DEFINE (
      ORB_CTRL_Thread_Policy,
      ACE_TEXT ("ORB_CTRL_Thread_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (ORB_CTRL_Thread_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Thread_Policy_Value>;
  template class ACE_Dynamic_Service<ORB_CTRL_Thread_Policy>;
  template class ACE_Dynamic_Service<SINGLE_THREAD_Thread_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Thread_Policy_Value>
  #pragma instantiate ACE_Dynamic_Service<ORB_CTRL_Thread_Policy>
  #pragma instantiate ACE_Dynamic_Service<SINGLE_THREAD_Thread_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}
