// @(#) $Id$

#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/Typecode.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, TypeCodeFactory_Adapter, "$Id$")

TAO_TypeCodeFactory_Adapter::~TAO_TypeCodeFactory_Adapter (void)
{
}

// Methods to be overridden.

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_struct_tc (
    const char * /* id */,
    const char * /* name */,
    const CORBA_StructMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_union_tc (
    const char * /* id */,
    const char * /* name */,
    CORBA::TypeCode_ptr /* discriminator_type */,
    const CORBA_UnionMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_enum_tc (
    const char * /* id */,
    const char * /* name */,
    const CORBA_EnumMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_alias_tc (
    const char * /* id */,
    const char * /* name */,
    CORBA::TypeCode_ptr /* original_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_exception_tc (
    const char * /* id */,
    const char * /* name */,
    const CORBA_StructMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_interface_tc (
    const char * /* id */,
    const char * /* name */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_string_tc (
    CORBA::ULong /* bound */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_wstring_tc (
    CORBA::ULong /* bound */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_fixed_tc (
    CORBA::UShort /* digits */,
    CORBA::UShort /* scale */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_sequence_tc (
    CORBA::ULong /* bound */,
    CORBA::TypeCode_ptr /* element_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_array_tc (
    CORBA::ULong /* length */,
    CORBA::TypeCode_ptr /* element_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_value_tc (
    const char * /* id */,
    const char * /* name */,
    CORBA::ValueModifier /* type_modifier */,
    CORBA::TypeCode_ptr /* concrete_base */,
    const CORBA_ValueMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_value_box_tc (
    const char * /* id */,
    const char * /* name */,
    CORBA::TypeCode_ptr  /* boxed_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_native_tc (
    const char * /* id */,
    const char * /* name */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_recursive_tc (
    const char * /* id */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_abstract_interface_tc (
    const char * /* id */,
    const char * /* name */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter::create_local_interface_tc (
    const char * /* id */,
    const char * /* name */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::TypeCode::_nil ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


