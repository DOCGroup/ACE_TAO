// $Id$

#include "testC.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::TypeCode_var iface_tc =
        orb->create_interface_tc (CORBA::string_dup ("IDL:iface:1.0"),
                                  CORBA::string_dup ("iface"));

      CORBA::ULong length = 5;

      CORBA::UnionMemberSeq foo_members (length);
      foo_members.length (length);

      // The first three members are multiple case labels for a
      // single string member. The label values are not in order
      // or consecutive.

      foo_members[0].name = CORBA::string_dup ("foo_str_member");

      // The type member of the UnionMember takes ownership.
      foo_members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);

      // Not needed for ORB-created typecodes
      // (see CORBA 2.4.2 section 10.7.3).
      foo_members[0].type_def = CORBA::IDLType::_nil ();
      CORBA::Short label_value = 3;
      foo_members[0].label <<= label_value;

      foo_members[1].name = CORBA::string_dup ("foo_str_member");
      foo_members[1].type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
      foo_members[1].type_def = CORBA::IDLType::_nil ();
      label_value = 4;
      foo_members[1].label <<= label_value;

      foo_members[2].name = CORBA::string_dup ("foo_str_member");
      foo_members[2].type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
      foo_members[2].type_def = CORBA::IDLType::_nil ();
      label_value = 1;
      foo_members[2].label <<= label_value;

      // The default member.
      foo_members[3].name = CORBA::string_dup ("foo_iface_member");
      foo_members[3].type = CORBA::TypeCode::_duplicate (iface_tc.in ());
      foo_members[3].type_def = CORBA::IDLType::_nil ();
      CORBA::Octet default_label_value = 0;
      foo_members[3].label <<= CORBA::Any::from_octet (default_label_value);

      // The last member is the same type as the previous one, but has
      // a different name.
      foo_members[4].name = CORBA::string_dup ("foo_iface_member2");
      foo_members[4].type = CORBA::TypeCode::_duplicate (iface_tc.in ());
      foo_members[4].type_def = CORBA::IDLType::_nil ();
      label_value = 0;
      foo_members[4].label <<= label_value;

      CORBA::TypeCode_ptr foo_tc = orb->create_union_tc ("IDL:foo:1.0",
                                                          "foo",
                                                          CORBA::_tc_short,
                                                          foo_members);

      length = 2;

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
                                                          bar_members);

      // Check for equality with the typecode created by the IDL compiler.
      CORBA::Boolean eq = bar_tc->equal (_tc_bar);

      if (!eq)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ORB-created typecode not correct!\n"),
                             -1);

        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("exception:");
      return 1;
    }

  return 0;
}

