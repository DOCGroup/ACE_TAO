// $Id$

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_i.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_TypeCodeFactory_Adapter_Impl::~TAO_TypeCodeFactory_Adapter_Impl (void)
{
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_struct_tc (
    const char * id,
    const char * name,
    const CORBA::StructMemberSeq & members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_struct_tc (id, name, members);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_union_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr discriminator_type,
    const CORBA::UnionMemberSeq & members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_union_tc (id, name, discriminator_type, members);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_enum_tc (
    const char * id,
    const char * name,
    const CORBA::EnumMemberSeq & members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_enum_tc (id, name, members);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_alias_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr original_type)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_alias_tc (id, name, original_type);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_exception_tc (
    const char * id,
    const char * name,
    const CORBA::StructMemberSeq & members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_exception_tc (id, name, members);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_interface_tc (
    const char * id,
    const char * name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_interface_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_string_tc (CORBA::ULong bound)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_string_tc (bound);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_wstring_tc (CORBA::ULong bound)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_wstring_tc (bound);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_fixed_tc (
    CORBA::UShort digits,
    CORBA::UShort scale)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_fixed_tc (digits, scale);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_sequence_tc (bound, element_type);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_array_tc (length, element_type);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_value_tc (
    const char * id,
    const char * name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq & members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_value_tc (id, name, type_modifier, concrete_base, members);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_value_box_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr boxed_type)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_value_box_tc (id, name, boxed_type);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_native_tc (
    const char * id,
    const char * name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_native_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_recursive_tc (const char * id)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_recursive_tc (id);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_abstract_interface_tc (
    const char * id,
    const char * name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_abstract_interface_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_local_interface_tc (
    const char * id,
    const char * name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_local_interface_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_component_tc (
    const char *id,
    const char *name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_component_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_home_tc (
    const char *id,
    const char *name)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_home_tc (id, name);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_event_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq &members)
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_event_tc (id, name, type_modifier, concrete_base, members);
}

// --

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_enum_tc (
  char const * /* id */,
  char const * /* name */,
  ACE_Array_Base<CORBA::String_var> const & /* enumerators */,
  CORBA::ULong /* ncases */
  )
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_struct_except_tc (
  CORBA::TCKind /* kind */,
  char const * /* id */,
  char const * /* name */,
  ACE_Array_Base<
    TAO::TypeCode::Struct_Field<CORBA::String_var,
                                CORBA::TypeCode_var> > const & /* fields */,
  CORBA::ULong /* nfields */
  )
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_union_tc (
  char const * /* id */,
  char const * /* name */,
  CORBA::TypeCode_ptr /* discriminant_type */,
  ACE_Array_Base<TAO::TypeCode::Case<CORBA::String_var,
                                     CORBA::TypeCode_var> > const & /* cases */,
  CORBA::ULong /* ncases */,
  CORBA::Long /* default_index */,
  char const * /* default_case_name */,
  CORBA::TypeCode_ptr /* default_case_type */
  )
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_Adapter_Impl::create_value_event_tc (
  CORBA::TCKind /* kind */,
  char const * /* id */,
  char const * /* name */,
  CORBA::ValueModifier /* modifier */,
  CORBA::TypeCode_ptr /* concrete_base */,
  ACE_Array_Base<
    TAO::TypeCode::Value_Field<CORBA::String_var,
                               CORBA::TypeCode_var> > const & /* fields */,
  CORBA::ULong /* nfields */
  )
{
  throw ::CORBA::NO_IMPLEMENT ();
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_TypeCodeFactory_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::typecodefactory_adapter_name ("Concrete_TypeCodeFactory_Adapter");

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_TypeCodeFactory_Adapter_Impl);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
    TAO_TypeCodeFactory_Adapter_Impl,
    ACE_TEXT ("Concrete_TypeCodeFactory_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_TypeCodeFactory_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Adapter_Impl)
