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
    void
    DataType_Handler::data_type (
                                CORBA::TypeCode_ptr& type,
                                const DataType& desc)
    {

        TCKind kind (desc.kind ());
        
        switch (kind.integral ())
          {
          case  TCKind::tk_null_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
            break;

          case  TCKind::tk_short_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_short);
            break;

          case  TCKind::tk_long_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
            break;

          case  TCKind::tk_ushort_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
            break;

          case  TCKind::tk_ulong_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
            break;

          case  TCKind::tk_float_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_float);
            break;

          case  TCKind::tk_double_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_double);
            break;

          case  TCKind::tk_boolean_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
            break;

          case  TCKind::tk_char_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_char);
            break;

          case  TCKind::tk_octet_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_octet);
            break;

          case  TCKind::tk_string_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
            break;

          case  TCKind::tk_longlong_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);
            break;

          case  TCKind::tk_ulonglong_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);
            break;

          case  TCKind::tk_longdouble_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);
            break;

          case  TCKind::tk_wchar_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);
            break;

          case  TCKind::tk_wstring_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
            break;

          case  TCKind::tk_any_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_any);
            break;

          case  TCKind::tk_TypeCode_l:
            type = CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);
            break;

          default:
            ACE_ERROR ((LM_ERROR, "Invalid typecode in any\n"));
            throw 1;
          }
        
        //   This case used to be supported...is it not in the schema?
    //    case  TCKind::tk_Object)
    //      type = CORBA::TypeCode::_duplicate (CORBA::_tc_Object);*/

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
            ACE_ERROR ((LM_ERROR, "Invalid typecode\n"));
            throw 1;
          }
        
        return type;
    }

  }
}
