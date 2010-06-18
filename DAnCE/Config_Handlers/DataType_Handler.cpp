//$Id$

#include "DataType_Handler.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace CIAO
{
  namespace Config_Handlers
  {

    DataType_Handler::DataType_Handler (void)
    {
    }
    DataType_Handler::~DataType_Handler (void)
    {
    }

    ///This method takes a <CIAO::Config_Handlers::DataType>
    ///and returns the corresponding CORBA::TypeCode.
    void
    DataType_Handler::data_type (const DataType& desc,
                                 CORBA::TypeCode_ptr& type)
    {
      DANCE_TRACE("DataType_Handler::data_type");
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
          DANCE_DEBUG (1, (LM_ERROR, "DataType_Handler::data_type, Invalid typecode in any\n"));
          throw 1;
        }

      //   This case used to be supported...is it not in the schema?
      //    case  TCKind::tk_Object)
      //      type = CORBA::TypeCode::_duplicate (CORBA::_tc_Object);*/

    }

    DataType
    DataType_Handler::data_type (const CORBA::TypeCode_ptr&  src)
    {
      DANCE_TRACE("DataType_Handler::data_type - reverse");
      switch (src->kind ())
        {
        case ::CORBA::tk_null:
          return DataType (TCKind::tk_null);
          break;
        case CORBA::tk_short:
          return DataType (TCKind::tk_short);
          break;
        case CORBA::tk_long:
          return DataType (TCKind::tk_long);
          break;
        case CORBA::tk_ushort:
          return DataType (TCKind::tk_ushort);
          break;
        case CORBA::tk_ulong:
          return DataType (TCKind::tk_ulong);
          break;
        case CORBA::tk_float:
          return DataType (TCKind::tk_float);
          break;
        case CORBA::tk_double:
          return DataType (TCKind::tk_double);
          break;
        case CORBA::tk_boolean:
          return DataType (TCKind::tk_boolean);
          break;
        case CORBA::tk_char:
          return DataType (TCKind::tk_char);
          break;
        case CORBA::tk_octet:
          return DataType (TCKind::tk_octet);
          break;
        case CORBA::tk_string:
          return DataType (TCKind::tk_string);
          break;
        case CORBA::tk_longlong:
          return DataType (TCKind::tk_longlong);
          break;
        case CORBA::tk_ulonglong:
          return DataType (TCKind::tk_ulonglong);
          break;
        case CORBA::tk_longdouble:
          return DataType (TCKind::tk_longdouble);
          break;
        case CORBA::tk_wchar:
          return DataType (TCKind::tk_wchar);
          break;
        case CORBA::tk_wstring:
          return DataType (TCKind::tk_wstring);
          break;
        case CORBA::tk_any:
          return DataType (TCKind::tk_any);
          break;
        case CORBA::tk_TypeCode:
          return DataType (TCKind::tk_TypeCode);
        default:
          DANCE_DEBUG (1, (LM_ERROR, "DataType_Handler::data_type, Invalid typecode\n"));
          throw 1;
        }
      throw 1;
    }
  }
}
