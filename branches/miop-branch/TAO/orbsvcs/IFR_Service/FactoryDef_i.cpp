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

CORBA::DefinitionKind
TAO_FactoryDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Factory;
}

void 
TAO_FactoryDef_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void 
TAO_FactoryDef_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA_Contained::Description *
TAO_FactoryDef_i::describe (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Contained::Description *
TAO_FactoryDef_i::describe_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

