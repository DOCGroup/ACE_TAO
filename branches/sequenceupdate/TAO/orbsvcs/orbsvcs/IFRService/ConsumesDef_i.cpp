/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ConsumesDef_i.h"

ACE_RCSID (IFRService, 
           ConsumesDef_i, 
           "$Id$")

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
TAO_ConsumesDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Consumes;
}

