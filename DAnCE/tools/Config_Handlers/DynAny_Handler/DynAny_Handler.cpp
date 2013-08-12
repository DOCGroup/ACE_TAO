// $Id$

#include /**/ "ace/pre.h"

#include "dance/Logger/Log_Macros.h"
#include "DynAny_Handler.h"
#include "DynEnum_Handler.h"
#include "DynSequence_Handler.h"
#include "DynStruct_Handler.h"
#include "DynAlias_Handler.h"
#include "DynArray_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "Common.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    DynAny_Handler::DynAny_Handler (void) :
      orb_ (0),
      daf_ (0)
    {
      DANCE_TRACE("DynAny_Handler::constructor");

      int argc = 0;
      char **argv = 0;

      orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var temp = orb_->resolve_initial_references (
        "DynAnyFactory");
      daf_ = DynamicAny::DynAnyFactory::_narrow (temp.in ());

      if (CORBA::is_nil (daf_.in ()))
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, ACE_TEXT ("Unable to narrow Dynamic Any factory\n")));
          throw Config_Error (
            ACE_TEXT (""), ACE_TEXT ("Unable to narrow DynAny factory"));
        }
    }

    DynAny_Handler::~DynAny_Handler (void)
    {
      DANCE_TRACE("DynAny_Handler::destructor");
    }

    CORBA::ORB_ptr
    DynAny_Handler::orb ()
    {
      DANCE_TRACE("DynAny_Handler::orb");

      return orb_.in ();
    }

    DynamicAny::DynAnyFactory_ptr
    DynAny_Handler::daf ()
    {
      DANCE_TRACE("DynAny_Handler::daf");

      return daf_.in ();
    }

    DynamicAny::DynAny_ptr
    DynAny_Handler::extract_into_dynany (const DataType &type,
                                         const DataValue &value,
                                         CORBA::TypeCode_ptr req_tc)
    {
      DANCE_TRACE("DynAny_Handler::extract_into_dynany");

      DynamicAny::DynAny_var retval;

      if (req_tc)
        retval = this->daf_->create_dyn_any_from_type_code (req_tc);

      TCKind::Value tkind = type.kind ().integral ();

      if ((value.count_short () && tkind != TCKind::tk_short_l) ||
          (value.count_long () && tkind != TCKind::tk_long_l) ||
          (value.count_ushort () && tkind != TCKind::tk_ushort_l ) ||
          (value.count_ulong () && tkind != TCKind::tk_ulong_l) ||
          (value.count_float () && tkind != TCKind::tk_float_l) ||
          (value.count_double () && tkind != TCKind::tk_double_l) ||
          (value.count_boolean () && tkind != TCKind::tk_boolean_l) ||
          (value.count_octet () && tkind != TCKind::tk_octet_l) ||
          (value.count_longlong () && tkind != TCKind::tk_longlong_l) ||
          (value.count_ulonglong () && tkind != TCKind::tk_ulonglong_l) ||
          (value.count_string () && !(tkind == TCKind::tk_string_l ||
                                      tkind == TCKind::tk_char_l ||
                                      tkind == TCKind::tk_wchar_l)))
        {
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for data type"));
        }

      switch (tkind)
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR,
            ACE_TEXT ("DynAny_Handler::extract_into_dynany - Don't know how to handle null or void types\n")));
          throw Config_Error (ACE_TEXT (""),
            ACE_TEXT ("Null or void types not supported"));
          break;

        case TCKind::tk_short_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_short);
              }
            CORBA::Short s = 0;
            if (value.count_short ())
              {
                s = *(*value.begin_short ());
              }
            retval->insert_short (s);
          }
          break;

        case TCKind::tk_long_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_long);
              }
            CORBA::Long l = 0;
            if (value.count_long ())
              {
                l = *(*value.begin_long ());
              }
            retval->insert_long (l);
          }
          break;

        case TCKind::tk_ushort_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ushort);
              }
            CORBA::UShort us = 0;
            if (value.count_ushort ())
              {
                us = *(*value.begin_ushort ());
              }
            retval->insert_ushort (us);
          }
          break;

        case TCKind::tk_ulong_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulong);
              }
            CORBA::ULong ul = 0;
            if (value.count_ulong ())
              {
                ul = *(*value.begin_ulong ());
              }
            retval->insert_ulong (ul);
          }
          break;

        case TCKind::tk_float_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_float);
              }
            CORBA::Float f = 0.0f;
            if (value.count_float ())
              {
                f = *(*value.begin_float ());
              }
            retval->insert_float (f);
          }
          break;

        case TCKind::tk_double_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_double);
              }
            CORBA::Double d = 0.0;
            if (value.count_double ())
              {
                d = *(*value.begin_double ());
              }
            retval->insert_double (d);
          }
          break;

        case TCKind::tk_boolean_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_boolean);
              }
            CORBA::Boolean b = false;
            if (value.count_boolean ())
              {
                b = *(*value.begin_boolean ());
              }
            retval->insert_boolean (b);
          }
          break;

        case TCKind::tk_octet_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_octet);
              }
            CORBA::Octet o = 0;
            if (value.count_octet ())
              {
                o = *(*(value.begin_octet ()));
              }
            retval->insert_octet (o);
          }
          break;

        case TCKind::tk_longlong_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_longlong);
              }
            CORBA::LongLong ll = 0;
            if (value.count_longlong ())
              {
                ll = *(*value.begin_longlong ());
              }
            retval->insert_longlong (ll);
          }
          break;

        case TCKind::tk_ulonglong_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulonglong);
              }
            CORBA::ULongLong ull = 0;
            if (value.count_ulonglong ())
              {
                ull = *(*value.begin_ulonglong ());
              }
            retval->insert_ulonglong (ull);
          }
          break;

        case TCKind::tk_string_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_string);
              }
            const char* s = 0;
            if (value.count_string ())
              {
                s = ACE_TEXT_ALWAYS_CHAR ((*value.begin_string ())->c_str ());
              }
            retval->insert_string (s);
          }
          break;

        case TCKind::tk_char_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_char);
              }
            CORBA::Char c = 0;
            if (value.count_string ())
              {
                c = *(*value.begin_string ())->c_str ();
              }
            retval->insert_char (c);
          }
          break;

        case TCKind::tk_wchar_l:
          {
            if (!req_tc)
              {
                retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_wchar);
              }
            CORBA::WChar wc = 0;
            if (value.count_string ())
              {
                wc = *(*value.begin_string ())->c_str ();
              }
            retval->insert_wchar (wc);
          }
          break;

        case TCKind::tk_enum_l:
          return DynEnum_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_sequence_l:
          return DynSequence_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_struct_l:
          return DynStruct_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_alias_l:
          return DynAlias_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_array_l:
          return DynArray_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_longdouble_l:
          // Not supported since a longdouble is defined in the xsd as double.
          // We are then creating a long double initialized with a regular
          // double. This is a very tricky conversion and doesn't work in
          // combination with certain (versions of) compilers.
        case TCKind::tk_wstring_l:
        case TCKind::tk_fixed_l:
        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_union_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
        case TCKind::tk_event_l:
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR, (LM_ERROR,
            DLINFO ACE_TEXT ("DynAny_Handler::extract_into_dynany - Type not supported\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Type not supported"));
        }

      return retval._retn ();
    }

    Any
    DynAny_Handler::extract_from_dynany (const CORBA::Any &any)
    {
      DANCE_TRACE("DynAny_Handler::extract_from_dynany");

      DynamicAny::DynAny_var dyn = this->daf_->create_dyn_any (any);

      DataValue val;
      Any retval (TCKind::tk_char, val);
      return retval;
    }

    CORBA::TypeCode_ptr
    DynAny_Handler::create_typecode (const DataType &type)
    {
      DANCE_TRACE("DynAny_Handler::create_typecode");

      switch (type.kind ().integral ())
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR, (LM_WARNING,
            ACE_TEXT ("I don't know how to handle null or void types\n")));
          throw Config_Error (ACE_TEXT (""),
            ACE_TEXT ("Null or void types not supported"));
          break;

        case TCKind::tk_short_l:
          return CORBA::_tc_short;
          break;

        case TCKind::tk_long_l:
          return CORBA::_tc_long;
          break;

        case TCKind::tk_ushort_l:
          return CORBA::_tc_ushort;
          break;

        case TCKind::tk_ulong_l:
          return CORBA::_tc_ulong;
          break;

        case TCKind::tk_float_l:
          return CORBA::_tc_float;
          break;

        case TCKind::tk_double_l:
          return CORBA::_tc_double;
          break;

        case TCKind::tk_boolean_l:
          return CORBA::_tc_boolean;
          break;

        case TCKind::tk_char_l:
          return CORBA::_tc_char;
          break;

        case TCKind::tk_octet_l:
          return CORBA::_tc_octet;
          break;

        case TCKind::tk_string_l:
          return CORBA::_tc_string;
          break;

        case TCKind::tk_longlong_l:
          return CORBA::_tc_longlong;
          break;

        case TCKind::tk_ulonglong_l:
          return CORBA::_tc_ulonglong;
          break;

        case TCKind::tk_longdouble_l:
          // Disabled since a longdouble is defined in the xsd as double.
          // We are then creating a long double initialized from a regular
          // double. This is a very tricky conversion and doesn't work in
          // combination with certain (versions of) compilers.
          break;

        case TCKind::tk_wchar_l:
          return CORBA::_tc_wchar;
          break;

        case TCKind::tk_wstring_l:
          break;

        case TCKind::tk_enum_l:
          return DynEnum_Handler::create_typecode (type);

        case TCKind::tk_struct_l:
          return DynStruct_Handler::create_typecode (type);

        case TCKind::tk_sequence_l:
          return DynSequence_Handler::create_typecode (type);

        case TCKind::tk_alias_l:
          return DynAlias_Handler::create_typecode (type);

        case TCKind::tk_array_l:
          return DynArray_Handler::create_typecode (type);

        case TCKind::tk_fixed_l:
        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_union_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
        case TCKind::tk_event_l:
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("DynAny_Handler::create_typecode - Type not supported\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Type not supported"));
        }

      return 0;
    }

    void
    DynAny_Handler::register_typecode (
      const std::basic_string<ACE_TCHAR> typeID,
      CORBA::TypeCode_ptr tc)
    {
      DANCE_TRACE("DynAny_Handler::register_typecode");

      this->typecode_map_[typeID] = tc;
    }


    CORBA::TypeCode_ptr
    DynAny_Handler::get_typecode (const std::basic_string<ACE_TCHAR> typeID)
    {
      DANCE_TRACE("DynAny_Handler::get_typecode");

      try
        {
          return this->typecode_map_ [typeID];
        }
      catch (...)
        {
          return 0;
        }
    }
  }
}
#include /**/ "ace/post.h"
