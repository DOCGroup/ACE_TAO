#include "IdAssignmentPolicyValueUser.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicyValueUser,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicyValueUser::~IdAssignmentPolicyValueUser (void)
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    IdAssignmentPolicyValueUser::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USER_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentPolicyValueUser)

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentPolicyValueUser,
        ACE_TEXT ("IdAssignmentPolicyValueUser"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentPolicyValueUser),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdAssignmentPolicyValueUser>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdAssignmentPolicyValueUser>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

