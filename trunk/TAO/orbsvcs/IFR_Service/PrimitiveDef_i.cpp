/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "PrimitiveDef_i.h"

ACE_RCSID(IFR_Service, PrimitiveDef_i, "$Id$")

TAO_PrimitiveDef_i::TAO_PrimitiveDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_PrimitiveDef_i::~TAO_PrimitiveDef_i (void)
{
}

IR::DefinitionKind
TAO_PrimitiveDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Primitive;
}

void 
TAO_PrimitiveDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_PrimitiveDef_i::destroy_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::BAD_INV_ORDER (2, CORBA::COMPLETED_NO));
}

CORBA::TypeCode_ptr 
TAO_PrimitiveDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_PrimitiveDef_i::type_i (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int pkind = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "pkind",
                                             pkind);

  switch (ACE_static_cast (IR::PrimitiveKind, pkind))
  {
    case IR::pk_null:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_null);
    case IR::pk_void:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_void);
    case IR::pk_short:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_short);
    case IR::pk_long:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_long);
    case IR::pk_ushort:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
    case IR::pk_ulong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
    case IR::pk_float:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_float);
    case IR::pk_double:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_double);
    case IR::pk_boolean:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
    case IR::pk_char:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_char);
    case IR::pk_octet:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_octet);
    case IR::pk_any:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_any);
    case IR::pk_TypeCode:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);
    case IR::pk_Principal:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_Principal);
    case IR::pk_string:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_string);
    case IR::pk_objref:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_Object);
    case IR::pk_longlong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);
    case IR::pk_ulonglong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);
    case IR::pk_longdouble:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);
    case IR::pk_wchar:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);
    case IR::pk_wstring:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
    case IR::pk_value_base:
      // TODO - we haven't added this typecode constant yet.
    default:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  }
}

IR::PrimitiveKind 
TAO_PrimitiveDef_i::kind (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::pk_null);

  return this->kind_i (ACE_TRY_ENV);
}

IR::PrimitiveKind 
TAO_PrimitiveDef_i::kind_i (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int pkind = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "pkind",
                                             pkind);

  return ACE_static_cast (IR::PrimitiveKind, pkind);
}

