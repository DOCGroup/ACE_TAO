//$Id$
// -*- C++ -*-


#ifndef FACTORY_STRUCT_H
#define FACTORY_STRUCT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

#include "ace/Array_Base.h"


struct Factory_Node
{

  /// FactoryCreationId assigned to the member.
  PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

};

typedef ACE_Array_Base<Factory_Node> Factory_Struct;


#include "ace/post.h"

#endif  /* FACTORY_STRUCT_H */
