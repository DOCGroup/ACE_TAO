// $Id$

#include "DynAlias_Handler.h"
#include "DynAny_Handler.h"
#include "Logger/Log_Macros.h"
#include "Basic_Deployment_Data.hpp"
#include "Common.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Enum_TypeCode.h"
#include "tao/IFR_Client/IFR_BasicC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    DynamicAny::DynAny_ptr
    DynAlias_Handler::extract_into_dynany (const DataType &type,
                                           const DataValue &value,
                                           CORBA::TypeCode_ptr req_tc)
    {
      try
        {
          CORBA::TypeCode_ptr tc;
            
          if (req_tc)
            tc = req_tc;
          else
            tc = DynAlias_Handler::create_typecode (type);
          
          // Make the actual DynAlias
          //          DynamicAny::DynAny_var retval =
          //DYNANY_HANDLER->daf ()->create_dyn_any_from_type_code (tc);
          //          DynamicAny::DynAlias_var retval = DynamicAny::DynAlias::_narrow (temp.in ());
          
          DynamicAny::DynAny_var alias = DYNANY_HANDLER->extract_into_dynany (type.alias ().elementType (), value, tc);
          return alias._retn ();
          /*
          switch (type.kind ().integral ())
            {
              // ========== BASIC TYPES
            case TCKind::tk_null_l:
            case TCKind::tk_void_l:
              DANCE_DEBUG (1, (LM_WARNING, "I don't know how to handle null or void types\n"));
              
            case TCKind::tk_short_l:
              retval->insert_short (CORBA::Short (static_cast < ::XMLSchema::short_ const & > (*value.begin_short ())));
              break;
              
            case TCKind::tk_long_l:
              retval->insert_long (CORBA::Long (static_cast < ::XMLSchema::int_ const& > (*value.begin_long ())));
              break;

            case TCKind::tk_ushort_l:
              retval->insert_ushort (CORBA::UShort (static_cast< ::XMLSchema::unsignedShort const & > (*value.begin_ushort ())));
              break;

            case TCKind::tk_ulong_l:
              retval->insert_ulong (CORBA::ULong (static_cast < ::XMLSchema::unsignedInt const& > (*value.begin_ulong ())));
              break;

            case TCKind::tk_float_l:
              retval->insert_float (CORBA::Float (*value.begin_float ()));
              break;

            case TCKind::tk_double_l:
              retval->insert_double (CORBA::Double (*value.begin_double ()));
              break;
              
            case TCKind::tk_boolean_l:
              retval->insert_boolean (static_cast < ::XMLSchema::boolean const& > (*value.begin_boolean ()));
              break;

            case TCKind::tk_char_l:
              retval->insert_char (CORBA::Char (*value.begin_string ()->c_str ()));
              break;
              
            case TCKind::tk_octet_l:
              retval->insert_octet (static_cast <const unsigned char &> (*value.begin_octet ()));
              break;

            case TCKind::tk_string_l:
              retval->insert_string (value.begin_string ()->c_str ());
              break;

            case TCKind::tk_longlong_l:
              retval->insert_longlong (CORBA::LongLong (static_cast < ::XMLSchema::long_ const& > (*value.begin_longlong ())));
              break;
              
            case TCKind::tk_ulonglong_l:
              retval->insert_ulonglong (CORBA::ULongLong (static_cast < ::XMLSchema::unsignedLong const& > (*value.begin_ulonglong ())));
              break;

            case TCKind::tk_wchar_l:
              retval->insert_wchar (CORBA::WChar (*value.begin_string ()->c_str ()));
              break;

        case TCKind::tk_enum_l:
          return DynEnum_Handler::extract_into_dynany (type, value);
          
        case TCKind::tk_sequence_l:
          return DynSequence_Handler::extract_into_dynany (type, value);
          
        case TCKind::tk_struct_l:
          return DynStruct_Handler::extract_into_dynany (type, value);

        case TCKind::tk_alias_l:
          return DynAlias_Handler::extract_into_dynany (type, value);

        case TCKind::tk_longdouble_l:
        case TCKind::tk_wstring_l:
        case TCKind::tk_wfixed_l:
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
          DANCE_DEBUG (1, (LM_ERROR, "Type not supported\n"));
          throw Config_Error ("", "Type not supported");
        }
          */
          //          retval->assign (alias);

          //          return retval._retn ();
        }
      catch (DynamicAny::DynAny::InvalidValue)
        {
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("Invalid value provided in XML when trying to ")
                      ACE_TEXT ("initialize an instance of enumerated type %s\n"),
                      type.alias ().typeId ().c_str ()));
          throw Config_Error (type.alias ().typeId (),
                              ACE_TEXT ("Invalid value provided in XML"));
        }
      catch (Config_Error &ex)
        {
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("DynAlias_Handler caught Config_Error\n")));
          if (type.alias_p ())
            ex.add_name (type.alias ().typeId ());

          throw ex;
        }
      catch (...)
        {
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("DynAlias_Handler caught unknown exception\n")));
          throw Config_Error (type.alias ().typeId (),
                              ACE_TEXT ("Unknown exception"));
        }
    }

    void
    DynAlias_Handler::extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn)
    {
      ACE_UNUSED_ARG (dyn);
      DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("Extracting Enums not yet supported\n")));
    }
    
    CORBA::TypeCode_ptr
    DynAlias_Handler::create_typecode (const DataType &type)
    {
      if (!type.alias_p ())
        {
          DANCE_DEBUG (1, (LM_ERROR, ACE_TEXT ("ERROR: Alias type description required")));
          throw Config_Error (ACE_TEXT (""), ACE_TEXT ("Did not find expected alias type description, tk_kind may be wrong."));
        }


      // @@ Leak this guy onto the heap to avoid a compile problem.
      CORBA::TypeCode_ptr tc =
        DYNANY_HANDLER->orb ()->create_alias_tc 
        (ACE_TEXT_ALWAYS_CHAR (type.alias ().typeId ().c_str ()),
         ACE_TEXT_ALWAYS_CHAR (type.alias ().name ().c_str ()),
         DYNANY_HANDLER->create_typecode (type.alias ().elementType ()));
      
      DYNANY_HANDLER->register_typecode (type.alias ().typeId (),
                                         tc);
      
      return tc;
    }
    
  }
}


