/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "FixedDef_i.h"

ACE_RCSID(IFR_Service, FixedDef_i, "$Id$")

TAO_FixedDef_i::TAO_FixedDef_i (TAO_Repository_i *repo,
                                ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_FixedDef_i::~TAO_FixedDef_i (void)
{
}

CORBA::DefinitionKind
TAO_FixedDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Fixed;
}

void 
TAO_FixedDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_FixedDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::TypeCode_ptr 
TAO_FixedDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_FixedDef_i::type_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::UShort 
TAO_FixedDef_i::digits (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->digits_i (ACE_TRY_ENV);
}

CORBA::UShort 
TAO_FixedDef_i::digits_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_FixedDef_i::digits (CORBA::UShort digits,
                        CORBA::Environment & ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->digits_i (digits,
                  ACE_TRY_ENV);
}

void 
TAO_FixedDef_i::digits_i (CORBA::UShort /* digits */,
                          CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Short 
TAO_FixedDef_i::scale (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->scale_i (ACE_TRY_ENV);
}

CORBA::Short 
TAO_FixedDef_i::scale_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_FixedDef_i::scale (CORBA::Short scale,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->scale_i (scale,
                 ACE_TRY_ENV);
}

void 
TAO_FixedDef_i::scale_i (CORBA::Short /* scale */,
                       CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

