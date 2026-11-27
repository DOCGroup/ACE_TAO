#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "structC.h"
#include <iostream>

using namespace StructTest;
using namespace DynamicAny;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb;
      try
        {
          orb = CORBA::ORB_init (argc, argv);
        }
      catch (...)
        {
          std::cerr << "Cannot initialize ORB" << std::endl;
          throw;
        }

      CORBA::Object_var const object =
        orb->resolve_initial_references ("DynAnyFactory");

      DynAnyFactory_var const factory =
        DynAnyFactory::_narrow (object.in ());

      MyStruct my_struct {};
      MyStructAlias my_struct_alias {};
      MyUnion my_union {};
      MyUnionAlias my_union_alias {};

      CORBA::Any any_struct;
      CORBA::Any any_struct_alias;
      CORBA::Any any_union;
      CORBA::Any any_union_alias;

      // Write the structs and unions to anys so we can get the TypeCode info.
      any_struct <<= my_struct;
      any_struct_alias <<= my_struct_alias;
      any_union <<= my_union;
      any_union_alias <<= my_union_alias;

      // Explicitly set the TypeCode for the aliased types because the any
      // doesn't take care of aliases.
      any_struct_alias.type (_tc_MyStructAlias);
      any_union_alias.type (_tc_MyUnionAlias);

      CORBA::TypeCode_var const tc_struct = any_struct.type ();
      CORBA::TypeCode_var const tc_struct_alias = any_struct_alias.type ();
      CORBA::TypeCode_var const tc_union = any_union.type ();
      CORBA::TypeCode_var const tc_union_alias = any_union_alias.type ();

      std::cout << "Type Code of the struct: " << tc_struct->kind () << std::endl;
      std::cout << "Type Code of the struct alias: "
                << tc_struct_alias->kind () << std::endl;
      std::cout << "Type Code of the union: " << tc_union->kind () << std::endl;
      std::cout << "Type Code of the union alias: "
                << tc_union_alias->kind () << std::endl;

      // equal returns true only when the TypeCodes are exactly the same.
      if (tc_struct->equal (tc_struct_alias.in ()))
        {
          std::cout << "Type Codes are identical" << std::endl;
        }
      else
        {
          std::cout << "Type Codes are different" << std::endl;
        }

      // equivalent returns true when the TypeCode is an alias.
      if (tc_struct->equivalent (tc_struct_alias.in ()))
        {
          std::cout << "Type Codes are equivalent" << std::endl;
        }
      else
        {
          std::cout << "Type Codes are not equivalent" << std::endl;
        }

      DynAny_var const dynany_struct =
        factory->create_dyn_any_from_type_code (tc_struct.in ());

      try
        {
          DynAny_var const dynany_struct_alias =
            factory->create_dyn_any_from_type_code (tc_struct_alias.in ());
        }
      catch (CORBA::UNKNOWN const &)
        {
          std::cout << "CORBA::UNKNOWN exception when calling "
                    << "create_dyn_any_from_type_code (tc_struct_alias)" << std::endl;
        }

      try
        {
          DynAny_var const dynany_struct_alias =
            factory->create_dyn_any (any_struct_alias);
        }
      catch (CORBA::UNKNOWN const &)
        {
          std::cout << "CORBA::UNKNOWN exception when calling "
                    << "create_dyn_any (any_struct_alias)" << std::endl;
        }

      DynAny_var const dynany_union =
        factory->create_dyn_any_from_type_code (tc_union.in ());

      try
        {
          DynAny_var const dynany_union_alias =
            factory->create_dyn_any_from_type_code (tc_union_alias.in ());
        }
      catch (CORBA::UNKNOWN const &)
        {
          std::cout << "CORBA::UNKNOWN exception when calling "
                    << "create_dyn_any_from_type_code (tc_union_alias)" << std::endl;
        }

      try
        {
          DynAny_var const dynany_union_alias =
            factory->create_dyn_any (any_union_alias);
        }
      catch (CORBA::UNKNOWN const &)
        {
          std::cout << "CORBA::UNKNOWN exception when calling "
                    << "create_dyn_any (any_union_alias)" << std::endl;
        }
    }
  catch (CORBA::Exception const &)
    {
      std::cerr << "Caught CORBA exception" << std::endl;
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
