// $Id$

#include "TypeCode_CDR_Extraction.h"

#include "tao/CDR.h"

#include "tao/TypeCode_Constants.h"
#include "tao/True_RefCount_Policy.h"

#include "tao/Alias_TypeCode.h"
#include "tao/Enum_TypeCode.h"
#include "tao/Fixed_TypeCode.h"
#include "Objref_TypeCode.h"
#include "tao/Sequence_TypeCode.h"
#include "tao/String_TypeCode.h"
#include "tao/Struct_TypeCode.h"
#include "tao/Union_TypeCode.h"
#include "tao/Value_TypeCode.h"

#include "tao/TypeCode_Case_T.h"
#include "tao/TypeCode_Struct_Field.h"
#include "tao/TypeCode_Value_Field.h"

#include "ace/Array_Base.h"


ACE_RCSID (tao,
           TypeCode_CDR_Extraction,
           "$Id$")


namespace
{
  bool start_cdr_encap_extraction (TAO_InputCDR & cdr)
  {
    CORBA::Boolean byte_order;

    // Don't bother demarshaling the encapsulation length.  Prefer
    // speed over early error checking.  Any CDR length related
    // failures will be detected when demarshaling the remainder of
    // the complex parameter list TypeCode.

    if (!(cdr.skip_ulong () // Skip encapsulation length.
          && cdr >> TAO_InputCDR::to_boolean (byte_order)))
      return false;

    cdr.reset_byte_order (byte_order);

    return true;
  }
}

bool
TAO::TypeCodeFactory::tc_null_factory (CORBA::TCKind,
                                       TAO_InputCDR &,
                                       CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  return true;
}

bool
TAO::TypeCodeFactory::tc_void_factory (CORBA::TCKind,
                                       TAO_InputCDR &,
                                       CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  return true;
}

bool
TAO::TypeCodeFactory::tc_short_factory (CORBA::TCKind,
                                        TAO_InputCDR &,
                                        CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_short);

  return true;
}

bool
TAO::TypeCodeFactory::tc_long_factory (CORBA::TCKind,
                                       TAO_InputCDR &,
                                       CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_long);

  return true;
}

bool
TAO::TypeCodeFactory::tc_ushort_factory (CORBA::TCKind,
                                         TAO_InputCDR &,
                                         CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);

  return true;
}

bool
TAO::TypeCodeFactory::tc_ulong_factory (CORBA::TCKind,
                                        TAO_InputCDR &,
                                        CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);

  return true;
}

bool
TAO::TypeCodeFactory::tc_float_factory (CORBA::TCKind,
                                        TAO_InputCDR &,
                                        CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_float);

  return true;
}

bool
TAO::TypeCodeFactory::tc_double_factory (CORBA::TCKind,
                                         TAO_InputCDR &,
                                         CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_double);

  return true;
}

bool
TAO::TypeCodeFactory::tc_boolean_factory (CORBA::TCKind,
                                          TAO_InputCDR &,
                                          CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);

  return true;
}

bool
TAO::TypeCodeFactory::tc_char_factory (CORBA::TCKind,
                                       TAO_InputCDR &,
                                       CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_char);

  return true;
}

bool
TAO::TypeCodeFactory::tc_octet_factory (CORBA::TCKind,
                                        TAO_InputCDR &,
                                        CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_octet);

  return true;
}

bool
TAO::TypeCodeFactory::tc_any_factory (CORBA::TCKind,
                                      TAO_InputCDR &,
                                      CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_any);

  return true;
}

bool
TAO::TypeCodeFactory::tc_TypeCode_factory (CORBA::TCKind,
                                           TAO_InputCDR &,
                                           CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);

  return true;
}

bool
TAO::TypeCodeFactory::tc_Principal_factory (CORBA::TCKind,
                                            TAO_InputCDR &,
                                            CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_Principal);

  return true;
}

