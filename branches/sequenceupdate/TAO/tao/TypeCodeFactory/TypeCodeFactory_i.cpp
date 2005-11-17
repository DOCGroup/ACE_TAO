// $Id$

#include "Recursive_TypeCode.h"
#include "TypeCodeFactory_i.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "tao/AnyTypeCode/Alias_TypeCode.h"
#include "tao/AnyTypeCode/Enum_TypeCode.h"
#include "tao/AnyTypeCode/Fixed_TypeCode.h"
#include "tao/AnyTypeCode/Objref_TypeCode.h"
#include "tao/AnyTypeCode/Sequence_TypeCode.h"
#include "tao/AnyTypeCode/String_TypeCode.h"
#include "tao/AnyTypeCode/Struct_TypeCode.h"
#include "tao/AnyTypeCode/Union_TypeCode.h"
#include "tao/AnyTypeCode/Value_TypeCode.h"

#include "tao/AnyTypeCode/Recursive_Type_TypeCode.h"

#include "tao/AnyTypeCode/TypeCode_Case_T.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/AnyTypeCode/TypeCode_Value_Field.h"

#include "tao/AnyTypeCode/True_RefCount_Policy.h"

#include "tao/IFR_Client/IFR_BasicC.h"

#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/SString.h"
#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include "ace/Value_Ptr.h"

#include "ace/os_include/os_ctype.h"


