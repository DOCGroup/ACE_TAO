// -*- C++ -*-
// $Id$

#include "TypeCodeFactory_i.h"

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/Marshal.h"

#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager_T.h"

ACE_RCSID (TypeCodeFactory,
           TypeCodeFactory_i,
           "$Id$")

TAO_TypeCodeFactory_i::TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i::~TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i *
TAO_TypeCodeFactory_i::_narrow (CORBA::Object_ptr obj
                                ACE_ENV_ARG_DECL_NOT_USED)
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
    const CORBA_StructMemberSeq &members
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->struct_except_tc_common (id,
                                        name,
                                        members,
                                        CORBA::tk_struct
                                        ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_union_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr discriminator_type,
    const CORBA_UnionMemberSeq &members
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  if (id != 0 && !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::Boolean good_disc_type =
    this->valid_disc_type (discriminator_type
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!good_disc_type)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (20,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::ULong len = members.length ();
  CORBA::ULong dups = 0;
  CORBA::ULong raw_default_index = ACE_UINT32_MAX;
  CORBA::Long default_index = -1;
  CORBA::Octet value = ACE_OCTET_MAX;
  CORBA::TypeCode_ptr tc_holder = CORBA::TypeCode::_nil ();
  CORBA::Boolean valid_member = 1;
  const char *member_name = 0;
  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  // No getting around iterating over the members twice. We have
  // to do it once *before* the overall length is written to the
  // CDR stream, to know by how much, if any, the number of members
  // differs from the number of labels.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      member_name = members[i].name;
      int trybind_status = 0;

      if (i > 0)
        {
          // Is this a duplicate case label? If so, we have to adjust
          // the 'length' we encode - a member gets encoded only once.
          if (ACE_OS::strcmp (member_name,
                              members[i - 1].name)
               == 0)
            {
              CORBA::Boolean equiv =
                members[i].type.in ()->equivalent (members[i - 1].type.in ()
                                                   ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

              // If adjacent names are the same and their types are equivalent,
              // then they are duplicate case labels. If the types are not
              // equivalent, then they are separate members with the same name,
              // which is not allowed.
              if (equiv)
                {
                  ++dups;
                }
              else
                {
                  ACE_THROW_RETURN (CORBA::BAD_PARAM (17,
                                                      CORBA::COMPLETED_NO),
                                    CORBA::TypeCode::_nil ());
                }
            }
          else
            {
              // Only if we do not have a duplicate case label do we
              // check for a duplicate member name.
              ACE_CString ext_id (member_name);
              int int_id = 0;

              trybind_status = map.trybind (ext_id, int_id);
            }
        }
      else
        {
          ACE_CString ext_id (member_name);
          int int_id = 0;

          trybind_status = map.trybind (ext_id, int_id);
        }

      // Duplicate member name?
      if (trybind_status != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (17,
                                              CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      tc_holder = members[i].type.in ();

      // Valid member type?
      valid_member = this->valid_content_type (tc_holder
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      if (!valid_member)
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                                 CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      // Reset the default index, if we have a default case label.
      if ((members[i].label >>= CORBA::Any::to_octet (value)) == 1)
        {
          if (value == 0)
            {
              raw_default_index = i;
              // Only the multiple labels that come before the
              // default label affect its adjusted value.
              default_index = ACE_static_cast (CORBA::Long, i - dups);
            }
        }
      else
        {
          // Else check that the label type is equivalent to the
          // given discriminator type.
          CORBA::TypeCode_var tmp = members[i].label.type ();

          CORBA::Boolean equiv =
            discriminator_type->equivalent (tmp.in ()
                                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          if (!equiv)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (19,
                                                  CORBA::COMPLETED_NO),
                                CORBA::TypeCode::_nil ());
            }
        }
    }

  CORBA::Boolean unique_labels =
    this->unique_label_values (members,
                               discriminator_type,
                               raw_default_index
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!unique_labels)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (18,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << discriminator_type;

  cdr << default_index;

  cdr << len - dups;

  for (CORBA::ULong index = 0; index < len; ++index)
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

      if (index == raw_default_index)
        {
          // This is the default label - we have to find a legal value.
          CORBA::TCKind kind = discriminator_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
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
                                                     &cdr
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }

      cdr << members[index].name;

      cdr << members[index].type.in ();
    }

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr union_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (union_typecode,
                    CORBA_TypeCode (CORBA::tk_union,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
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
    const CORBA_EnumMemberSeq &members
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  if (id != 0 && !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  this->string_pad (cdr,
                    ACE_OS::strlen (id) + 1);

  cdr << name;

  this->string_pad (cdr,
                    ACE_OS::strlen (name) + 1);

  CORBA::ULong len = members.length ();

  cdr << len;

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      ACE_CString ext_id (members[index]);
      int int_id = 0;

      // Is there a duplicate member name?
      if (map.trybind (ext_id, int_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (17,
                                              CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      cdr << members[index].in ();

      this->string_pad (cdr,
                        ACE_OS::strlen (members[index].in ()) + 1);
    }

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr enum_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (enum_typecode,
                    CORBA_TypeCode (CORBA::tk_enum,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
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
    CORBA::TypeCode_ptr original_type
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  if (id != 0 && !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::Boolean valid_content =
    this->valid_content_type (original_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!valid_content)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  this->string_pad (cdr,
                    ACE_OS::strlen (id) + 1);

  cdr << name;

  this->string_pad (cdr,
                    ACE_OS::strlen (name) + 1);

  cdr << original_type;

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr alias_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (alias_typecode,
                    CORBA_TypeCode (CORBA::tk_alias,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
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
    const CORBA_StructMemberSeq &members
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->struct_except_tc_common (id,
                                        name,
                                        members,
                                        CORBA::tk_except
                                        ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_objref
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_string_tc (
    CORBA::ULong bound
    ACE_ENV_ARG_DECL
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
    CORBA::ULong bound
    ACE_ENV_ARG_DECL
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
    CORBA::UShort /* scale */
    ACE_ENV_ARG_DECL_NOT_USED
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean valid_element =
    this->valid_content_type (element_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!valid_element)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << element_type;

  cdr << bound;

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr sequence_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (sequence_typecode,
                    CORBA_TypeCode (CORBA::tk_sequence,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return sequence_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean valid_element =
    this->valid_content_type (element_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!valid_element)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << element_type;

  cdr << length;

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr array_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (array_typecode,
                    CORBA_TypeCode (CORBA::tk_array,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
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
    const CORBA_ValueMemberSeq & /* members */
    ACE_ENV_ARG_DECL_NOT_USED
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
    CORBA::TypeCode_ptr boxed_type
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  // Repo id may not be null for valueboxtype.
  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::Boolean valid_content =
    this->valid_content_type (boxed_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (!valid_content)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  cdr << boxed_type;

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr value_box_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (value_box_typecode,
                    CORBA_TypeCode (CORBA::tk_value_box,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return value_box_typecode;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_native_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_native
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_recursive_tc (
    const char * /* id */
    ACE_ENV_ARG_DECL_NOT_USED
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_abstract_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_abstract_interface
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_local_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_local_interface
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_component_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_component
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_home_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->create_tc_common (id,
                                 name,
                                 CORBA::tk_home
                                 ACE_ENV_ARG_PARAMETER);
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
  struct disc_types
  {
    CORBA::Char char_val;
    CORBA::WChar wchar_val;
    CORBA::Boolean bool_val;
    CORBA::Short short_val;
    CORBA::UShort ushort_val;
    CORBA::Long long_val;
    CORBA::ULong ulong_val;
#if !defined (ACE_LACKS_LONGLONG_T)
    CORBA::ULongLong ulonglong_val;
#endif /* ACE_LACKS_LONGLONG_T */
    CORBA::ULong enum_val;
    // TODO - handle (u)longlong types
  } dv, u;

  dv.char_val = 0;
  dv.wchar_val = 0;
  dv.bool_val = 0;
  dv.short_val = ACE_INT16_MIN;
  dv.ushort_val = 0;
  dv.long_val = ACE_INT32_MIN;
  dv.ulong_val = 0;
#if !defined (ACE_LACKS_LONGLONG_T)
  dv.ulonglong_val = 0;
#endif /* ACE_LACKS_LONGLONG_T */
  dv.enum_val = 0;

  CORBA::ULong len = members.length ();
  int success = 0;

  // A collision forces us to start over, because the label
  // values need not be in sorted order.
  while (success == 0)
    {
      success = 1;

      for (CORBA::ULong i = 0; i < len; ++i)
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
#if !defined (ACE_LACKS_LONGLONG_T)
            case CORBA::tk_ulonglong:
              members[i].label >>= u.ulonglong_val;
              if (u.ulonglong_val == dv.ulonglong_val)
                {
                  dv.ulonglong_val++;
                  success = 0;
                }
              break;
#endif /* ACE_LACKS_LONGLONG_T */
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
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_ulonglong:
      cdr << dv.ulonglong_val;
      break;
#endif /* ACE_LACKS_LONGLONG_T */
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
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  // Repo id may not be null for object or native type.
  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  cdr << name;

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr result =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (result,
                    CORBA_TypeCode (kind,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return result;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::struct_except_tc_common (
    const char *id,
    const char *name,
    const CORBA_StructMemberSeq &members,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name != 0 && !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  // Repo id may not be null for an exception.
  if ((id == 0 && kind == CORBA::tk_except)
      || (id != 0 && !this->valid_id (id)))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  TAO_OutputCDR cdr;

  cdr << TAO_ENCAP_BYTE_ORDER;

  cdr << id;

  this->string_pad (cdr,
                    ACE_OS::strlen (id) + 1);

  cdr << name;

  this->string_pad (cdr,
                    ACE_OS::strlen (name) + 1);

  // Number of members..
  CORBA::ULong len = members.length ();

  cdr << len;

  CORBA::TypeCode_ptr tc_holder = CORBA::TypeCode::_nil ();
  CORBA::Boolean valid_member = 1;
  const char *member_name = 0;
  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      // Valid member type?
      tc_holder = members[index].type.in ();

      valid_member = this->valid_content_type (tc_holder
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      if (!valid_member)
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (2,
                                                 CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      member_name = members[index].name;

      if (member_name == 0 || !this->valid_name (member_name))
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (15,
                                              CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      ACE_CString ext_id (member_name);
      int int_id = 0;

      // Is there a duplicate member name?
      if (map.trybind (ext_id, int_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (17,
                                              CORBA::COMPLETED_NO),
                            CORBA::TypeCode::_nil ());
        }

      cdr << member_name;

      this->string_pad (cdr,
                        ACE_OS::strlen (member_name) + 1);

      cdr << tc_holder;
    }

  ACE_Message_Block consolidated_block;

  ACE_CDR::consolidate (&consolidated_block,
                        cdr.begin ());

  CORBA::TypeCode_ptr new_typecode =
    CORBA::TypeCode::_nil ();

  ACE_NEW_THROW_EX (new_typecode,
                    CORBA_TypeCode (kind,
                                    consolidated_block.length (),
                                    consolidated_block.rd_ptr (),
                                    0,
                                    0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return new_typecode;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::valid_name (const char *name)
{
  if (!isalpha (*name))
    {
      return 0;
    }

  const char *tmp = name + 1;

  for (; *tmp; ++tmp)
    {
      if (isalnum (*tmp) || *tmp == '_')
        {
          continue;
        }
      else
        {
          return 0;
        }
    }

  return 1;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::valid_id (const char *id)
{
  ACE_CString safety (id);

  int pos = safety.find (':');

  if (pos == ACE_CString::npos)
    {
      return 0;
    }

  ACE_CString format (safety.substr (0, pos));

  if (format == "IDL"
      || format == "RMI"
      || format == "DCE"
      || format == "LOCAL")
    {
      return 1;
    }

  return 0;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::valid_content_type (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (kind == CORBA::tk_null
      || kind == CORBA::tk_void
      || kind == CORBA::tk_except)
    {
      return 0;
    }

  return 1;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::unique_label_values (const CORBA_UnionMemberSeq &members,
                                            CORBA::TypeCode_ptr disc_tc,
                                            CORBA::ULong default_index_slot
                                            ACE_ENV_ARG_DECL)
{
  CORBA::TCKind disc_kind = disc_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = members.length ();

  // We have already checked for valid discriminator type
  // and valid label types, so we won't check for any of that here.

  struct label_types
  {
    CORBA::Char char_val;
    CORBA::WChar wchar_val;
    CORBA::Boolean bool_val;
    CORBA::Short short_val;
    CORBA::UShort ushort_val;
    CORBA::Long long_val;
    CORBA::ULong ulong_val;
    CORBA::ULong enum_val;
    // TODO - handle (u)longlong types
  } s;

  s.char_val = 0;
  s.wchar_val = 0;
  s.bool_val = 0;
  s.short_val = 0;
  s.ushort_val = 0;
  s.long_val = 0;
  s.ulong_val = 0;
  s.enum_val = 0;

  // Two cases - one for signed and one for unsigned discriminator types.
  if (disc_kind == CORBA::tk_long
      || disc_kind == CORBA::tk_short)
    {
      ACE_Bounded_Set<CORBA::Long> checker (length);

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          if (i == default_index_slot)
            {
              continue;
            }

          switch (disc_kind)
          {
            case CORBA::tk_long:
              members[i].label >>= s.long_val;

              if (checker.insert (s.long_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_short:
              members[i].label >>= s.short_val;

              if (checker.insert (s.short_val) != 0)
                {
                  return 0;
                }

              break;
            default:
              break;
          }
        }
    }
  else
    {
      ACE_Bounded_Set<CORBA::ULong> checker (length);

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          if (i == default_index_slot)
            {
              continue;
            }

          switch (disc_kind)
          {
            case CORBA::tk_boolean:
              members[i].label >>= CORBA::Any::to_boolean (s.bool_val);

              if (checker.insert (s.bool_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_char:
              members[i].label >>= CORBA::Any::to_char (s.char_val);

              if (checker.insert (s.char_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_wchar:
              members[i].label >>= CORBA::Any::to_wchar (s.wchar_val);

              if (checker.insert (s.wchar_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_ushort:
              members[i].label >>= s.ushort_val;

              if (checker.insert (s.ushort_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_ulong:
              members[i].label >>= s.ulong_val;

              if (checker.insert (s.ulong_val) != 0)
                {
                  return 0;
                }

              break;
            case CORBA::tk_enum:
            {
              TAO_InputCDR cdr (members[i].label._tao_get_cdr (),
                                members[i].label._tao_byte_order ());
              cdr.read_ulong (s.enum_val);

              if (checker.insert (s.enum_val) != 0)
                {
                  return 0;
                }

              break;
            }
            default:
              break;
          }
        }
    }

  return 1;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::valid_disc_type (CORBA::TypeCode_ptr tc
                                        ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (kind == CORBA::tk_enum
      || kind == CORBA::tk_ulong
      || kind == CORBA::tk_long
      || kind == CORBA::tk_ushort
      || kind == CORBA::tk_short
      || kind == CORBA::tk_char
      || kind == CORBA::tk_wchar
      || kind == CORBA::tk_boolean
      || kind == CORBA::tk_longlong
      || kind == CORBA::tk_ulonglong)
    {
      return 1;
    }

  return 0;
}

void
TAO_TypeCodeFactory_i::string_pad (TAO_OutputCDR &cdr,
                                   CORBA::ULong slen)
{
  CORBA::ULong padlen = (4 - (slen % 4)) % 4;

  for (CORBA::ULong i = 0; i < padlen; ++i)
    {
      cdr.write_char ('\0');
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, int>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash<ACE_CString>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex>;
template class ACE_Bounded_Set<CORBA::Long>;
template class ACE_Bounded_Set<CORBA::ULong>;
template class ACE_Bounded_Set_Iterator<CORBA::Long>;
template class ACE_Bounded_Set_Iterator<CORBA::ULong>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, int>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Equal_To<ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex>
#pragma instantiate ACE_Bounded_Set<CORBA::Long>
#pragma instantiate ACE_Bounded_Set<CORBA::ULong>
#pragma instantiate ACE_Bounded_Set_Iterator<CORBA::Long>
#pragma instantiate ACE_Bounded_Set_Iterator<CORBA::ULong>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
