/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "FinderDef_i.h"

ACE_RCSID(IFR_Service, FinderDef_i, "$Id$")

TAO_FinderDef_i::TAO_FinderDef_i (TAO_Repository_i *repo,
                                  ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_OperationDef_i (repo, section_key)
{
}

TAO_FinderDef_i::~TAO_FinderDef_i (void)
{
}

IR::DefinitionKind
TAO_FinderDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Finder;
}

void 
TAO_FinderDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_FinderDef_i::describe (CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