bool
TAO::TypeCodeFactory::tc_objref_factory (CORBA::TCKind kind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_objref TypeCode is encoded in a CDR
  // encapsulation.
  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID and name.
  CORBA::String_var id;
  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)))
    return false;

  static char const Object_id[]    = "IDL:omg.org/CORBA/Object:1.0";
  static char const CCMObject_id[] = "IDL:omg.org/CORBA/CCMObject:1.0";
  static char const CCMHome_id[]   = "IDL:omg.org/CORBA/CCMHome:1.0";

  char const * tc_constant_id = "";

  switch (kind)
    {
    case CORBA::tk_component:
      tc_constant_id = CCMObject_id;
      break;
    case CORBA::tk_home:
      tc_constant_id = CCMHome_id;
      break;
    case CORBA::tk_objref:
      tc_constant_id = Object_id;
      break;
    default:
      break;
    }

  if (ACE_OS::strcmp (id.in (),  // len >= 0!!!
                      tc_constant_id) == 0)
    {
      if (!cdr.skip_string ())  // No need to demarshal the name.
        return false;

      CORBA::TypeCode_ptr tc_constant = CORBA::TypeCode::_nil ();
      switch (kind)
        {
        case CORBA::tk_component:
          tc_constant = CORBA::_tc_Component;
          break;
        case CORBA::tk_home:
          tc_constant = CORBA::_tc_Home;
          break;
        case CORBA::tk_objref:
          tc_constant = CORBA::_tc_Object;
          break;
        default:
          break;
        }

      // No need to create a TypeCode.  Just use the TypeCode
      // constant.
      tc =
        CORBA::TypeCode::_duplicate (tc_constant);
    }
  else
    {
      CORBA::String_var name;

      if (!(cdr >> TAO_InputCDR::to_string (name.out (), 0)))
        return false;

      typedef TAO::TypeCode::Objref<CORBA::String_var,
                                    TAO::True_RefCount_Policy> typecode_type;

      ACE_NEW_RETURN (tc,
                      typecode_type (kind,
                                     id.in (),
                                     name.in ()),
                      false);
    }

  return true;
}

