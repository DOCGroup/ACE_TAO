//
// $Id$
//
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "structC.h"
#include <ace/streams.h>

using namespace StructTest;
using namespace DynamicAny;

//--------------------------------------------------------------------
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
//--------------------------------------------------------------------
{

  // Generic catch handler
  try {

    // Initialize the ORB
    // ------------------
    CORBA::ORB_var orb;  // _var, so we don't need/may not CORBA::release(orb)
    try {
      orb = CORBA::ORB_init (argc, argv);
    } catch (...) {
      cerr << "Cannot initialize ORB" << endl;
      throw;
    }

    // Get reference to the DynAny Factory
    CORBA::Object_var obj =  orb->resolve_initial_references("DynAnyFactory");

    DynAnyFactory_var daf =
      DynAnyFactory::_narrow(obj.in());

    MyStruct my_struct;
    MyStructAlias my_struct_alias;
    MyUnion my_union;
    MyUnionAlias my_union_alias;

    CORBA::Any any_struct;
    CORBA::Any any_struct_alias;
    CORBA::Any any_union;
    CORBA::Any any_union_alias;

    // Write the structs and unions to anys so we can get the TypeCode info
    any_struct <<= my_struct;
    any_struct_alias <<= my_struct_alias;
    any_union <<= my_union;
    any_union_alias <<= my_union_alias;

    // Explicitly set the TypeCode for the aliased types because the any
    // doesn't take care of aliases
    any_struct_alias.type(_tc_MyStructAlias);
    any_union_alias.type(_tc_MyUnionAlias);

    CORBA::TypeCode_var tc_struct = any_struct.type();
    CORBA::TypeCode_var tc_struct_alias = any_struct_alias.type();
    CORBA::TypeCode_var tc_union = any_union.type();
    CORBA::TypeCode_var tc_union_alias = any_union_alias.type();

    cout << "Type Code of the struct: " << tc_struct->kind() << endl;
    cout << "Type Code of the struct alias: " << tc_struct_alias->kind() << endl;
    cout << "Type Code of the union: " << tc_union->kind() << endl;
    cout << "Type Code of the union alias: " << tc_union_alias->kind() << endl;

    // equal returns true only when the TypeCodes are exactly the same.
    if (tc_struct->equal(tc_struct_alias.in())) {
      cout << "Type Codes are identical" << endl;
    } else {
      cout << "Type Codes are different" << endl;
    }
    // equivalent returns true when the TypeCode is an alias
    if (tc_struct->equivalent(tc_struct_alias.in())) {
      cout << "Type Codes are equivalent" << endl;
    } else {
      cout << "Type Codes are not equivalent" << endl;
    }

    DynAny_var da_struct = daf->create_dyn_any_from_type_code (tc_struct.in());

    try {
      DynAny_var da_struct_alias = daf->create_dyn_any_from_type_code (tc_struct_alias.in());
    } catch ( const CORBA::UNKNOWN &) {
      cout << "CORBA::UNKNOWN exception when calling create_dyn_any_from_type_code (tc_struct_alias)" << endl;
    }

    try {
      DynAny_var da_struct_alias = daf->create_dyn_any (any_struct_alias);
    } catch ( const CORBA::UNKNOWN &) {
      cout << "CORBA::UNKNOWN exception when calling create_dyn_any (any_struct_alias)" << endl;
    }

    DynAny_var da_union = daf->create_dyn_any_from_type_code (tc_union.in());

    try {
      DynAny_var da_union_alias = daf->create_dyn_any_from_type_code (tc_union_alias.in());
    } catch ( const CORBA::UNKNOWN &) {
      cout << "CORBA::UNKNOWN exception when calling create_dyn_any_from_type_code (tc_union_alias)" << endl;
    }

    try {
      DynAny_var da_union_alias = daf->create_dyn_any (any_union_alias);
    } catch ( const CORBA::UNKNOWN &) {
      cout << "CORBA::UNKNOWN exception when calling create_dyn_any (any_union_alias)" << endl;
    }

  } // end try

  catch (const CORBA::Exception &) {
    cerr << "Caught CORBA exception" << endl;
    return 1;
  }
  catch (...) {
    return 1;
  }
  return 0;
}
