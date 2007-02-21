// $Id$

#include "DynSequence_Handler.h"
#include "DynAny_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Sequence_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynSequence_Handler::extract_into_dynany (const DataType &type,
                                          const DataValue &value)
    {
      CORBA::TypeCode_ptr tc = DynSequence_Handler::create_typecode (type);
      
      // Make the actual DynSequence
      DynamicAny::DynAny_var temp =
        DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
      DynamicAny::DynSequence_var retval = DynamicAny::DynSequence::_narrow (temp.in ());
      
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

    void
    DynSequence_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      ACE_ERROR ((LM_ERROR, "Extracting Sequences not yet supported\n"));
    }
    
    CORBA::TypeCode_ptr 
    DynSequence_Handler::create_typecode (const DataType &type)
    {
      if (!type.sequence_p ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Sequence type descriptioin required"));
          throw 1;
        }
      
      CORBA::TypeCode_ptr etc = 
        DYNANY_HANDLER->create_typecode (type.sequence ().elementType ());
      
      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_sequence_tc (type.sequence ().bound ()
                                                    etc);
      
      // Sequences don't have names.
      //      DYNANY_HANDLER->register_typecode (type.sequence_ ().typeId (),
      //                                   tc);
      
      return tc;
    }
    
  }
}


