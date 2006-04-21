// $Id$

#include /**/ "ace/pre.h"

#include "DynAny_Handler.h"
#include "DynEnum_Handler.h"
#include "Basic_Deployment_Data.hpp"

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
          ACE_ERROR ((LM_ERROR, "Unable to narrow Dynamic Any factory\n"));
          throw 1;
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
    DynAny_Handler::extract_into_dynany (const DataType& type,
                                         const DataValue& value)
    {
      DynamicAny::DynAny_var retval;

      switch (type.kind ().integral ())
        {
          // ========== BASIC TYPES
        case TCKind::tk_null_l:
        case TCKind::tk_void_l:
          ACE_ERROR ((LM_WARNING, "I don't know how to handle null or void types\n"));

        case TCKind::tk_short_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_short);
          retval->insert_short (CORBA::Short (static_cast < ::XMLSchema::short_ const & > (*value.begin_short ())));
          break;

        case TCKind::tk_long_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_long);
          retval->insert_long (CORBA::Long (static_cast < ::XMLSchema::int_ const& > (*value.begin_long ())));
          break;

        case TCKind::tk_ushort_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ushort);
          retval->insert_ushort (CORBA::UShort (static_cast< ::XMLSchema::unsignedShort const & > (*value.begin_ushort ())));
          break;

        case TCKind::tk_ulong_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulong);
          retval->insert_ulong (CORBA::ULong (static_cast < ::XMLSchema::unsignedInt const& > (*value.begin_ulong ())));
          break;

        case TCKind::tk_float_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_float);
          retval->insert_float (CORBA::Float (*value.begin_float ()));
          break;

        case TCKind::tk_double_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_double);
          retval->insert_double (CORBA::Double (*value.begin_double ()));
          break;

        case TCKind::tk_boolean_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_boolean);
          retval->insert_boolean (static_cast < ::XMLSchema::boolean const& > (*value.begin_boolean ()));
          break;

        case TCKind::tk_char_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_char);
          retval->insert_char (CORBA::Char (*value.begin_string ()->c_str ()));
          break;

        case TCKind::tk_octet_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_octet);
          retval->insert_octet (static_cast <const unsigned char &> (*value.begin_octet ()));
          break;

        case TCKind::tk_string_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_string);
          retval->insert_string (value.begin_string ()->c_str ());
          break;

        case TCKind::tk_longlong_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_longlong);
          retval->insert_longlong (CORBA::LongLong (static_cast < ::XMLSchema::long_ const& > (*value.begin_longlong ())));
          break;

        case TCKind::tk_ulonglong_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_ulonglong);
          retval->insert_ulonglong (CORBA::ULongLong (static_cast < ::XMLSchema::unsignedLong const& > (*value.begin_ulonglong ())));
          break;

         case TCKind::tk_longdouble_l:
           break;

        case TCKind::tk_wchar_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_wchar);
          retval->insert_wchar (CORBA::WChar (*value.begin_string ()->c_str ()));
          break;

        case TCKind::tk_wstring_l:
          break;

        case TCKind::tk_enum_l:
          ACE_ERROR ((LM_ERROR, "Preparing to extract into enum\n"));
          return DynEnum_Handler::extract_into_dynany (type, value);

        case TCKind::tk_wfixed_l:
        case TCKind::tk_any_l:
        case TCKind::tk_TypeCode_l:
        case TCKind::tk_Principal_l:
        case TCKind::tk_objref_l:
        case TCKind::tk_struct_l:
        case TCKind::tk_union_l:
        case TCKind::tk_sequence_l:
        case TCKind::tk_array_l:
        case TCKind::tk_alias_l:
        case TCKind::tk_except_l:
        case TCKind::tk_value_l:
        case TCKind::tk_value_box_l:
        case TCKind::tk_native_l:
        case TCKind::tk_abstract_interface_l:
        case TCKind::tk_local_interface_l:
        case TCKind::tk_component_l:
        case TCKind::tk_home_l:
        case TCKind::tk_event_l:
          ACE_ERROR ((LM_ERROR, "Type not supported\n"));
          throw 1;
        }

      return retval._retn ();
    }

    Any
    DynAny_Handler::extract_from_dynany (const CORBA::Any &any)
    {
      DynamicAny::DynAny_var dyn = this->daf_->create_dyn_any (any);

      DataValue val;

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
          ACE_ERROR ((LM_ERROR, "DynAny_Handler: I have no idea how to perform a referse mapping.\n"));
          throw 1;
        }

    }
  }
}
#include /**/ "ace/post.h"
