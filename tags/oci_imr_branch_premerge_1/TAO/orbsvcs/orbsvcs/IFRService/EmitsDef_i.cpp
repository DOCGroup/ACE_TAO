/* -*- C++ -*- */
// $Id$

#include "EmitsDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           EmitsDef_i, 
           "$Id$")

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
TAO_EmitsDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Emits;
}

