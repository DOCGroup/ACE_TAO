// $Id$

#include "tao/PortableServer/IdAssignmentStrategyUser.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategyUser::id_assignment_key_type (void) const
    {
      return 'U';
    }

    bool
    IdAssignmentStrategyUser::has_system_id (void) const
    {
      return false;
    }
  }
}


ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  IdAssignmentStrategyUser,
  TAO::Portable_Server::IdAssignmentStrategyUser)

ACE_STATIC_SVC_DEFINE (
  IdAssignmentStrategyUser,
  ACE_TEXT ("IdAssignmentStrategyUser"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (IdAssignmentStrategyUser),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL
