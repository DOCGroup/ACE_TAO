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
TAO_FixedDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Fixed;
}

void
TAO_FixedDef_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_FixedDef_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::UShort
TAO_FixedDef_i::digits (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->digits_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::UShort
TAO_FixedDef_i::digits_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::digits (CORBA::UShort digits
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->digits_i (digits
                  TAO_ENV_ARG_PARAMETER);
}

void
TAO_FixedDef_i::digits_i (CORBA::UShort /* digits */
                          TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Short
TAO_FixedDef_i::scale (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->scale_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Short
TAO_FixedDef_i::scale_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::scale (CORBA::Short scale
                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->scale_i (scale
                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_FixedDef_i::scale_i (CORBA::Short /* scale */
                       TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

