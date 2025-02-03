#include "tao/PortableServer/IdAssignmentStrategyUser.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategyUser::id_assignment_key_type () const
    {
      return 'U';
    }

    bool
    IdAssignmentStrategyUser::has_system_id () const
    {
      return false;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
