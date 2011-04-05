// $Id$

#include "tao/PortableServer/Active_Object_Map_Entry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Active_Object_Map_Entry::TAO_Active_Object_Map_Entry (void)
  : user_id_ (),
    system_id_ (),
    servant_ (0),
    reference_count_ (1),
    deactivated_ (false),
    priority_ (-1)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
