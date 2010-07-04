//$Id$

#include "DataType_Handler.h"
#include "tao/AnyTypeCode/TypeCode.h"

namespace CIAO{

  namespace Config_Handlers{

    DataType_Handler::DataType_Handler (void)
    {
    }
    DataType_Handler::~DataType_Handler (void)
    {
    }

    ///This method takes a <CIAO::Config_Handlers::DataType>
    ///and returns the corresponding CORBA::TypeCode.
    CORBA::TypeCode_ptr
    DataType_Handler::data_type (const DataType& desc)
    {

        TCKind kind (desc.kind ());

        switch (kind.integral ())
          {
          case  TCKind::tk_null_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_null);


          case  TCKind::tk_short_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_short);


          case  TCKind::tk_long_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_long);


          case  TCKind::tk_ushort_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);


          case  TCKind::tk_ulong_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);


          case  TCKind::tk_float_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_float);


          case  TCKind::tk_double_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_double);


          case  TCKind::tk_boolean_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);


          case  TCKind::tk_char_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_char);


          case  TCKind::tk_octet_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_octet);


          case  TCKind::tk_string_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_string);


          case  TCKind::tk_longlong_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);


          case  TCKind::tk_ulonglong_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);


          case  TCKind::tk_longdouble_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);


          case  TCKind::tk_wchar_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);


          case  TCKind::tk_wstring_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);


          case  TCKind::tk_any_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_any);


          case  TCKind::tk_TypeCode_l:
            return CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);


          default:
            DANCE_DEBUG (1, (LM_ERROR, "Invalid typecode in any\n"));
            throw Config_Error ("", "Invalid typecode");
          }

        //   This case used to be supported...is it not in the schema?
    //    case  TCKind::tk_Object)
    //      return CORBA::TypeCode::_duplicate (CORBA::_tc_Object);*/

    }

    DataType
    DataType_Handler::data_type (
                      const CORBA::TypeCode_ptr&  src)
    {
        DataType type;

        switch (src->kind ())
          {
          case ::CORBA::tk_null:
            type.kind (TCKind::tk_null);
            break;
          case CORBA::tk_short:
            type.kind (TCKind::tk_short);
            break;
          case CORBA::tk_long:
            type.kind (TCKind::tk_long);
            break;
          case CORBA::tk_ushort:
            type.kind (TCKind::tk_ushort);
            break;
          case CORBA::tk_ulong:
            type.kind (TCKind::tk_ulong);
            break;
          case CORBA::tk_float:
            type.kind (TCKind::tk_float);
            break;
          case CORBA::tk_double:
            type.kind (TCKind::tk_double);
            break;
          case CORBA::tk_boolean:
            type.kind (TCKind::tk_boolean);
            break;
          case CORBA::tk_char:
            type.kind (TCKind::tk_char);
            break;
          case CORBA::tk_octet:
            type.kind (TCKind::tk_octet);
            break;
          case CORBA::tk_string:
            type.kind (TCKind::tk_string);
            break;
          case CORBA::tk_longlong:
            type.kind (TCKind::tk_longlong);
            break;
          case CORBA::tk_ulonglong:
            type.kind (TCKind::tk_ulonglong);
            break;
          case CORBA::tk_longdouble:
            type.kind (TCKind::tk_longdouble);
            break;
          case CORBA::tk_wchar:
            type.kind (TCKind::tk_wchar);
            break;
          case CORBA::tk_wstring:
            type.kind (TCKind::tk_wstring);
            break;
          case CORBA::tk_any:
            type.kind (TCKind::tk_any);
            break;
          case CORBA::tk_TypeCode:
            type.kind (TCKind::tk_TypeCode);
          default:
            DANCE_DEBUG (1, (LM_ERROR, "Invalid typecode\n"));
            throw Config_Error ("", "Invalid typecode");
          }

        return type;
    }
    

  }
}
