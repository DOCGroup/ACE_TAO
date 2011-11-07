// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/FixedDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FixedDef_i::TAO_FixedDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_FixedDef_i::~TAO_FixedDef_i (void)
{
}

CORBA::DefinitionKind
TAO_FixedDef_i::def_kind (void)
{
  return CORBA::dk_Fixed;
}

void
TAO_FixedDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_FixedDef_i::destroy_i (void)
{
  // TODO
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_FixedDef_i::type_i (void)
{
  // TODO
  return CORBA::TypeCode::_nil ();
}

CORBA::UShort
TAO_FixedDef_i::digits (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->digits_i ();
}

CORBA::UShort
TAO_FixedDef_i::digits_i (void)
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::digits (CORBA::UShort digits)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->digits_i (digits);
}

void
TAO_FixedDef_i::digits_i (CORBA::UShort /* digits */
                          )
{
  // TODO
}

CORBA::Short
TAO_FixedDef_i::scale (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->scale_i ();
}

CORBA::Short
TAO_FixedDef_i::scale_i (void)
{
  // TODO
  return 0;
}

void
TAO_FixedDef_i::scale (CORBA::Short scale)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->scale_i (scale);
}

void
TAO_FixedDef_i::scale_i (CORBA::Short /* scale */
                         )
{
  // TODO
}

TAO_END_VERSIONED_NAMESPACE_DECL
