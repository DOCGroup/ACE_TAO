/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "PrimaryKeyDef_i.h"

ACE_RCSID(IFR_Service, PrimaryKeyDef_i, "$Id$")

TAO_PrimaryKeyDef_i::TAO_PrimaryKeyDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_PrimaryKeyDef_i::~TAO_PrimaryKeyDef_i (void)
{
}

CORBA::DefinitionKind
TAO_PrimaryKeyDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_PrimaryKey;
}

void
TAO_PrimaryKeyDef_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_PrimaryKeyDef_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA_Contained::Description *
TAO_PrimaryKeyDef_i::describe (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Contained::Description *
TAO_PrimaryKeyDef_i::describe_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean
TAO_PrimaryKeyDef_i::is_a (const char *primary_key_id
                           TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (primary_key_id
                       TAO_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_PrimaryKeyDef_i::is_a_i (const char * /* primary_key_id */
                             TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA_ValueDef_ptr
TAO_PrimaryKeyDef_i::primary_key (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_ValueDef::_nil ());

  return this->primary_key_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_ValueDef_ptr
TAO_PrimaryKeyDef_i::primary_key_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

