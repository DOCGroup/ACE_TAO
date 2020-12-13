#include "Repository_i.h"
#include "FactoryDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FactoryDef_i::TAO_FactoryDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_OperationDef_i (repo)
{
}

TAO_FactoryDef_i::~TAO_FactoryDef_i ()
{
}

CORBA::DefinitionKind
TAO_FactoryDef_i::def_kind ()
{
  return CORBA::dk_Factory;
}

TAO_END_VERSIONED_NAMESPACE_DECL
