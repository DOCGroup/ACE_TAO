// $Id$

#include "orbsvcs/IFRService/EmitsDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EmitsDef_i::TAO_EmitsDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_EventPortDef_i (repo)
{
}

TAO_EmitsDef_i::~TAO_EmitsDef_i (void)
{
}

CORBA::DefinitionKind
TAO_EmitsDef_i::def_kind (void)
{
  return CORBA::dk_Emits;
}


TAO_END_VERSIONED_NAMESPACE_DECL
