// $Id$

#include "Loadable_Thread_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_ThreadPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Thread_Policy,
           "$Id$")

namespace TAO
{
  Loadable_Thread_Policy::~Loadable_Thread_Policy (void)
  {
  }

  PortableServer::ThreadPolicy_ptr
  Loadable_Thread_Policy::create (
    PortableServer::ThreadPolicyValue value)
  {
    POA_ThreadPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_ThreadPolicy (value),
                    0);

    return policy;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Thread_Policy,
      ACE_TEXT ("Loadable_Thread_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Thread_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Thread_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Thread_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Thread_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

