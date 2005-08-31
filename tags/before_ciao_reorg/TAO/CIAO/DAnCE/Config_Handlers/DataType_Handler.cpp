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

        if (kind == TCKind::tk_null)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
        else if (kind == TCKind::tk_short)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_short);
        else if (kind == TCKind::tk_long)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
        else if (kind == TCKind::tk_ushort)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
        else if (kind == TCKind::tk_ulong)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
        else if (kind == TCKind::tk_float)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_float);
        else if (kind == TCKind::tk_double)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_double);
        else if (kind == TCKind::tk_boolean)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
        else if (kind == TCKind::tk_char)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_char);
        else if (kind == TCKind::tk_octet)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_octet);
        else if (kind == TCKind::tk_string)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
        else if (kind == TCKind::tk_longlong)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);
        else if (kind == TCKind::tk_ulonglong)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);
        else if (kind == TCKind::tk_longdouble)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);
        else if (kind == TCKind::tk_wchar)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);
        else if (kind == TCKind::tk_wstring)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
        else if (kind == TCKind::tk_any)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_any);
        else if (kind == TCKind::tk_TypeCode)
          type = CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);

    //   This case used to be supported...is it not in the schema?
    //    else if (kind == TCKind::tk_Object)
    //      type = CORBA::TypeCode::_duplicate (CORBA::_tc_Object);*/

    }

    DataType
    DataType_Handler::data_type (
                      const CORBA::TypeCode_ptr&  src)
    {

        DataType type;

        if (src->kind () == ::CORBA::tk_null)
          type.kind (TCKind::tk_null);
        else if (src->kind () == CORBA::tk_short)
          type.kind (TCKind::tk_short);
        else if (src->kind () == CORBA::tk_long)
          type.kind (TCKind::tk_long);
        else if (src->kind () == CORBA::tk_ushort)
          type.kind (TCKind::tk_ushort);
        else if (src->kind () == CORBA::tk_ulong)
          type.kind (TCKind::tk_ulong);
        else if (src->kind () == CORBA::tk_float)
          type.kind (TCKind::tk_float);
        else if (src->kind () == CORBA::tk_double)
          type.kind (TCKind::tk_double);
        else if (src->kind () == CORBA::tk_boolean)
          type.kind (TCKind::tk_boolean);
        else if (src->kind () == CORBA::tk_char)
          type.kind (TCKind::tk_char);
        else if (src->kind () == CORBA::tk_octet)
          type.kind (TCKind::tk_octet);
        else if (src->kind () == CORBA::tk_string)
          type.kind (TCKind::tk_string);
        else if (src->kind () == CORBA::tk_longlong)
          type.kind (TCKind::tk_longlong);
        else if (src->kind () == CORBA::tk_ulonglong)
          type.kind (TCKind::tk_ulonglong);
        else if (src->kind () == CORBA::tk_longdouble)
          type.kind (TCKind::tk_longdouble);
        else if (src->kind () == CORBA::tk_wchar)
          type.kind (TCKind::tk_wchar);
        else if (src->kind () == CORBA::tk_wstring)
          type.kind (TCKind::tk_wstring);
        else if (src->kind () == CORBA::tk_any)
          type.kind (TCKind::tk_any);
        else if (src->kind () == CORBA::tk_TypeCode)
          type.kind (TCKind::tk_TypeCode);

        return type;
    }

  }
}
