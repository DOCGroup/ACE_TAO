#include "Factory_Map.h"

#include "Objref_TypeCode_Factory.h"

#include "tao/CDR.h"

#include "tao/TypeCode_Constants.h"
#include "tao/True_RefCount_Policy.h"

#include "tao/Alias_TypeCode.h"
#include "tao/Enum_TypeCode.h"
#include "tao/Fixed_TypeCode.h"
#include "tao/Objref_TypeCode.h"
#include "tao/Sequence_TypeCode.h"
#include "tao/String_TypeCode.h"
#include "tao/Struct_TypeCode.h"
//#include "tao/Union_TypeCode.h"
#include "tao/Value_TypeCode.h"

#include "tao/TypeCode_Struct_Field.h"
#include "tao/TypeCode_Value_Field.h"


ACE_RCSID (TypeCodeFactory,
           Factory_Map,
           "$Id$")


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
TAO::TypeCodeFactory::tc_objref_factory (CORBA::TCKind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_objref> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_struct_factory (CORBA::TCKind kind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  ACE_ASSERT (kind == CORBA::tk_struct || kind == CORBA::tk_except);

  CORBA::Boolean byte_order;

  // The remainder of a tk_struct/tk_except TypeCode is encoded in
  // a CDR encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
TAO::TypeCodeFactory::tc_union_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & /* cdr */,
                                        CORBA::TypeCode_ptr & /* tc */)
{
  ACE_ASSERT (kind == CORBA::tk_union);
  ACE_ASSERT (0);   // @@ Temporarily unimplemented.

  return false;  // @@ Temporarily unimplemented.
}

bool
TAO::TypeCodeFactory::tc_enum_factory (CORBA::TCKind kind,
                                       TAO_InputCDR & cdr,
                                       CORBA::TypeCode_ptr & tc)
{
  ACE_ASSERT (kind == CORBA::tk_enum);

  CORBA::Boolean byte_order;

  // The remainder of a tk_enum TypeCode is encoded in a CDR
  // encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
  CORBA::Boolean byte_order;

  // The remainder of a tk_sequence TypeCode is encoded in a CDR
  // encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
TAO::TypeCodeFactory::tc_alias_factory (CORBA::TCKind  /* kind */,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  CORBA::Boolean byte_order;

  // The remainder of a tk_alias TypeCode is encoded in a CDR
  // encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
    CORBA::tk_alias,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (id.in (),
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
  CORBA::Boolean byte_order;

  // The remainder of a tk_value/tk_event TypeCode is encoded in a
  // CDR encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
TAO::TypeCodeFactory::tc_value_box_factory (CORBA::TCKind /* kind */,
                                            TAO_InputCDR & cdr,
                                            CORBA::TypeCode_ptr & tc)
{
  CORBA::Boolean byte_order;

  // The remainder of a tk_value_box TypeCode is encoded in a CDR
  // encapsulation.
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (byte_order);

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
    CORBA::tk_value_box,
    TAO::True_RefCount_Policy> typecode_type;

  ACE_NEW_RETURN (tc,
                  typecode_type (id.in (),
                                 name.in (),
                                 content_type),
                  false);

  return true;
}

bool
TAO::TypeCodeFactory::tc_native_factory (CORBA::TCKind,
                                         TAO_InputCDR & cdr,
                                         CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_native> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_abstract_interface_factory (CORBA::TCKind,
                                                     TAO_InputCDR & cdr,
                                                     CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_abstract_interface> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_local_interface_factory (CORBA::TCKind,
                                                  TAO_InputCDR & cdr,
                                                  CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_local_interface> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_component_factory (CORBA::TCKind,
                                            TAO_InputCDR & cdr,
                                            CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_component> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_home_factory (CORBA::TCKind,
                                       TAO_InputCDR & cdr,
                                       CORBA::TypeCode_ptr & tc)
{
  return tc_objref_factory<CORBA::tk_home> (cdr, tc);
}

bool
TAO::TypeCodeFactory::tc_event_factory (CORBA::TCKind kind,
                                        TAO_InputCDR & cdr,
                                        CORBA::TypeCode_ptr & tc)
{
  return tc_value_factory (kind, cdr, tc);
}
