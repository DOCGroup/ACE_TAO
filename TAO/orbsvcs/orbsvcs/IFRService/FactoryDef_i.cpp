/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "FactoryDef_i.h"

ACE_RCSID (IFRService, 
           FactoryDef_i, 
           "$Id$")

TAO_FactoryDef_i::TAO_FactoryDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_OperationDef_i (repo)
{
}

TAO_FactoryDef_i::~TAO_FactoryDef_i (void)
{
}

CORBA::DefinitionKind
TAO_FactoryDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Factory;
}

