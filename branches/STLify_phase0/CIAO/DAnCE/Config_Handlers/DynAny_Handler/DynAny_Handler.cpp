// $Id$

#include /**/ "ace/pre.h"

#include "Logger/Log_Macros.h"
#include "DynAny_Handler.h"
#include "DynEnum_Handler.h"
#include "DynSequence_Handler.h"
#include "DynStruct_Handler.h"
#include "DynAlias_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "Common.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynAny_Handler::DynAny_Handler (void) :
      orb_ (0),
      daf_ (0)
    {
      int argc = 0;
      char **argv = 0;

      orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var temp = orb_->resolve_initial_references ("DynAnyFactory");
      daf_ = DynamicAny::DynAnyFactory::_narrow (temp.in ());

      if (CORBA::is_nil (daf_.in ()))
        {
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("Unable to narrow Dynamic Any factory\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Unable to narrow DynAny factory"));
        }
    }

    DynAny_Handler::~DynAny_Handler (void)
    {
    }

    CORBA::ORB_ptr
    DynAny_Handler::orb ()
    {
      return orb_.in ();
    }

    DynamicAny::DynAnyFactory_ptr
    DynAny_Handler::daf ()
    {
      return daf_.in ();
    }

    DynamicAny::DynAny_ptr
    DynAny_Handler::extract_into_dynany (const DataType &type,
                                         const DataValue &value,
                                         CORBA::TypeCode_ptr req_tc)
    {
      DynamicAny::DynAny_var retval;

      if (req_tc)
        retval = this->daf_->create_dyn_any_from_type_code (req_tc);

      switch (type.kind ().integral ())
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          DANCE_DEBUG (1, (LM_WARNING, ACE_TEXT ("I don't know how to handle null or void types\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Null or void types not supported"));
          break;

        case TCKind::tk_short_l:
          if (!value.count_short ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_short data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_short);
          retval->insert_short (CORBA::Short (static_cast < ::XMLSchema::short_ const & > (*(*value.begin_short ()))));
          break;

        case TCKind::tk_long_l:
          if (!value.count_long ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_long data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_long);
          retval->insert_long (CORBA::Long (static_cast < ::XMLSchema::int_ const& > (*(*value.begin_long ()))));
          break;

        case TCKind::tk_ushort_l:
          if (!value.count_ushort ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_ushort data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ushort);
          retval->insert_ushort (CORBA::UShort (static_cast< ::XMLSchema::unsignedShort const & > (*(*value.begin_ushort ()))));
          break;

        case TCKind::tk_ulong_l:
          if (!value.count_ulong ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_ulong data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulong);
          retval->insert_ulong (CORBA::ULong (static_cast < ::XMLSchema::unsignedInt const& > (*(*value.begin_ulong ()))));
          break;

        case TCKind::tk_float_l:
          if (!value.count_float ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_float data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_float);
          retval->insert_float (CORBA::Float (*(*value.begin_float ())));
          break;

        case TCKind::tk_double_l:
          if (!value.count_double ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_double data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_double);
          retval->insert_double (CORBA::Double (*(*value.begin_double ())));
          break;

        case TCKind::tk_boolean_l:
          if (!value.count_boolean ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_boolean data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_boolean);
          retval->insert_boolean (static_cast < ::XMLSchema::boolean const& > (*(*value.begin_boolean ())));
          break;

        case TCKind::tk_char_l:
          if (!value.count_string ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_char data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_char);
          retval->insert_char (CORBA::Char (*(*value.begin_string ())->c_str ()));
          break;

        case TCKind::tk_octet_l:
          if (!value.count_octet ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_octet data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_octet);
          retval->insert_octet (static_cast <const unsigned char &> (*(*(value.begin_octet ()))));
          break;

        case TCKind::tk_string_l:
          if (!value.count_string ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_string data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_string);
          retval->insert_string (ACE_TEXT_ALWAYS_CHAR ((*value.begin_string ())->c_str ()));
          break;

        case TCKind::tk_longlong_l:
          if (!value.count_longlong ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_longlong data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_longlong);
          retval->insert_longlong (CORBA::LongLong (static_cast < ::XMLSchema::long_ const& > (*(*value.begin_longlong ()))));
          break;

        case TCKind::tk_ulonglong_l:
          if (!value.count_ulonglong ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_ulonglong data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulonglong);
          retval->insert_ulonglong (CORBA::ULongLong (static_cast < ::XMLSchema::unsignedLong const& > (*(*value.begin_ulonglong ()))));
          break;

        case TCKind::tk_wchar_l:
          if (!value.count_string ()) throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Wrong value type for tk_wchar data type"));
          if (!req_tc) retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_wchar);
          retval->insert_wchar (CORBA::WChar (*(*value.begin_string ())->c_str ()));
          break;

        case TCKind::tk_enum_l:
          return DynEnum_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_sequence_l:
          return DynSequence_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_struct_l:
          return DynStruct_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_alias_l:
          return DynAlias_Handler::extract_into_dynany (type, value, req_tc);

        case TCKind::tk_longdouble_l:
        case TCKind::tk_wstring_l:
        case TCKind::tk_fixed_l:
        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_union_l:
        case TCKind::tk_array_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
        case TCKind::tk_event_l:
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("Type not supported\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Type not supported"));
        }

      return retval._retn ();
    }

    Any
    DynAny_Handler::extract_from_dynany (const CORBA::Any &any)
    {
      DynamicAny::DynAny_var dyn = this->daf_->create_dyn_any (any);

      DataValue val;
#if 0
      switch (dyn->type ()->kind ())
        {
        case CORBA::tk_short:
          val.add_short (dyn->get_short ());
          return Any (TCKind::tk_short, val);

        case CORBA::tk_long:
          val.add_long (dyn->get_long ());
          return Any (TCKind::tk_long, val);

        case CORBA::tk_ushort:
          val.add_ushort (dyn->get_ushort ());
          return Any (TCKind::tk_ushort, val);

        case CORBA::tk_ulong:
          val.add_ulong (dyn->get_ulong ());
          return Any (TCKind::tk_ulong, val);

        case CORBA::tk_float:
          val.add_float (dyn->get_float ());
          return Any (TCKind::tk_float, val);

        case CORBA::tk_double:
          val.add_double (dyn->get_double ());
          return Any (TCKind::tk_double, val);

        case CORBA::tk_boolean:
          val.add_boolean (dyn->get_boolean ());
          return Any (TCKind::tk_boolean, val);

        case CORBA::tk_octet:
          val.add_octet (dyn->get_octet ());
          return Any (TCKind::tk_octet, val);

        case CORBA::tk_string:
          val.add_string (dyn->get_string ());
          return Any (TCKind::tk_string, val);

        case CORBA::tk_longlong:
          val.add_longlong (dyn->get_longlong ());
          return Any (TCKind::tk_longlong, val);

        case CORBA::tk_ulonglong:
          val.add_ulonglong (dyn->get_ulonglong ());
          return Any (TCKind::tk_ulonglong, val);

        case CORBA::tk_longdouble:
          //          return Any (TCKind::tk_longdouble);
          // @@MAJO: Need to add longdouble to schema.
          //val.add_longdouble (dyn->get_longdouble ());

        case CORBA::tk_wchar:
          //          return Any (TCKind::tk_wchar);
          // @@MAJO: Need to add wchar to schema
          //          val.add_wchar (dyn->get_wchar ());
        case CORBA::tk_char:
          // return Any (TCKind::tk_char);
          // @@MAJO: Need to add char into the schema.
          //          val.add_char (dyn->get_char ());

        case CORBA::tk_wstring:
        case CORBA::tk_any:
        case CORBA::tk_TypeCode:
        case ::CORBA::tk_null:
        default:
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("DynAny_Handler: I have no idea how to perform a referse mapping.\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("reverse mapping for this type not supported"));
        }
#endif
      Any retval (TCKind::tk_char, val);
      return retval;
    }

    CORBA::TypeCode_ptr 
    DynAny_Handler::create_typecode (const DataType &type)
    {

      switch (type.kind ().integral ())
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          DANCE_DEBUG (1, (LM_WARNING, ACE_TEXT ("I don't know how to handle null or void types\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Null or void types not supported"));
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

        case TCKind::tk_fixed_l:
        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_union_l:
        case TCKind::tk_array_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
        case TCKind::tk_event_l:
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("Type not supported\n")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Type not supported"));
        }

      return 0;
    }

    void 
    DynAny_Handler::register_typecode (const std::basic_string<ACE_TCHAR> typeID,
                                       CORBA::TypeCode_ptr tc)
    {
      this->typecode_map_[typeID] = tc;
    }


    CORBA::TypeCode_ptr
    DynAny_Handler::get_typecode (const std::basic_string<ACE_TCHAR> typeID)
    {
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
