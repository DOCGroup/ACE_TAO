/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueMemberDef_i.h"

ACE_RCSID(IFR_Service, ValueMemberDef_i, "$Id$")

TAO_ValueMemberDef_i::TAO_ValueMemberDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_ValueMemberDef_i::~TAO_ValueMemberDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueMemberDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_ValueMember;
}

void 
TAO_ValueMemberDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_ValueMemberDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA_Contained::Description *
TAO_ValueMemberDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

CORBA_Contained::Description *
TAO_ValueMemberDef_i::describe_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ValueMemberDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_ValueMemberDef_i::type_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA_IDLType_ptr 
TAO_ValueMemberDef_i::type_def (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_IDLType::_nil ());

  return this->type_def_i (ACE_TRY_ENV);
}

CORBA_IDLType_ptr 
TAO_ValueMemberDef_i::type_def_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueMemberDef_i::type_def (CORBA_IDLType_ptr type_def,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->type_def_i (type_def,
                    ACE_TRY_ENV);
}

void 
TAO_ValueMemberDef_i::type_def_i (CORBA_IDLType_ptr /* type_def */,
                                  CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Visibility 
TAO_ValueMemberDef_i::access (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->access_i (ACE_TRY_ENV);
}

CORBA::Visibility 
TAO_ValueMemberDef_i::access_i (CORBA::Environment & /* ACE_TRY_ENV */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueMemberDef_i::access (CORBA::Visibility access,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->access (access,
                ACE_TRY_ENV);
}

void 
TAO_ValueMemberDef_i::access_i (CORBA::Visibility /* access */,
                                CORBA::Environment & /* ACE_TRY_ENV */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

