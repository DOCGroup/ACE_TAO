#include "constant.h"

const char *exprtype_to_cpp_corba_type (AST_Expression::ExprType et)
{
  switch (et)
    {
    case AST_Expression::EV_short:
      return "CORBA::Short";
    case AST_Expression::EV_ushort:
      return "CORBA::UShort";
    case AST_Expression::EV_long:
      return "CORBA::Long";
    case AST_Expression::EV_ulong:
      return "CORBA::ULong";
    case AST_Expression::EV_float:
      return "CORBA::Float";
    case AST_Expression::EV_double:
      return "CORBA::Double";
    case AST_Expression::EV_char:
      return "CORBA::Char";
    case AST_Expression::EV_octet:
      return "CORBA::Octet";
    case AST_Expression::EV_bool:
      return "CORBA::Boolean";
    case AST_Expression::EV_string:
      return "char *const";
    case AST_Expression::EV_void:
      return "void";
    case AST_Expression::EV_none:
      return "none";
    case AST_Expression::EV_longlong:
      return "CORBA::LongLong";
    case AST_Expression::EV_ulonglong:
      return "CORBA::ULongLong";
    case AST_Expression::EV_wchar:
      return "CORBA::WChar";
    case AST_Expression::EV_wstring:
      return "CORBA::WChar *const";
    case AST_Expression::EV_fixed:
      return "Fixed";
    case AST_Expression::EV_int8:
      return "CORBA::Int8";
    case AST_Expression::EV_uint8:
      return "CORBA::UInt8";
    default:
      return 0;
    }

  return 0;
}
