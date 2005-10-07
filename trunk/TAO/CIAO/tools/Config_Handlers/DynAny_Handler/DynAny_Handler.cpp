// $Id$

#include /**/ "ace/pre.h"

#include "DynAny_Handler.h"
#include "DynEnum_Handler.h"
#include "Basic_Deployment_Data.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynAny_Handler * DynAny_Handler::instance_ = 0;
    
    DynAny_Handler::DynAny_Handler (int argc, char **argv) :
      orb_ (CORBA::ORB_init (argc, argv)),
      daf_ (0)
    {
      CORBA::Object_var temp = this->orb_->resolve_initial_references ("DynAnyFactory");
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
    
    DynAny_Handler *
    DynAny_Handler::instance (void)
    {
      if (instance_ == 0)
        instance_ = new DynAny_Handler (0, 0);
      
      return instance_;
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
          /*
#ifdef __BORLANDC__
          CORBA::Float (static_cast < float & > (*value.begin_float_ ())));
#else
          CORBA::Float (static_cast < ::XMLSchema::float_ const& > (*value.begin_float_ ())));
#endif
          */
          break;
            
        case TCKind::tk_double_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_double);
          retval->insert_double (CORBA::Double (*value.begin_double ()));
          /*
#ifdef __BORLANDC__
            CORBA::Double (static_cast < double &> (*value.begin_double_ ())));
#else
          CORBA::Double (static_cast < ::XMLSchema::double_ const& > (*value.begin_double_ ())));
#endif
          */
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
//           retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_longdouble);
//           retval->insert_longdouble (CORBA::LongDouble (*value.begin_longdouble ()));
           break;
            
        case TCKind::tk_wchar_l:
          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_wchar);
          retval->insert_wchar (CORBA::WChar (*value.begin_string ()->c_str ()));
          break;
            
        case TCKind::tk_wstring_l:
          //          retval = this->daf_->create_dyn_any_from_type_code (CORBA::_tc_wstring);
          //retval->insert_wstring (CORBA::WString (*value.begin_string ().c_str ()));
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
    };
    
          //    Any Any_Handler::get_any (const ::CORBA::Any& src)
          //   {
          //ACE_ERROR ((LM_ERROR, "Output of Anys not yet supported."));
          // }
  }
}
#include /**/ "ace/post.h"
