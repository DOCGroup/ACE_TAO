// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/PrimitiveDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PrimitiveDef_i::TAO_PrimitiveDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_PrimitiveDef_i::~TAO_PrimitiveDef_i (void)
{
}

CORBA::DefinitionKind
TAO_PrimitiveDef_i::def_kind (void)
{
  return CORBA::dk_Primitive;
}

void
TAO_PrimitiveDef_i::destroy (void)
{
  this->destroy_i ();
}

void
TAO_PrimitiveDef_i::destroy_i (void)
{
  throw CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
}

CORBA::TypeCode_ptr
TAO_PrimitiveDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_PrimitiveDef_i::type_i (void)
{
  u_int pkind = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "pkind",
                                             pkind);

  switch (static_cast<CORBA::PrimitiveKind> (pkind))
  {
    case CORBA::pk_null:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_null);
    case CORBA::pk_void:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_void);
    case CORBA::pk_short:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_short);
    case CORBA::pk_long:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_long);
    case CORBA::pk_ushort:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
    case CORBA::pk_ulong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
    case CORBA::pk_float:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_float);
    case CORBA::pk_double:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_double);
    case CORBA::pk_boolean:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
    case CORBA::pk_char:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_char);
    case CORBA::pk_octet:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_octet);
    case CORBA::pk_any:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_any);
    case CORBA::pk_TypeCode:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);
    case CORBA::pk_Principal:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_Principal);
    case CORBA::pk_string:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_string);
    case CORBA::pk_objref:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_Object);
    case CORBA::pk_longlong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);
    case CORBA::pk_ulonglong:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);
    case CORBA::pk_longdouble:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);
    case CORBA::pk_wchar:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);
    case CORBA::pk_wstring:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
    case CORBA::pk_value_base:
      // TODO - we haven't added this typecode constant yet.
    default:
      return CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  }
}

CORBA::PrimitiveKind
TAO_PrimitiveDef_i::kind (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::pk_null);

  this->update_key ();

  return this->kind_i ();
}

CORBA::PrimitiveKind
TAO_PrimitiveDef_i::kind_i (void)
{
  u_int pkind = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "pkind",
                                             pkind);

  return static_cast<CORBA::PrimitiveKind> (pkind);
}

TAO_END_VERSIONED_NAMESPACE_DECL
