//==================================================================
/**
 *  @file  tckind_names.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "tckind_names.cpp"

const char* TCKind_Names::tc_kind_names[36];

static TCKind_Names __tc_kind_names__;

TCKind_Names::TCKind_Names()
{
  tc_kind_names[::CORBA::tk_null] = "tk_null";
  tc_kind_names[::CORBA::tk_void] = "tk_void";
  tc_kind_names[::CORBA::tk_short] = "tk_short";
  tc_kind_names[::CORBA::tk_long] = "tk_long";
  tc_kind_names[::CORBA::tk_ushort] = "tk_ushort";
  tc_kind_names[::CORBA::tk_ulong] = "tk_ulong";
  tc_kind_names[::CORBA::tk_float] = "tk_float";
  tc_kind_names[::CORBA::tk_double] = "tk_double";
  tc_kind_names[::CORBA::tk_boolean] = "tk_boolean";
  tc_kind_names[::CORBA::tk_char] = "tk_char";
  tc_kind_names[::CORBA::tk_octet] = "tk_octet";
  tc_kind_names[::CORBA::tk_any] = "tk_any";
  tc_kind_names[::CORBA::tk_TypeCode] = "tk_TypeCode";
  tc_kind_names[::CORBA::tk_Principal] = "tk_Principal";
  tc_kind_names[::CORBA::tk_objref] = "tk_objref";
  tc_kind_names[::CORBA::tk_struct] = "tk_struct";
  tc_kind_names[::CORBA::tk_union] = "tk_union";
  tc_kind_names[::CORBA::tk_enum] = "tk_enum";
  tc_kind_names[::CORBA::tk_string] = "tk_string";
  tc_kind_names[::CORBA::tk_sequence] = "tk_sequence";
  tc_kind_names[::CORBA::tk_array] = "tk_array";
  tc_kind_names[::CORBA::tk_alias] = "tk_alias";
  tc_kind_names[::CORBA::tk_except] = "tk_except";
  tc_kind_names[::CORBA::tk_longlong] = "tk_longlong";
  tc_kind_names[::CORBA::tk_ulonglong] = "tk_ulonglong";
  tc_kind_names[::CORBA::tk_longdouble] = "tk_longdouble";
  tc_kind_names[::CORBA::tk_wchar] = "tk_wchar";
  tc_kind_names[::CORBA::tk_wstring] = "tk_wstring";
  tc_kind_names[::CORBA::tk_fixed] = "tk_fixed";
  tc_kind_names[::CORBA::tk_value] = "tk_value";
  tc_kind_names[::CORBA::tk_value_box] = "tk_value_box";
  tc_kind_names[::CORBA::tk_native] = "tk_native";
  tc_kind_names[::CORBA::tk_abstract_interface] = "tk_abstract_interface";
  tc_kind_names[::CORBA::tk_local_interface] = "tk_local_interface";
  tc_kind_names[::CORBA::tk_component] = "tk_component";
  tc_kind_names[::CORBA::tk_home] = "tk_home";
};
