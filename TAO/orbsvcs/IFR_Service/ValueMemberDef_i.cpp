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

IR::DefinitionKind
TAO_ValueMemberDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_ValueMember;
}

void 
TAO_ValueMemberDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_ValueMemberDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ValueMemberDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::IDLType_ptr 
TAO_ValueMemberDef_i::type_def (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueMemberDef_i::type_def (IR::IDLType_ptr type_def,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Visibility 
TAO_ValueMemberDef_i::access (CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueMemberDef_i::access (CORBA::Visibility access,
                              CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