bool
TAO::TypeCodeFactory::tc_struct_factory (CORBA::TCKind kind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  ACE_ASSERT (kind == CORBA::tk_struct || kind == CORBA::tk_except);

  // The remainder of a tk_struct/tk_except TypeCode is encoded in
  // a CDR encapsulation.
  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name and number of fields.
  CORBA::String_var id, name;
  CORBA::ULong nfields;

  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)
        && cdr >> TAO_InputCDR::to_string (name.out (), 0)
        && cdr >> nfields))
    return false;

  ACE_Array_Base<TAO::TypeCode::Struct_Field<CORBA::String_var,
                                             CORBA::TypeCode_var> >
    fields (nfields);

  for (CORBA::ULong i = 0; i < nfields; ++i)
    {
      if (!(cdr >> TAO_InputCDR::to_string (fields[i].name.out (), 0)
            && cdr >> fields[i].type.out ()))
        return false;
    }

  typedef TAO::TypeCode::Struct<
    CORBA::String_var,
    CORBA::TypeCode_var,
    ACE_Array_Base<TAO::TypeCode::Struct_Field<CORBA::String_var,
                                               CORBA::TypeCode_var> >,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (kind,
                                 id.in (),
                                 name.in (),
                                 fields,     // Will be copied.
                                 nfields),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_union_factory (CORBA::TCKind /* kind */,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_enum TypeCode is encoded in a CDR
  // encapsulation.

  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name, discriminant type, default index
  // and case count.
  CORBA::String_var id, name;
  CORBA::TypeCode_var discriminant_type;
  CORBA::Long default_index = -1;
  CORBA::ULong ncases = 0;  // Just 'n case :-)

  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)
        && cdr >> TAO_InputCDR::to_string (name.out (), 0)
        && cdr >> discriminant_type.out ()))
    return false;

  ACE_DECLARE_NEW_CORBA_ENV;
  CORBA::TCKind const discriminant_kind =
    discriminant_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  // Check for valid discriminant type.
  if (!(discriminant_kind == CORBA::tk_enum
        || discriminant_kind == CORBA::tk_ulong
        || discriminant_kind == CORBA::tk_long
        || discriminant_kind == CORBA::tk_ushort
        || discriminant_kind == CORBA::tk_short
        || discriminant_kind == CORBA::tk_char
        || discriminant_kind == CORBA::tk_boolean
        || discriminant_kind == CORBA::tk_longlong
        || discriminant_kind == CORBA::tk_ulonglong))
    return false;

  if (!(cdr >> default_index
        && default_index >= -1
        && cdr >> ncases))
    return false;

  // Use an ACE::Value_Ptr to provide exception safety and proper
  // copying semantics.
  typedef ACE::Value_Ptr<TAO::TypeCode::Case<CORBA::String_var,
                                             CORBA::TypeCode_var> > elem_type;
  typedef ACE_Array_Base<elem_type> case_array_type;

  case_array_type cases (ncases);

  for (CORBA::ULong i = 0; i < ncases; ++i)
    {
      elem_type & member = cases[i];

      TAO::TypeCode::Case<CORBA::String_var, CORBA::TypeCode_var> * the_case;

      // Ugly.  *sigh*
      switch (discriminant_kind)
        {
        case CORBA::tk_enum:  // Enumerators are encoded as unsigned longs.
        case CORBA::tk_ulong:
          {
            CORBA::ULong label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::ULong,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_long:
          {
            CORBA::Long label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::Long,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_ushort:
          {
            CORBA::UShort label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::UShort,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_short:
          {
            CORBA::Short label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::Short,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_char:
          {
            CORBA::Char label;
            if (!(cdr >> CORBA::Any::to_char (label)))
              return false;

            typedef TypeCode::Case_T<CORBA::Char,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_boolean:
          {
            CORBA::Boolean label;
            if (!(cdr >> CORBA::Any::to_boolean (label)))
              return false;

            typedef TypeCode::Case_T<CORBA::Boolean,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
        case CORBA::tk_longlong:
          {
            CORBA::LongLong label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::LongLong,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
#if !defined (ACE_LACKS_LONGLONG_T)
        case CORBA::tk_ulonglong:
          {
            CORBA::ULongLong label;
            if (!(cdr >> label))
              return false;

            typedef TypeCode::Case_T<CORBA::ULongLong,
                                     CORBA::String_var,
                                     CORBA::TypeCode_var> case_type;

            ACE_NEW_RETURN (the_case,
                            case_type (label),
                            false);
          }
          break;
#endif  /* !ACE_LACKS_LONGLONG_T */
        default:
          return false;
        }

      elem_type case_value (the_case);
      member.swap (case_value);  // Exception-safe

      CORBA::String_var the_name;
      CORBA::TypeCode_var the_type;

      if (!(cdr >> TAO_InputCDR::to_string (the_name.out (), 0)
            && cdr >> the_type.out ()))
        return false;

      member->name (the_name.in ());
      member->type (the_type.in ());
    }

  typedef TAO::TypeCode::Union<CORBA::String_var,
                               CORBA::TypeCode_var,
                               case_array_type,
                               TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (id.in (),
                                 name.in (),
                                 discriminant_type,
                                 cases,     // Will be copied.
                                 ncases,
                                 default_index),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_enum_factory (CORBA::TCKind /* kind */,
                                       TAO_InputCDR & cdr,
                                       CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_enum TypeCode is encoded in a CDR
  // encapsulation.

  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name and number of fields.
  CORBA::String_var id, name;
  CORBA::ULong nenumerators;

  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)
        && cdr >> TAO_InputCDR::to_string (name.out (), 0)
        && cdr >> nenumerators))
    return false;

  ACE_Array_Base<CORBA::String_var> enumerators (nenumerators);

  for (CORBA::ULong i = 0; i < nenumerators; ++i)
    {
      if (!(cdr >> TAO_InputCDR::to_string (enumerators[i].out (), 0)))
        return false;
    }

  typedef TAO::TypeCode::Enum<
    CORBA::String_var,
    ACE_Array_Base<CORBA::String_var>,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (id.in (),
                                 name.in (),
                                 enumerators,     // Will be copied.
                                 nenumerators),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_string_factory (CORBA::TCKind kind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  // A tk_string/tk_wstring TypeCode has a simple parameter list,
  // i.e. it is not encoded in a CDR encapsulation.

  CORBA::ULong bound;
  if (!(cdr >> bound))
    return false;

  if (bound == 0)
    {
      // Just use the TypeCode constant.
      if (kind == CORBA::tk_string)
        tc = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
      else if (kind == CORBA::tk_wstring)
        tc = CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
      else
        return false;

      return true;
    }

  typedef TAO::TypeCode::String<TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (kind, bound),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_sequence_factory (CORBA::TCKind kind,
                                           TAO_InputCDR & cdr,
                                           CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_sequence TypeCode is encoded in a CDR
  // encapsulation.

  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name and content type.
  CORBA::TypeCode_var content_type;
  CORBA::ULong length;

  if (!(cdr >> content_type.out ()
        && cdr >> length))
    return false;

  typedef TAO::TypeCode::Sequence<
    CORBA::TypeCode_var,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (kind, content_type, length),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_array_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  return tc_sequence_factory (kind, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_alias_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_alias or tk_value_box TypeCode is encoded
  // in a CDR encapsulation.

  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name and content type.
  CORBA::String_var id, name;
  CORBA::TypeCode_var content_type;
  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)
        && cdr >> TAO_InputCDR::to_string (name.out (), 0)
        && cdr >> content_type.out ()))
    return false;

  typedef TAO::TypeCode::Alias<
    CORBA::String_var,
    CORBA::TypeCode_var,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (kind,
                                 id.in (),
                                 name.in (),
                                 content_type),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_except_factory (CORBA::TCKind kind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  return tc_struct_factory (kind, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_longlong_factory (CORBA::TCKind,
                                           TAO_InputCDR &,
                                           CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);

  return true;
}

bool
TAO::TypeCodeFactory::tc_ulonglong_factory (CORBA::TCKind,
                                            TAO_InputCDR &,
                                            CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);

  return true;
}

bool
TAO::TypeCodeFactory::tc_longdouble_factory (CORBA::TCKind,
                                             TAO_InputCDR &,
                                             CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);

  return true;
}

bool
TAO::TypeCodeFactory::tc_wchar_factory (CORBA::TCKind,
                                        TAO_InputCDR &,
                                        CORBA::TypeCode_ptr & tc)
{
  tc = CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);

  return true;
}

bool
TAO::TypeCodeFactory::tc_wstring_factory (CORBA::TCKind kind,
                                          TAO_InputCDR & cdr,
                                          CORBA::TypeCode_ptr & tc)
{
  return tc_string_factory (kind, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_fixed_factory (CORBA::TCKind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  // A tk_fixed TypeCode has a simple parameter list, i.e. it is not
  // encoded in a CDR encapsulation.

  CORBA::UShort digits, scale;
  if (!(cdr >> digits && cdr >> scale))
    return false;

  typedef TAO::TypeCode::Fixed<TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (digits, scale),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_value_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  // The remainder of a tk_value/tk_event TypeCode is encoded in a
  // CDR encapsulation

  if (!start_cdr_encap_extraction (cdr))
    return false;

  // Extract the repository ID, name and number of fields.
  CORBA::String_var id;
  if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)))
    return false;

//   if (ACE_OS::strcmp (id.in (), "IDL:omg.org/CORBA/ValueBase:1.0") == 0)
//     {
//       // No need to create a TypeCode.  Just use the TypeCode
//       // constant.
//       tc = CORBA::TypeCode::_duplicate (CORBA::_tc_ValueBase);

//       return true;
//     }
//   else if (ACE_OS::strcmp (id.in (),
//                            "IDL:omg.org/CORBA/EventBase:1.0") == 0)
//     {
//       // No need to create a TypeCode.  Just use the TypeCode
//       // constant.
//       tc = CORBA::TypeCode::_duplicate (CORBA::_tc_EventBase);

//       return true;
//     }

  CORBA::String_var name;
  CORBA::ValueModifier type_modifier;
  CORBA::TypeCode_var concrete_base;
  CORBA::ULong nfields;

  if (!(cdr >> TAO_InputCDR::to_string (name.out (), 0)
        && cdr >> type_modifier
        && cdr >> concrete_base.out ()
        && cdr >> nfields))
    return false;

  ACE_Array_Base<TAO::TypeCode::Value_Field<CORBA::String_var,
                                            CORBA::TypeCode_var> >
    fields (nfields);

  for (CORBA::ULong i = 0; i < nfields; ++i)
    {
      TAO::TypeCode::Value_Field<CORBA::String_var,
                                 CORBA::TypeCode_var> & field = fields[i];

      if (!(cdr >> TAO_InputCDR::to_string (field.name.out (), 0)
            && cdr >> field.type.out ()
            && cdr >> field.visibility))
        return false;
    }

  typedef TAO::TypeCode::Value<
    CORBA::String_var,
    CORBA::TypeCode_var,
    ACE_Array_Base<TAO::TypeCode::Value_Field<CORBA::String_var,
                                              CORBA::TypeCode_var> >,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (kind,
                                 id.in (),
                                 name.in (),
                                 type_modifier,
                                 concrete_base,
                                 fields,     // Will be copied.
                                 nfields),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_value_box_factory (CORBA::TCKind kind,
                                            TAO_InputCDR & cdr,
                                            CORBA::TypeCode_ptr & tc)
{
  return tc_alias_factory (kind, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_native_factory (CORBA::TCKind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory (CORBA::tk_native, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_abstract_interface_factory (CORBA::TCKind,
                                                     TAO_InputCDR & cdr,
                                                     CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory (CORBA::tk_abstract_interface, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_local_interface_factory (CORBA::TCKind,
                                                  TAO_InputCDR & cdr,
                                                  CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory (CORBA::tk_local_interface, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_component_factory (CORBA::TCKind,
                                            TAO_InputCDR & cdr,
                                            CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory (CORBA::tk_component, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_home_factory (CORBA::TCKind,
                                       TAO_InputCDR & cdr,
                                       CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory (CORBA::tk_home, cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_event_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  return tc_value_factory (kind, cdr, tc);
}

// ----------------------------------------------------------------

bool
operator>> (TAO_InputCDR & cdr,
            CORBA::TypeCode_ptr & tc)
{
  CORBA::TCKind kind;
  if (!(cdr >> kind) || kind >= CORBA::TAO_TC_KIND_COUNT)
    return false;

  using namespace TAO::TypeCodeFactory;

  static factory const factory_map[] =
    {
      tc_null_factory,
      tc_void_factory,
      tc_short_factory,
      tc_long_factory,
      tc_ushort_factory,
      tc_ulong_factory,
      tc_float_factory,
      tc_double_factory,
      tc_boolean_factory,
      tc_char_factory,
      tc_octet_factory,
      tc_any_factory,
      tc_TypeCode_factory,
      tc_Principal_factory,
      tc_objref_factory,
      tc_struct_factory,
      tc_union_factory,
      tc_enum_factory,
      tc_string_factory,
      tc_sequence_factory,
      tc_array_factory,
      tc_alias_factory,
      tc_except_factory,
      tc_longlong_factory,
      tc_ulonglong_factory,
      tc_longdouble_factory,
      tc_wchar_factory,
      tc_wstring_factory,
      tc_fixed_factory,
      tc_value_factory,
      tc_value_box_factory,
      tc_native_factory,
      tc_abstract_interface_factory,
      tc_local_interface_factory,
      tc_component_factory,
      tc_home_factory,
      tc_event_factory
    };

  return factory_map[kind] (kind, cdr, tc);
}
