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
TAO_PrimaryKeyDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_PrimaryKey;
}

void 
TAO_PrimaryKeyDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_PrimaryKeyDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA_Contained::Description *
TAO_PrimaryKeyDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

CORBA_Contained::Description *
TAO_PrimaryKeyDef_i::describe_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean 
TAO_PrimaryKeyDef_i::is_a (const char *primary_key_id,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (primary_key_id,
                       ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_PrimaryKeyDef_i::is_a_i (const char * /* primary_key_id */,
                             CORBA::Environment & /* ACE_TRY_ENV */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA_ValueDef_ptr 
TAO_PrimaryKeyDef_i::primary_key (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_ValueDef::_nil ());

  return this->primary_key_i (ACE_TRY_ENV);
}

CORBA_ValueDef_ptr 
TAO_PrimaryKeyDef_i::primary_key_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

