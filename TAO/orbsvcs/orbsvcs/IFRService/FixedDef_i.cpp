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
TAO_FixedDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Fixed;
}

void
TAO_FixedDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_FixedDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::UShort
TAO_FixedDef_i::digits (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->digits_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::UShort
TAO_FixedDef_i::digits_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::digits (CORBA::UShort digits
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->digits_i (digits
                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_FixedDef_i::digits_i (CORBA::UShort /* digits */
                          ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Short
TAO_FixedDef_i::scale (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->scale_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Short
TAO_FixedDef_i::scale_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::scale (CORBA::Short scale
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->scale_i (scale
                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_FixedDef_i::scale_i (CORBA::Short /* scale */
                       ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

