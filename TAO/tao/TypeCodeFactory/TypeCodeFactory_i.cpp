/* -*- C++ -*- */
// $Id$

#include "TypeCodeFactory_i.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/Marshal.h"

ACE_RCSID(IFR_Service, TypeCodeFactory_i, "$Id$")

TAO_TypeCodeFactory_i::TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i::~TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i *
TAO_TypeCodeFactory_i::_narrow (CORBA::Object_ptr obj,
                                CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_TypeCodeFactory_i *,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_TypeCodeFactory_i::_narrow
                        )
                    )
           );
}

void *
TAO_TypeCodeFactory_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_TypeCodeFactory_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return this->CORBA_TypeCodeFactory::_tao_QueryInterface (type);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_struct_tc (
    const char *id,
    const char *name,
    const CORBA_StructMemberSeq &members,
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

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      cdr << members[index].name;

      cdr << members[index].type.in ();
    }

  CORBA::TypeCode_ptr struct_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (struct_typecode,
                    CORBA_TypeCode (CORBA::tk_struct,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return struct_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_union_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr discriminator_type,
    const CORBA_UnionMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << discriminator_type;

  CORBA::ULong len = members.length ();
  CORBA::ULong dups = 0;
  CORBA::Long default_index = -1;
  CORBA::Long raw_default_index = -1;
  CORBA::Octet value = ACE_OCTET_MAX;

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (i > 0)
        {
          // Is this a duplicate case label? If so, we have to adjust
          // the 'length' we encode - a member gets encoded only once.
          if (ACE_OS::strcmp (members[i].name, 
                              members[i - 1].name) 
               == 0)
            {
              ++dups;
            }
        }

      // Reset the default index, if we have a default case label.
      if ((members[i].label >>= CORBA::Any::to_octet (value)) == 1)
        {
          if (value == 0)
            {
              raw_default_index = ACE_static_cast (CORBA::Long, i);
              // Only the multiple labels that come before the
              // default label affect its adjusted value.
              default_index = ACE_static_cast (CORBA::Long, i - dups);
            }
        }
    }

  cdr << default_index;

  cdr << len - dups;

  for (CORBA::ULong index = 0; index < len; index++)
    {
      if (index > 0)
        {
          // Is this a duplicate case label? If so, skip it - a member
          // goes into the typecode only once.
          if (ACE_OS::strcmp (members[index].name, 
                              members[index - 1].name) 
               == 0)
            {
              continue;
            }
        }

      if (raw_default_index >= 0
          && index == ACE_static_cast (CORBA::ULong, raw_default_index))
        {
          // This is the default label - we have to find a legal value.
          CORBA::TCKind kind = discriminator_type->kind (ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          this->compute_default_label (kind,
                                       index,
                                       members,
                                       cdr);
        }
      else
        {
          // Interpretive marshaling refuses to die ;-).
          TAO_InputCDR in (members[index].label._tao_get_cdr (),
                           members[index].label._tao_byte_order ());

          CORBA::TypeCode_var tc = members[index].label.type ();

          (void) TAO_Marshal_Object::perform_append (tc.in (),
                                                     &in,
                                                     &cdr,
                                                     ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }

      cdr << members[index].name;

      cdr << members[index].type.in ();
    }

  CORBA::TypeCode_ptr union_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (union_typecode,
                    CORBA_TypeCode (CORBA::tk_union,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return union_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_enum_tc (
    const char *id,
    const char *name,
    const CORBA_EnumMemberSeq &members,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  CORBA::ULong len = members.length ();

  cdr << len;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      cdr << members[index];
    }

  CORBA::TypeCode_ptr enum_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (enum_typecode,
                    CORBA_TypeCode (CORBA::tk_enum,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return enum_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_alias_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr original_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << original_type;

  CORBA::TypeCode_ptr alias_typecode = 
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (alias_typecode,
                    CORBA_TypeCode (CORBA::tk_alias,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return alias_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_exception_tc (
    const char *id,
    const char *name,
    const CORBA_StructMemberSeq &members,
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

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      // Get the first member which is a string..
      CORBA_StructMember struct_member = members[index];

      cdr << struct_member.name;

      cdr << struct_member.type.in ();
    }

  CORBA::TypeCode_ptr exception_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (exception_typecode,
                    CORBA_TypeCode (CORBA::tk_except,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return exception_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_interface_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_objref,
                                 ACE_TRY_ENV); 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_string_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << bound;

  CORBA::TypeCode_ptr string_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (string_typecode,
                    CORBA_TypeCode (CORBA::tk_string,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return string_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_wstring_tc (
    CORBA::ULong bound,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << bound;

  CORBA::TypeCode_ptr wstring_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (wstring_typecode,
                    CORBA_TypeCode (CORBA::tk_wstring,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return wstring_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_fixed_tc (
    CORBA::UShort /* digits */,
    CORBA::UShort /* scale */,
    CORBA::Environment & /* ACE_TRY_ENV */
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
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << element_type;

  cdr << bound;

  CORBA::TypeCode_ptr sequence_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (sequence_typecode,
                    CORBA_TypeCode (CORBA::tk_sequence,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return sequence_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << element_type;

  cdr << length;

  CORBA::TypeCode_ptr array_typecode = 
    CORBA::TypeCode::_nil ();
  ACE_NEW_THROW_EX (array_typecode,
                    CORBA_TypeCode (CORBA::tk_array,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return array_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_value_tc (
    const char * /* id */,
    const char * /* name */,
    CORBA::ValueModifier /* type_modifier */,
    CORBA::TypeCode_ptr /* concrete_base */,
    const CORBA_ValueMemberSeq & /* members */,
    CORBA::Environment & /* ACE_TRY_ENV */
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
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << boxed_type;

  CORBA::TypeCode_ptr value_box_typecode = 
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (value_box_typecode,
                    CORBA_TypeCode (CORBA::tk_value_box,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return value_box_typecode;
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_native_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_native,
                                 ACE_TRY_ENV); 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_recursive_tc (
    const char * /* id */,
    CORBA::Environment & /* ACE_TRY_ENV */
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
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_abstract_interface,
                                 ACE_TRY_ENV); 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_local_interface_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_local_interface,
                                 ACE_TRY_ENV); 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_component_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_component,
                                 ACE_TRY_ENV); 
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_home_tc (
    const char *id,
    const char *name,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_home,
                                 ACE_TRY_ENV); 
}

void
TAO_TypeCodeFactory_i::compute_default_label (
    CORBA::TCKind kind,
    CORBA::ULong skip_slot,
    const CORBA_UnionMemberSeq &members,
    TAO_OutputCDR &cdr
  )
{
  // One to hold the current default value, one to
  // hold the curent label's extracted value.
  union disc_types
  {
    CORBA::Char char_val;
    CORBA::WChar wchar_val;
    CORBA::Boolean bool_val;
    CORBA::Short short_val;
    CORBA::UShort ushort_val;
    CORBA::Long long_val;
    CORBA::ULong ulong_val;
    CORBA::ULongLong ulonglong_val;
    CORBA::ULong enum_val;
    // TODO - handle (u)longlong types
  } dv, u;

  // To prevent 'may be uninitialized' warnings.
  dv.ulonglong_val = 0;

  // Set these to the minimum value they can have.
  switch (kind)
  {
    case CORBA::tk_char:
      dv.char_val = 0;
      break;
    case CORBA::tk_wchar:
      dv.wchar_val = 0;
      break;
    case CORBA::tk_boolean:
      dv.bool_val = 0;
      break;
    case CORBA::tk_short:
      dv.short_val = ACE_INT16_MIN;
      break;
    case CORBA::tk_ushort:
      dv.ushort_val = 0;
      break;
    case CORBA::tk_long:
      dv.long_val = ACE_INT32_MIN;
      break;
    case CORBA::tk_ulong:
      dv.ulong_val = 0;
      break;
    case CORBA::tk_ulonglong:
      dv.ulonglong_val = 0;
      break;
    case CORBA::tk_enum:
      dv.enum_val = 0;
      break;
    default:
      break;
  }

  CORBA::ULong len = members.length ();
  int success = 0;

  // A collision forces us to start over, because the label
  // values need not be in sorted order.
  while (success == 0)
    {
      success = 1;

      for (CORBA::ULong i = 0; i < len; i++)
        {
          // This is the one we're trying to find a legal value for.
          if (i == skip_slot)
            continue;

          // If there's a collision, we increment the default value.
          switch (kind)
          {
            case CORBA::tk_char:
              members[i].label >>= CORBA::Any::to_char (u.char_val);
              if (u.char_val == dv.char_val)
                {
                  dv.char_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_wchar:
              members[i].label >>= CORBA::Any::to_wchar (u.wchar_val);
              if (u.wchar_val == dv.wchar_val)
                {
                  dv.wchar_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_boolean:
              members[i].label >>= CORBA::Any::to_boolean (u.bool_val);
              if (u.bool_val == dv.bool_val)
                {
                  dv.bool_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_short:
              members[i].label >>= u.short_val;
              if (u.short_val == dv.short_val)
                {
                  dv.short_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_ushort:
              members[i].label >>= u.ushort_val;
              if (u.ushort_val == dv.ushort_val)
                {
                  dv.ushort_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_long:
              members[i].label >>= u.long_val;
              if (u.long_val == dv.long_val)
                {
                  dv.long_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_ulong:
              members[i].label >>= u.ulong_val;
              if (u.ulong_val == dv.ulong_val)
                {
                  dv.ulong_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_ulonglong:
              members[i].label >>= u.ulonglong_val;
              if (u.ulonglong_val == dv.ulonglong_val)
                {
                  dv.ulonglong_val++;
                  success = 0;
                }
              break;
            case CORBA::tk_enum:
            {
              TAO_InputCDR cdr (members[i].label._tao_get_cdr (),
                                members[i].label._tao_byte_order ());
              cdr.read_ulong (u.enum_val);
              if (u.enum_val == dv.enum_val)
                {
                  dv.enum_val++;
                  success = 0;
                }
              break;
            }
            default:
              break;
          }

          // If there's been a collision, we should start over right away.
          if (success == 0)
            break;
        }
    }

  // Add the default value to the encapsulation.
  switch (kind)
  {
    case CORBA::tk_char:
      cdr << CORBA::Any::from_char (dv.char_val);
      break;
    case CORBA::tk_wchar:
      cdr << CORBA::Any::from_wchar (dv.wchar_val);
      break;
    case CORBA::tk_boolean:
      cdr << CORBA::Any::from_boolean (dv.bool_val);
      break;
    case CORBA::tk_short:
      cdr << dv.short_val;
      break;
    case CORBA::tk_ushort:
      cdr << dv.ushort_val;
      break;
    case CORBA::tk_long:
      cdr << dv.long_val;
      break;
    case CORBA::tk_ulong:
      cdr << dv.ulong_val;
      break;
    case CORBA::tk_ulonglong:
      cdr << dv.ulonglong_val;
      break;
    case CORBA::tk_enum:
      cdr << dv.enum_val;
      break;
    default:
      break;
  }
}

CORBA::TypeCode_ptr 
TAO_TypeCodeFactory_i::create_tc_common (
    const char *id,
    const char *name,
    CORBA::TCKind kind,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  CORBA::TypeCode_ptr result = 
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (result,
                    CORBA_TypeCode (kind,
                                    cdr.total_length (),
                                    cdr.buffer (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return result;
}

