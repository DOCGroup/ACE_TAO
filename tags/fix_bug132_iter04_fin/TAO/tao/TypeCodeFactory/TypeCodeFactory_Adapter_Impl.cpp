// $Id$

#include "TypeCodeFactory_Adapter_Impl.h"
#include "TypeCodeFactory_i.h"
#include "tao/ORB_Core.h"

ACE_RCSID(IFR_Service, TypeCodeFactory_Adapter_Impl, "$Id$")

TAO_TypeCodeFactory_Adapter_Impl::~TAO_TypeCodeFactory_Adapter_Impl (void)
{
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_struct_tc (
    const char * id,
    const char * name,
    const CORBA_StructMemberSeq & members,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_struct_tc (id,
                               name,
                               members,
                               ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_union_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr discriminator_type,
    const CORBA_UnionMemberSeq & members,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_union_tc (id,
                              name,
                              discriminator_type,
                              members,
                              ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_enum_tc (
    const char * id,
    const char * name,
    const CORBA_EnumMemberSeq & members,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_enum_tc (id,
                             name,
                             members,
                             ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_alias_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr original_type,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_alias_tc (id,
                              name,
                              original_type,
                              ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_exception_tc (
    const char * id,
    const char * name,
    const CORBA_StructMemberSeq & members,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_exception_tc (id,
                                  name,
                                  members,
                                  ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_interface_tc (
    const char * id,
    const char * name,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_interface_tc (id,
                                  name,
                                  ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_string_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_string_tc (bound,
                               ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_wstring_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_wstring_tc (bound,
                                ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_fixed_tc (
    CORBA::UShort digits,
    CORBA::UShort scale,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_fixed_tc (digits,
                              scale,
                              ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_sequence_tc (bound,
                                 element_type,
                                 ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_array_tc (length,
                              element_type,
                              ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_value_tc (
    const char * id,
    const char * name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA_ValueMemberSeq & members,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_value_tc (id,
                              name,
                              type_modifier,
                              concrete_base,
                              members,
                              ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_value_box_tc (
    const char * id,
    const char * name,
    CORBA::TypeCode_ptr boxed_type,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_value_box_tc (id,
                                  name,
                                  boxed_type,
                                  ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_native_tc (
    const char * id,
    const char * name,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_native_tc (id,
                               name,
                               ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_recursive_tc (
    const char * id,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_recursive_tc (id,
                                  ACE_TRY_ENV);              
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_abstract_interface_tc (
    const char * id,
    const char * name,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_abstract_interface_tc (id,
                                           name,
                                           ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_Adapter_Impl::create_local_interface_tc (
    const char * id,
    const char * name,
    CORBA::Environment &ACE_TRY_ENV
  )  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TypeCodeFactory_i tcf;

  return tcf.create_local_interface_tc (id,
                                        name,
                                        ACE_TRY_ENV);
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_TypeCodeFactory_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::typecodefactory_adapter_name (
      "Concrete_TypeCodeFactory_Adapter"
    );

  ACE_Service_Config::static_svcs ()->insert (
      &ace_svc_desc_TAO_TypeCodeFactory_Adapter_Impl
    );
  
  return 0;
}

ACE_STATIC_SVC_DEFINE (
    TAO_TypeCodeFactory_Adapter_Impl,
    ACE_TEXT ("Concrete_TypeCodeFactory_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_TypeCodeFactory_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Adapter_Impl)