ACE_RCSID (TypeCodeFactory,
           TypeCodeFactory_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TCF
{
  namespace Struct
  {
    typedef
      ACE_Array_Base<TAO::TypeCode::Struct_Field<CORBA::String_var,
                                                 CORBA::TypeCode_var> >
        field_array_type;

    typedef TAO::TypeCode::Struct<CORBA::String_var,
                                  CORBA::TypeCode_var,
                                  field_array_type,
                                  TAO::True_RefCount_Policy> typecode_type;

    typedef TAO::TypeCode::Recursive_Type<typecode_type,
                                          CORBA::TypeCode_var,
                                          field_array_type>
      recursive_typecode_type;

  }  // End namespace Struct

  namespace Union
  {
    // Use an ACE::Value_Ptr to provide exception safety and proper
    // copying semantics.
    typedef ACE::Value_Ptr<TAO::TypeCode::Case_Dynamic> elem_type;
    typedef ACE_Array_Base<elem_type> case_array_type;

    typedef TAO::TypeCode::Union<CORBA::String_var,
                                 CORBA::TypeCode_var,
                                 case_array_type,
                                 TAO::True_RefCount_Policy> typecode_type;

    typedef TAO::TypeCode::Recursive_Type<
      typecode_type,
      CORBA::TypeCode_var,
      case_array_type> recursive_typecode_type;
  }  // End namespace Union

  namespace Value
  {
    typedef
      ACE_Array_Base<TAO::TypeCode::Value_Field<CORBA::String_var,
                                                CORBA::TypeCode_var> >
        field_array_type;

    typedef TAO::TypeCode::Value<CORBA::String_var,
                                 CORBA::TypeCode_var,
                                 field_array_type,
                                 TAO::True_RefCount_Policy> typecode_type;

    typedef TAO::TypeCode::Recursive_Type<typecode_type,
                                          CORBA::TypeCode_var,
                                          field_array_type>
      recursive_typecode_type;
  }  // End namespace Value
}


TAO_TypeCodeFactory_i::TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i::~TAO_TypeCodeFactory_i (void)
{
}

TAO_TypeCodeFactory_i *
TAO_TypeCodeFactory_i::_narrow (CORBA::Object_ptr _tao_objref
                                ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_TypeCodeFactory_i *> (_tao_objref);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_struct_tc (
    const char *id,
    const char *name,
    const CORBA::StructMemberSeq &members
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
    const CORBA::UnionMemberSeq &members
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::Boolean const good_disc_type =
    this->valid_disc_type (discriminator_type
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  if (!good_disc_type)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 20,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::ULong const len = members.length ();
  CORBA::ULong dups = 0;
  CORBA::ULong raw_default_index = ACE_UINT32_MAX;
  CORBA::Long default_index = -1;
  CORBA::Octet value = ACE_OCTET_MAX;
  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  // No getting around iterating over the members twice. We have
  // to do it once *before* the overall length is written to the
  // CDR stream, to know by how much, if any, the number of members
  // differs from the number of labels.
  //
  // @@ Now that the TypeCode implementation has been rewritten, do we
  // still need to iterate over the members twice?
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      CORBA::UnionMember const & member = members[i];
      char const * const member_name = member.name;

      int trybind_status = 0;

      if (i > 0)
        {
          // Is this a duplicate case label? If so, we have to adjust
          // the 'length' we encode - a member gets encoded only once.
          if (ACE_OS::strcmp (member_name, members[i - 1].name) == 0)
            {
              CORBA::Boolean const equiv =
                member.type.in ()->equivalent (members[i - 1].type.in ()
                                               ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (tc);

              // If adjacent names are the same and their types are
              // equivalent, then they are duplicate case labels. If
              // the types are not equivalent, then they are separate
              // members with the same name, which is not allowed.
              if (equiv)
                {
                  ++dups;
                }
              else
                {
                  ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 17,
                                                      CORBA::COMPLETED_NO),
                                    tc);
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
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 17,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      CORBA::TypeCode_ptr const tc_holder = member.type.in ();

      // Valid member type?
      CORBA::Boolean const valid_member =
        this->valid_content_type (tc_holder
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      if (!valid_member)
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 2,
                                                 CORBA::COMPLETED_NO),
                            tc);
        }

      // Reset the default index, if we have a default case label.
      if ((member.label >>= CORBA::Any::to_octet (value)) == 1)
        {
          if (value == 0)
            {
              raw_default_index = i;
              // Only the multiple labels that come before the
              // default label affect its adjusted value.
              default_index = static_cast<CORBA::Long> (i - dups);
            }
        }
      else
        {
          // Else check that the label type is equivalent to the
          // given discriminator type.
          CORBA::TypeCode_var const tmp = member.label.type ();

          CORBA::Boolean const equiv =
            discriminator_type->equivalent (tmp.in ()
                                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (tc);

          if (!equiv)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                  CORBA::COMPLETED_NO),
                                tc);
            }
        }
    }

  CORBA::Boolean const unique_labels =
    this->unique_label_values (members,
                               discriminator_type,
                               raw_default_index
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  if (!unique_labels)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 18,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  using namespace TCF::Union;

  case_array_type cases (len - dups);

  CORBA::TCKind const kind =
    discriminator_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  CORBA::ULong ci = 0;  // Case array index.

  bool is_recursive = false;
  CORBA::TypeCode_var recursive_tc;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      CORBA::UnionMember const & member = members[index];

      if (index > 0)
        {
          // Is this a duplicate case label? If so, skip it - a member
          // goes into the TypeCode only once.
          if (ACE_OS::strcmp (member.name,
                              members[index - 1].name) == 0)
            {
              continue;
            }
        }

      // Check if recursive.
      bool const recursion_detected =
        this->check_recursion (CORBA::tk_union,
                               id,
                               member.type.in (),
                               recursive_tc.inout ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      // Do not clobber previous positive detection.
      if (recursion_detected)
        is_recursive = true;

      elem_type & element = cases[ci];

      TAO::TypeCode::Case_Dynamic * the_case = 0;

      if (index == raw_default_index)
        {
          // This is the default label - we have to find a legal value.
          this->compute_default_label (kind,
                                       index,
                                       members,
                                       the_case);

          if (the_case == 0)
            {
              // Should never throw since label kind was
              // verified earlier.
              ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                  CORBA::COMPLETED_NO),
                                CORBA::TypeCode::_nil ());
            }
        }
      else
        {
          // Ugly.  *sigh*
          switch (kind)
            {
            case CORBA::tk_enum:
              {
                TAO::Any_Impl * const impl = member.label.impl ();
                TAO_InputCDR for_reading (
                  static_cast<ACE_Message_Block *> (0));

                if (impl->encoded ())
                  {
                    TAO::Unknown_IDL_Type * const unk =
                      dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

                    // We don't want unk's rd_ptr to move, in case we
                    // are shared by another Any, so we use this to
                    // copy the state, not the buffer.
                    for_reading = unk->_tao_get_cdr ();
                  }
                else
                  {
                    TAO_OutputCDR out;
                    impl->marshal_value (out);
                    TAO_InputCDR tmp (out);
                    for_reading = tmp;
                  }

                CORBA::ULong label;
                for_reading.read_ulong (label);

                typedef TAO::TypeCode::Case_T<CORBA::ULong,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_ulong:
              {
                CORBA::ULong label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::ULong,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_long:
              {
                CORBA::Long label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::Long,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_ushort:
              {
                CORBA::UShort label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::UShort,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_short:
              {
                CORBA::Short label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::Short,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_char:
              {
                CORBA::Char label;
                if (!(member.label >>= CORBA::Any::to_char (label)))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::Char,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_boolean:
              {
                CORBA::Boolean label;
                if (!(member.label >>= CORBA::Any::to_boolean (label)))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::Boolean,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
            case CORBA::tk_longlong:
              {
                CORBA::LongLong label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::LongLong,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
#if !defined (ACE_LACKS_LONGLONG_T)
            case CORBA::tk_ulonglong:
              {
                CORBA::ULongLong label;
                if (!(member.label >>= label))
                  {
                    // Should never throw since label kind was
                    // verified earlier.
                    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 19,
                                                        CORBA::COMPLETED_NO),
                                      CORBA::TypeCode::_nil ());
                  }

                typedef TAO::TypeCode::Case_T<CORBA::ULongLong,
                                              CORBA::String_var,
                                              CORBA::TypeCode_var> case_type;

                ACE_NEW_THROW_EX (the_case,
                                  case_type (label),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
              }
              break;
#endif  /* !ACE_LACKS_LONGLONG_T */
            default:
              ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 20,
                                                  CORBA::COMPLETED_NO),
                                CORBA::TypeCode::_nil ());
            }
        }

      ++ci;

      elem_type case_value (the_case);
      element.swap (case_value);  // Exception-safe

      element->name (member.name.in ());
      element->type (member.type.in ());
    }

  // @@ Blame this on MSVC++ 6 workarounds.  *sigh*
  CORBA::TypeCode_var duped_disc_type (
    CORBA::TypeCode::_duplicate (discriminator_type));

  if (is_recursive)
    {

      recursive_typecode_type * const rtc =
        dynamic_cast<recursive_typecode_type *> (recursive_tc.in ());

      ACE_ASSERT (rtc);

      rtc->union_parameters (name,
                             duped_disc_type,
                             cases,     // Will be copied.
                             cases.size (),
                             default_index);

      return recursive_tc._retn ();
    }

  ACE_NEW_THROW_EX (tc,
                    typecode_type (id,
                                   name,
                                   duped_disc_type,
                                   cases,     // Will be copied.
                                   cases.size (),
                                   default_index),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_enum_tc (
    const char *id,
    const char *name,
    const CORBA::EnumMemberSeq &members
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::ULong const len = members.length ();

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  ACE_Array_Base<CORBA::String_var> enumerators (len);

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      ACE_CString ext_id (members[index]);
      int int_id = 0;

      // Is there a duplicate member name?
      if (map.trybind (ext_id, int_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 17,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      enumerators[index] = members[index];
    }

  typedef TAO::TypeCode::Enum<
    CORBA::String_var,
    ACE_Array_Base<CORBA::String_var>,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_THROW_EX (tc,
                    typecode_type (id,
                                   name,
                                   enumerators,
                                   len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
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
  return this->alias_value_box_tc_common (id,
                                          name,
                                          original_type,
                                          CORBA::tk_alias
                                          ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_exception_tc (
    const char *id,
    const char *name,
    const CORBA::StructMemberSeq &members
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
  return this->string_wstring_tc_common (bound,
                                         CORBA::tk_string
                                         ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_wstring_tc (
    CORBA::ULong bound
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->string_wstring_tc_common (bound,
                                         CORBA::tk_wstring
                                         ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_fixed_tc (
    CORBA::UShort digits,
    CORBA::UShort scale
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc;
  ACE_NEW_THROW_EX (tc,
                    TAO::TypeCode::Fixed<TAO::True_RefCount_Policy> (digits,
                                                                     scale),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return sequence_array_tc_common (bound,
                                   element_type,
                                   CORBA::tk_sequence
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return sequence_array_tc_common (length,
                                   element_type,
                                   CORBA::tk_array
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_value_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq &members
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_event_tc_common (id,
                                      name,
                                      type_modifier,
                                      concrete_base,
                                      members,
                                      CORBA::tk_value
                                      ACE_ENV_ARG_PARAMETER);
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
  return this->alias_value_box_tc_common (id,
                                          name,
                                          boxed_type,
                                          CORBA::tk_value_box
                                          ACE_ENV_ARG_PARAMETER);
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
    const char *id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  ACE_NEW_THROW_EX (tc,
                    TAO::TypeCodeFactory::Recursive_TypeCode (id),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
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

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::create_event_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq &members
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_event_tc_common (id,
                                      name,
                                      type_modifier,
                                      concrete_base,
                                      members,
                                      CORBA::tk_event
                                      ACE_ENV_ARG_PARAMETER);
}

// =====================================================================

void
TAO_TypeCodeFactory_i::compute_default_label (
  CORBA::TCKind kind,
  CORBA::ULong skip_slot,
  const CORBA::UnionMemberSeq &members,
  TAO::TypeCode::Case_Dynamic *& the_case)
{
  // One to hold the current default value, one to
  // hold the curent label's extracted value.
  struct disc_types
  {
    CORBA::Char char_val;
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
  dv.bool_val = 0;
  dv.short_val = ACE_INT16_MIN;
  dv.ushort_val = 0;
  dv.long_val = ACE_INT32_MIN;
  dv.ulong_val = 0;
#if !defined (ACE_LACKS_LONGLONG_T)
  dv.ulonglong_val = 0;
#endif /* ACE_LACKS_LONGLONG_T */
  dv.enum_val = 0;

  CORBA::ULong const len = members.length ();
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
            {
              continue;
            }

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
            case CORBA::tk_boolean:
              members[i].label >>= CORBA::Any::to_boolean (u.bool_val);

              if (u.bool_val == dv.bool_val)
                {
                  dv.bool_val = !dv.bool_val;
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
              TAO::Any_Impl *impl = members[i].label.impl ();
              TAO_InputCDR for_reading (static_cast<ACE_Message_Block *> (0));

              if (impl->encoded ())
                {
                  TAO::Unknown_IDL_Type *unk =
                    dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

                  // We don't want unk's rd_ptr to move, in case
                  // we are shared by another Any, so we use this
                  // to copy the state, not the buffer.
                  for_reading = unk->_tao_get_cdr ();
                }
              else
                {
                  TAO_OutputCDR out;
                  impl->marshal_value (out);
                  TAO_InputCDR tmp (out);
                  for_reading = tmp;
                }

              for_reading.read_ulong (u.enum_val);

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
            {
              break;
            }
        }
    }

  // Add the default value to the encapsulation.
  switch (kind)
  {
    case CORBA::tk_char:
      {
        typedef TAO::TypeCode::Case_T<CORBA::Char,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.char_val));
      }
      break;
    case CORBA::tk_boolean:
      {
        typedef TAO::TypeCode::Case_T<CORBA::Boolean,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.bool_val));
      }
      break;
    case CORBA::tk_short:
      {
        typedef TAO::TypeCode::Case_T<CORBA::Short,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.short_val));
      }
      break;
    case CORBA::tk_ushort:
      {
        typedef TAO::TypeCode::Case_T<CORBA::UShort,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.ushort_val));
      }
      break;
    case CORBA::tk_long:
      {
        typedef TAO::TypeCode::Case_T<CORBA::Long,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.long_val));
      }
      break;
    case CORBA::tk_ulong:
      {
        typedef TAO::TypeCode::Case_T<CORBA::ULong,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.ulong_val));
      }
      break;
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_ulonglong:
      {
        typedef TAO::TypeCode::Case_T<CORBA::ULongLong,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.ulonglong_val));
      }
      break;
#endif /* ACE_LACKS_LONGLONG_T */
    case CORBA::tk_enum:
      {
        // Enumerators are encoded as CORBA::ULong.
        typedef TAO::TypeCode::Case_T<CORBA::ULong,
                                      CORBA::String_var,
                                      CORBA::TypeCode_var> case_type;

        ACE_NEW (the_case,
                 case_type (dv.enum_val));
      }
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
  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  // Repo id may not be null for object or native type.
  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::TypeCode_ptr tc;

  typedef TAO::TypeCode::Objref<CORBA::String_var,
                                TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_THROW_EX (tc,
                    typecode_type (kind, id, name),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::string_wstring_tc_common (
    CORBA::ULong bound,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc;
  ACE_NEW_THROW_EX (tc,
                    TAO::TypeCode::String<TAO::True_RefCount_Policy> (kind,
                                                                      bound),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::sequence_array_tc_common (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  CORBA::Boolean const valid_element =
    this->valid_content_type (element_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  if (!valid_element)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 2,
                                             CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::TypeCode_var tmp (CORBA::TypeCode::_duplicate (element_type));

  typedef TAO::TypeCode::Sequence<CORBA::TypeCode_var,
                                  TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_THROW_EX (tc,
                    typecode_type (kind, tmp, bound),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::struct_except_tc_common (
    const char *id,
    const char *name,
    const CORBA::StructMemberSeq & members,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::ULong const len = members.length ();

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  using namespace TCF::Struct;

  field_array_type fields (len);

  bool is_recursive = false;

  CORBA::TypeCode_var recursive_tc;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      // Valid member type?
      CORBA::TypeCode_ptr const member_tc = members[index].type.in ();

      CORBA::Boolean const valid_member =
        this->valid_content_type (member_tc
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      if (!valid_member)
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 2,
                                                 CORBA::COMPLETED_NO),
                            tc);
        }

      char const * const member_name = members[index].name;

      if (member_name == 0 || !this->valid_name (member_name))
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      ACE_CString ext_id (member_name);
      int int_id = 0;

      // Is there a duplicate member name?
      if (map.trybind (ext_id, int_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 17,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      TAO::TypeCode::Struct_Field<CORBA::String_var,
                                  CORBA::TypeCode_var> & field = fields[index];

      // Check if recursive.
      bool const recursion_detected =
        this->check_recursion (kind,
                               id,
                               member_tc,
                               recursive_tc.inout ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      // Do not clobber previous positive detection.
      if (recursion_detected)
        is_recursive = true;

      field.name = member_name;
      field.type = CORBA::TypeCode::_duplicate (member_tc);
    }

  if (is_recursive)
    {
      recursive_typecode_type * const rtc =
        dynamic_cast<recursive_typecode_type *> (recursive_tc.in ());

      ACE_ASSERT (rtc);

      rtc->struct_parameters (name, fields, len);

      return recursive_tc._retn ();
    }

  ACE_NEW_THROW_EX (tc,
                    typecode_type (kind,
                                   id,
                                   name,
                                   fields,
                                   len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::alias_value_box_tc_common (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr underlying_type,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::Boolean const valid_content =
    this->valid_content_type (underlying_type
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  if (!valid_content)
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 2,
                                             CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::TypeCode_var tmp (CORBA::TypeCode::_duplicate (underlying_type));

  typedef TAO::TypeCode::Alias<CORBA::String_var,
    CORBA::TypeCode_var,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_THROW_EX (tc,
                    typecode_type (kind, id, name, tmp),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::value_event_tc_common (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq &members,
    CORBA::TCKind kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if (name == 0 || !this->valid_name (name))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  if (id == 0 || !this->valid_id (id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 16,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::ULong const len = members.length ();

  using namespace TCF::Value;

  field_array_type fields (len);

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> map;

  bool is_recursive = false;
  CORBA::TypeCode_var recursive_tc;

  for (CORBA::ULong index = 0; index < len; ++index)
    {
      // Valid member type?
      CORBA::TypeCode_ptr const member_tc = members[index].type.in ();

      CORBA::Boolean const valid_member =
        this->valid_content_type (member_tc
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      if (!valid_member)
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 2,
                                                 CORBA::COMPLETED_NO),
                            tc);
        }

      const char * const member_name = members[index].name;

      if (member_name == 0 || !this->valid_name (member_name))
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 15,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      ACE_CString ext_id (member_name);
      int int_id = 0;

      // Is there a duplicate member name?
      if (map.trybind (ext_id, int_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 17,
                                              CORBA::COMPLETED_NO),
                            tc);
        }

      // Check if recursive.
      bool const recursion_detected =
        this->check_recursion (kind,
                               id,
                               member_tc,
                               recursive_tc.inout ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc);

      // Do not clobber previous positive detection.
      if (recursion_detected)
        is_recursive = true;

      TAO::TypeCode::Value_Field<CORBA::String_var,
                                 CORBA::TypeCode_var> & field = fields[index];

      field.name       = member_name;
      field.type       = CORBA::TypeCode::_duplicate (member_tc);
      field.visibility = members[index].access;
    }

  CORBA::TypeCode_var tmp (
    CORBA::TypeCode::_duplicate (CORBA::is_nil (concrete_base)
                                 ? CORBA::_tc_null
                                 : concrete_base));

  if (is_recursive)
    {
      recursive_typecode_type * const rtc =
        dynamic_cast<recursive_typecode_type *> (recursive_tc.in ());

      ACE_ASSERT (rtc);

      rtc->valuetype_parameters (name,
                                 type_modifier,
                                 tmp,
                                 fields,
                                 len);

      return recursive_tc._retn ();
    }

  ACE_NEW_THROW_EX (tc,
                    typecode_type (kind,
                                   id,
                                   name,
                                   type_modifier,
                                   tmp,
                                   fields,
                                   len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (tc);

  return tc;
}

CORBA::Boolean
TAO_TypeCodeFactory_i::valid_name (const char *name)
{
  // Empty string is valid for name.
  if (*name == '\0')
    {
      return 1;
    }

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
  ACE_CString safety (id,
                      0,
                      0);

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
  CORBA::TCKind const kind =
    TAO::unaliased_kind (tc
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  switch (kind)
  {
    case CORBA::TAO_TC_KIND_COUNT:
      return 1;  // Recursive TypeCode.
    case CORBA::tk_void:
    case CORBA::tk_except:
      return 0;
    default:
      return 1;
  }
}

CORBA::Boolean
TAO_TypeCodeFactory_i::unique_label_values (
  const CORBA::UnionMemberSeq &members,
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
    CORBA::Boolean bool_val;
    CORBA::Short short_val;
    CORBA::UShort ushort_val;
    CORBA::Long long_val;
    CORBA::ULong ulong_val;
    CORBA::ULong enum_val;
    CORBA::ULongLong ulonglong_val;
    CORBA::LongLong longlong_val;
  } s = {0, 0, 0, 0, 0, 0, 0, 0, ACE_CDR_LONGLONG_INITIALIZER };

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
              TAO::Any_Impl *impl = members[i].label.impl ();
              TAO_InputCDR for_reading (static_cast<ACE_Message_Block *> (0));

              if (impl->encoded ())
                {
                  TAO::Unknown_IDL_Type *unk =
                    dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

                  // We don't want unk's rd_ptr to move, in case
                  // we are shared by another Any, so we use this
                  // to copy the state, not the buffer.
                  for_reading = unk->_tao_get_cdr ();
                }
              else
                {
                  TAO_OutputCDR out;
                  impl->marshal_value (out);
                  TAO_InputCDR tmp (out);
                  for_reading = tmp;
                }

              for_reading.read_ulong (s.enum_val);

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
  CORBA::TCKind const kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return
    (kind == CORBA::tk_enum
     || kind == CORBA::tk_ulong
     || kind == CORBA::tk_long
     || kind == CORBA::tk_ushort
     || kind == CORBA::tk_short
     || kind == CORBA::tk_char
     || kind == CORBA::tk_boolean
     || kind == CORBA::tk_longlong
     || kind == CORBA::tk_ulonglong);
}

bool
TAO_TypeCodeFactory_i::check_recursion (CORBA::TCKind kind,
                                        char const * id,
                                        CORBA::TypeCode_ptr member,
                                        CORBA::TypeCode_ptr & recursive_tc
                                        ACE_ENV_ARG_DECL)
{
  if (kind != CORBA::tk_struct
      && kind != CORBA::tk_union
      && kind != CORBA::tk_value
      && kind != CORBA::tk_event)
    return false;

  CORBA::TypeCode_var unaliased_member =
    TAO::unaliased_typecode (member
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  CORBA::TCKind const unaliased_member_kind =
    unaliased_member->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  // Recursively iterate through the member and content types until
  // we've exhausted all TypeCodes capable of containing other
  // TypeCodes.
  switch (unaliased_member_kind)
    {
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_value:
    case CORBA::tk_event:
      {
        CORBA::ULong const nfields =
          unaliased_member->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (false);

        for (CORBA::ULong i = 0; i < nfields; ++i)
          {
            CORBA::TypeCode_var member_tc =
              unaliased_member->member_type (i
                                             ACE_ENV_ARG_PARAMETER);
            ACE_CHECK_RETURN (false);

            CORBA::TCKind const member_tc_kind =
              member_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK_RETURN (false);

            if (member_tc_kind == CORBA::TAO_TC_KIND_COUNT)
              {
                // Valuetypes can directly contain a recursive member
                // (e.g. valuetype V { public V member; };).  Check if
                // the member TypeCode is the recursive TypeCode
                // placeholder.
                if (kind == CORBA::tk_value || kind == CORBA::tk_event)
                  {
                    char const * member_tc_id =
                      member_tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
                    ACE_CHECK_RETURN (false);

                    if (ACE_OS::strcmp (id, member_tc_id) == 0)
                      {
                        TAO::TypeCodeFactory::Recursive_TypeCode * const rtc =
                          dynamic_cast<
                            TAO::TypeCodeFactory::Recursive_TypeCode *> (
                              member_tc.in ());

                        ACE_ASSERT (rtc);

                        if (CORBA::is_nil (recursive_tc))
                          {
                            recursive_tc =
                              this->make_recursive_tc (kind,
                                                       id
                                                       ACE_ENV_ARG_PARAMETER);
                            ACE_CHECK_RETURN (false);
                          }

                        // Set the actual recursive TypeCode.
                        rtc->the_typecode (recursive_tc);

                        return true;
                      }

                    // Different recursive TypeCode.  Let it be.
                  }
                else
                  {
                    // @@ structs and unions may not directly contain
                    //    recursive members.  They must be indirectly
                    //    recursive through a member sequence (which
                    //    itself may be contained inside a nested
                    //    struct, union, etc).
                        ACE_THROW_RETURN (CORBA::BAD_TYPECODE (), false);
                  }
              }
            else
              {
                bool const recursion_detected =
                  this->check_recursion (kind,
                                         id,
                                         member_tc.in (),
                                         recursive_tc
                                         ACE_ENV_ARG_PARAMETER);
                ACE_CHECK_RETURN (false);

                if (recursion_detected)
                  {
                    return true;
                  }
              }

            // Not recursive or not the recursive TypeCode we want.
            // Try the next member.
          }
      }
      break;

    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
        CORBA::TypeCode_var content_tc =
          unaliased_member->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (false);

        CORBA::TCKind const content_tc_kind =
          content_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (false);

        if (content_tc_kind == CORBA::TAO_TC_KIND_COUNT)
          {
            char const * content_tc_id =
              content_tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK_RETURN (false);

            if (ACE_OS::strcmp (id, content_tc_id) == 0)
              {
                TAO::TypeCodeFactory::Recursive_TypeCode * const rtc =
                  dynamic_cast<TAO::TypeCodeFactory::Recursive_TypeCode *>
                  (content_tc.in ());

                ACE_ASSERT (rtc);

                if (CORBA::is_nil (recursive_tc))
                  {
                    recursive_tc =
                      this->make_recursive_tc (kind,
                                               id
                                               ACE_ENV_ARG_PARAMETER);
                    ACE_CHECK_RETURN (false);
                  }

                // Set the actual recursive TypeCode.
                rtc->the_typecode (recursive_tc);

                return true;
              }

            // Different recursive TypeCode.  Let it be.
          }

        return this->check_recursion (kind,
                                      id,
                                      content_tc.in (),
                                      recursive_tc
                                      ACE_ENV_ARG_PARAMETER);
      }

    default:
      break;  // Not a recursion-capable TypeCode.
    }

  return false;
}

CORBA::TypeCode_ptr
TAO_TypeCodeFactory_i::make_recursive_tc (CORBA::TCKind kind,
                                          char const * id
                                          ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  switch (kind)
    {
    case CORBA::tk_struct:
      ACE_NEW_THROW_EX (tc,
                        TCF::Struct::recursive_typecode_type (kind, id),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (tc);
      break;

    case CORBA::tk_union:
      ACE_NEW_THROW_EX (tc,
                        TCF::Union::recursive_typecode_type (kind, id),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (tc);
      break;

    case CORBA::tk_value:
    case CORBA::tk_event:
      ACE_NEW_THROW_EX (tc,
                        TCF::Value::recursive_typecode_type (kind, id),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (tc);
      break;

    default:  // Should never hit this case.
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        tc);
    }

  return tc;
}


TAO_END_VERSIONED_NAMESPACE_DECL
