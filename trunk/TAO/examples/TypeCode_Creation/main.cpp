// $Id$

#include "testC.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"

ACE_RCSID(TypeCode_Creation, main, "$Id$")

int main(int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
                                            argv, 
                                            "", 
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::TypeCode_ptr iface_tc = 
        orb->create_interface_tc (CORBA::string_dup ("IDL:iface:1.0"),
                                  CORBA::string_dup ("iface"),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong length = 2;

      CORBA::StructMemberSeq foo_members (length);
      foo_members.length (length);

      foo_members[0].name = CORBA::string_dup ("foo_str_member");
      foo_members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);

      // Not needed for ORB-created typecodes 
      // (see CORBA 2.4.2 section 10.7.3).
      foo_members[0].type_def = CORBA::IDLType::_nil ();

      foo_members[1].name = CORBA::string_dup ("foo_iface_member");

      // StructMember.type takes ownership.
      foo_members[1].type = iface_tc;

      foo_members[1].type_def = CORBA::IDLType::_nil ();

      CORBA::TypeCode_ptr foo_tc = orb->create_struct_tc ("IDL:foo:1.0",
                                                          "foo",
                                                          foo_members,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::StructMemberSeq bar_members (length);
      bar_members.length (length);

      bar_members[0].name = CORBA::string_dup ("bar_long_member");
      bar_members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);

      bar_members[0].type_def = CORBA::IDLType::_nil ();

      bar_members[1].name = CORBA::string_dup ("bar_foo_member");

      bar_members[1].type = foo_tc;

      bar_members[1].type_def = CORBA::IDLType::_nil ();

      CORBA::TypeCode_var bar_tc = orb->create_struct_tc ("IDL:bar:1.0",
                                                          "bar",
                                                          bar_members,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Check for equality with the typecode created by the IDL compiler.
      CORBA::Boolean eq = bar_tc->equal (_tc_bar,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!eq)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ORB-created typecode not correct!\n"),
                             -1);
                      
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

