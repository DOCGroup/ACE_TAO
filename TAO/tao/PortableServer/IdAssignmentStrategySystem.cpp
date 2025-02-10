// -*- C++ -*-
#include "tao/PortableServer/IdAssignmentStrategySystem.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategySystem::id_assignment_key_type () const
    {
      return 'S';
    }

    bool
    IdAssignmentStrategySystem::has_system_id () const
    {
      return true;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

