// -*- C++ -*-
// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ConsumesDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ConsumesDef_i::TAO_ConsumesDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_EventPortDef_i (repo)
{
}

TAO_ConsumesDef_i::~TAO_ConsumesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ConsumesDef_i::def_kind (void)
{
  return CORBA::dk_Consumes;
}

TAO_END_VERSIONED_NAMESPACE_DECL
