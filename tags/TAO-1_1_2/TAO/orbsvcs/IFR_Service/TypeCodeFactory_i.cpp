/* -*- C++ -*- */
// $Id$

#include "TypeCodeFactory_i.h"

ACE_RCSID(IFR_Service, TypeCodeFactory_i, "$Id$")

TAO_TypeCodeFactory_i::TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i::~TAO_TypeCodeFactory_i (void)
{
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_struct_tc (
    const char *id,
    const char *name,
    const IR::StructMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  // Number of members..
  CORBA::ULong len = members.length ();
  cdr << len;

  for (CORBA::ULong index = 0; index < len; index++)
    {
      // Get the first member which is a string..
      IR::StructMember struct_member = members[index];

      cdr << struct_member.name.in ();

      cdr << struct_member.type.in ();
    }

  CORBA::TypeCode_ptr interface_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (interface_typecode,
                    CORBA_TypeCode (CORBA::tk_struct,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return interface_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_union_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr discriminator_type,
    const IR::UnionMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_enum_tc (
    const char *id,
    const char *name,
    const IR::EnumMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  // The piece of code that follows has been based
  // on the code in the IDL compiler
  cdr << TAO_ENCAP_BYTE_ORDER; // Byte Order

  cdr << id;

  cdr << name;

  CORBA::ULong len = members.length ();

  cdr << len;

  for (CORBA::ULong index = 0; index < len; index++)
    {
      cdr << members[index].in ();
    }

  CORBA::TypeCode_ptr interface_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (interface_typecode,
                    CORBA_TypeCode (CORBA::tk_enum,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return interface_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_alias_tc (
    const char *id,
    const char *name,
    const CORBA::TypeCode_ptr original_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << original_type;

  CORBA::TypeCode_ptr interface_typecode = 
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (interface_typecode,
                    CORBA_TypeCode (CORBA::tk_alias,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return interface_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_exception_tc (
    const char *id,
    const char *name,
    const IR::StructMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  // The piece of code that follows has been based
  // on the code in the IDL compiler
  cdr << TAO_ENCAP_BYTE_ORDER; // Byte Order

  cdr << id;

  cdr << name;

  // Number of members..
  CORBA::ULong len = members.length ();
  cdr << len;

  for (CORBA::ULong index = 0; index < len; index++)
    {
      // Get the first member which is a string..
      IR::StructMember struct_member = members[index];

      cdr << struct_member.name.in ();

      cdr << struct_member.type.in ();
    }

  CORBA::TypeCode_ptr interface_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (interface_typecode,
                    CORBA_TypeCode (CORBA::tk_except,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return interface_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_interface_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  // The piece of code that follows has been based
  // on the code in the IDL compiler
  cdr << TAO_ENCAP_BYTE_ORDER; // Byte Order

  // Use the overloaded operator from the TAO_Output CDR class
  cdr << id;

  // Send the name
  cdr << name;

  CORBA::TypeCode_ptr interface_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (interface_typecode,
                    CORBA_TypeCode (CORBA::tk_objref,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return interface_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_string_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_wstring_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_fixed_tc (
    CORBA::UShort digits,
    CORBA::UShort scale,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_value_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const IR::ValueMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_value_box_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr boxed_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_native_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_recursive_tc (
    const char *id,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_abstract_interface_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_component_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_home_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0; 
}

