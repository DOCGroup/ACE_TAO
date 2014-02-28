// -*- C++ -*-
// $Id$

#include "tao/PortableServer/IdAssignmentStrategySystem.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategySystem::id_assignment_key_type (void) const
    {
      return 'S';
    }

    bool
    IdAssignmentStrategySystem::has_system_id (void) const
    {
      return true;
    }
  }
}

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  IdAssignmentStrategySystem,
  TAO::Portable_Server::IdAssignmentStrategySystem)

ACE_STATIC_SVC_DEFINE (
  IdAssignmentStrategySystem,
  ACE_TEXT ("IdAssignmentStrategySystem"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (IdAssignmentStrategySystem),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL

