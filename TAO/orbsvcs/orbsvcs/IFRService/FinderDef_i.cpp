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

CORBA::DefinitionKind
TAO_FinderDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Finder;
}

void
TAO_FinderDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_FinderDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA_Contained::Description *
TAO_FinderDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Contained::Description *
TAO_FinderDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

