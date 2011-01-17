// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/FinderDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FinderDef_i::TAO_FinderDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_OperationDef_i (repo)
{
}

TAO_FinderDef_i::~TAO_FinderDef_i (void)
{
}

CORBA::DefinitionKind
TAO_FinderDef_i::def_kind (void)
{
  return CORBA::dk_Finder;
}

TAO_END_VERSIONED_NAMESPACE_DECL
