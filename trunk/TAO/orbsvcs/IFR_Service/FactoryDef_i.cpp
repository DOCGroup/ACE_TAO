/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "FactoryDef_i.h"

ACE_RCSID(IFR_Service, FactoryDef_i, "$Id$")

TAO_FactoryDef_i::TAO_FactoryDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_OperationDef_i (repo, section_key)
{
}

TAO_FactoryDef_i::~TAO_FactoryDef_i (void)
{
}

IR::DefinitionKind
TAO_FactoryDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Factory;
}

void 
TAO_FactoryDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_FactoryDef_i::describe (CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

