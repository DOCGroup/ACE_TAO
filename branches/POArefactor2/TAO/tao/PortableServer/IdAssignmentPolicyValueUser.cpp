#include "IdAssignmentPolicyValueUser.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicyValueUser,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    User_IdAssignment_Policy::~User_IdAssignment_Policy (void)
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    User_IdAssignment_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USER_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, User_IdAssignment_Policy)

    ACE_STATIC_SVC_DEFINE (
        User_IdAssignment_Policy,
        ACE_TEXT ("User_IdAssignment_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (User_IdAssignment_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<User_IdAssignment_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<User_IdAssignment_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
