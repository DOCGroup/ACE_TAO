// $Id$

#include "Common/CIF_Common.h"

//============================================================
// connect
//============================================================
::Components::Cookie *
connect (::Components::Receptacles_ptr rec,
         ::CORBA::Object_ptr facet)
{
  ::Components::Cookie_var ck;
  try
    {
      ck = rec->connect ("use_cif_foo", facet);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidConnection "
                            "exception during connect\n"));
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: AlreadyConnected "
                            "exception during connect\n"));
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: ExceededConnectionLimit "
                            "exception during connect\n"));
    }
  return ck._retn ();
}

//============================================================
// disconnect
//============================================================
::CORBA::Object_ptr
disconnect (::Components::Receptacles_ptr rec,
            ::Components::Cookie * ck)
{
  ::CORBA::Object_var obj;
  try
    {
      obj = rec->disconnect ("use_cif_foo", ck);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidName "
                            "exception during disconnect\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidConnection "
                            "exception during disconnect\n"));
    }
  catch (const ::Components::CookieRequired &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: CookieRequired "
                            "exception during disconnect\n"));
    }
  catch (const ::Components::NoConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: NoConnection "
                            "exception during disconnect\n"));
    }
  return obj._retn ();
}

//============================================================
// test_connect_disconnect
//============================================================
int
test_connect_disconnect (::Components::Receptacles_ptr rec,
                         ::CORBA::Object_ptr facet)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_connect_disconnect test\n"));

  ::Components::Cookie_var ck = connect (rec, facet);
  ::CORBA::Object_var obj = disconnect (rec, ck.in ());
  if (!::CORBA::is_nil (obj.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle run_test - "
                            "disconnect test passed !\n"));
      return 0;
    }
  else
    return 1;
}

//============================================================
// test_invalid_name_exception
//============================================================
int
test_invalid_name_exception (::Components::Receptacles_ptr rec,
                             ::CORBA::Object_ptr facet)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_invalid_name_exception test\n"));

  ::Components::Cookie_var ck;
  try
    {
      ck = rec->connect ("use_cif_foo_invalid_name", facet);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle test_invalid_name_exception - "
                            "Received expected InvalidName "
                            "exception during connect\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_name_exception - "
                            "Error: exception during connect\n"));
      return 1;
    }
  //connect an excisting receptacle
  ck = connect (rec, facet);
  //disconnect a wrong receptacle
  ::CORBA::Object_var obj;
  try
    {
      obj = rec->disconnect ("use_cif_foo_invalid_name", ck);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle test_invalid_name_exception - "
                            "Received expected InvalidName "
                            "exception during disconnect\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_name_exception - "
                            "Error: exception during disconnect\n"));
      return 1;
    }
  return 0;
}

int
test_invalid_connection_exception (::Components::Receptacles_ptr rec)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_invalid_connection_exception test\n"));

  ::Components::Cookie_var ck;
  try
    {
      ck = rec->connect ("use_cif_foo", ::CORBA::Object::_nil ());
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_connection_exception - "
                            "Error: InvalidName "
                            "exception during connect\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_connection_exception - "
                            "Expected InvalidConnection "
                            "exception received\n"));
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_connection_exception - "
                            "Error: AlreadyConnected "
                            "exception during connect\n"));
      return 1;
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_invalid_connection_exception - "
                            "Error: ExceededConnectionLimit "
                            "exception during connect\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_invalid_connection_exception - "
                        "Test passed!\n"));
  return 0;
}

//============================================================
// test_already_connected_exception
//============================================================
int
test_already_connected_exception (::Components::Receptacles_ptr rec,
                                  ::CORBA::Object_ptr facet)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_already_connected_exception test\n"));

  ::Components::Cookie_var ck;
  try
    {
      ck = rec->connect ("use_cif_foo", facet);
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle test_already_connected_exception - "
                            "Received expected AlreadyConnected "
                            "exception during connect\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_already_connected_exception - "
                            "Error: exception during connect\n"));
      return 1;
    }
  return 0;
}

//============================================================
// test_get_connections
//============================================================
#if !defined (CCM_LW)
int
test_get_connections (::Components::Receptacles_ptr rec)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_get_connections test\n"));
  ::Components::ConnectionDescriptions_var descriptions;
  try
    {
      descriptions = rec->get_connections ("use_cif_foo");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_connections - "
                             "Error: Received InvalidName "
                             "exception during invocation of "
                             "get_connections.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_connections");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_connections - "
                            "Error: exception during invocation of "
                            "get_connections.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_connections - "
                        "Test passed!\n"));
  // test invalid name exception during invocation of get_connections
  try
    {
      descriptions = rec->get_connections ("use_cif_foo_invalid_name");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_connections - "
                            "Received expected InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_connections - invalid name");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_connections - "
                            "Error: exception during invocation of "
                            "get_connections.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_connections - "
                        "Invalid name test passed!\n"));

  return 0;
}
#endif

//============================================================
// test_get_all_receptacles
//============================================================
#if !defined (CCM_LW)
int
test_get_all_receptacles (::Components::Receptacles_ptr rec)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_get_all_receptacles test\n"));

  ::Components::ReceptacleDescriptions_var descriptions;
  try
    {
      descriptions = rec->get_all_receptacles ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_all_receptacles");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_all_receptacles - "
                            "Error: exception during invocation of "
                            "get_all_receptacles.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_all_receptacles - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_LW)
int
test_get_named_receptacles (::Components::Receptacles_ptr rec)
{
  ACE_DEBUG ((LM_DEBUG, "Run test_get_named_receptacles test\n"));
  ::Components::ReceptacleDescriptions_var descriptions;

  ::Components::NameList one_name;
  one_name.length (1);
  one_name[0] = ::CORBA::string_dup ("use_cif_foo");

  try
    {
      descriptions = rec->get_named_receptacles (one_name);
      if (descriptions->length () != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Receptacle test_get_named_receptacles one name - "
                            "Error: Unexpected length recieived.\n"),
                            1);
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles one name - "
                             "Error: Received InvalidName "
                             "exception during invocation of "
                             "get_named_receptacles.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_receptacles one name");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles one name - "
                            "Error: exception during invocation of "
                            "get_named_receptacles.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_named_receptacles - "
                        "Test with one name passed!\n"));

  ::Components::NameList two_names;
  two_names.length (2);
  two_names[0] = ::CORBA::string_dup ("use_cif_foo");
  two_names[1] = ::CORBA::string_dup ("use_cif_inherited_foo");

  try
    {
      descriptions = rec->get_named_receptacles (two_names);
      if (descriptions->length () != 2)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Receptacle test_get_named_receptacles two names - "
                            "Error: Unexpected length recieived.\n"),
                            1);
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles two names - "
                             "Error: Received InvalidName "
                             "exception during invocation of "
                             "test_get_named_receptacles.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_receptacles two names");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles two names - "
                            "Error: exception during invocation of "
                            "test_get_named_receptacles.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_named_receptacles - "
                        "Test with two names passed!\n"));


  // test invalid name exception during invocation of get_named_receptacles
  ::Components::NameList invalid_names;
  invalid_names.length (2);
  invalid_names[0] = ::CORBA::string_dup ("use_cif_foo");
  invalid_names[1] = ::CORBA::string_dup ("use_cif_foo_invalid_name");
  try
    {
      descriptions = rec->get_named_receptacles (invalid_names);
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles - "
                            "Error: didn't receive expected InvalidName "
                            "exception.\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Receptacle test_get_named_receptacles - "
                            "Received expected InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_receptacles");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle test_get_named_receptacles - "
                            "Error: exception during invocation of "
                            "test_get_named_receptacles.\n"));
      return 1;
    }
  return 0;
}
#endif


int
run_test (::Components::Receptacles_ptr rec,
          ::CORBA::Object_ptr facet)
{
  int ret = 0;
  try
    {
      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_connect_disconnect (rec, facet);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_invalid_name_exception (rec, facet);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_already_connected_exception (rec, facet);

//       TODO:
       ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
       ret += test_invalid_connection_exception (rec);

//       ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
//       ret += test_exceeded_limit_exception ();

#if !defined (CCM_LW)
      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_connections (rec);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_all_receptacles (rec);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_receptacles (rec);
#endif
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: Unknown "
                            "exception during run_test\n"));
      ++ret;
    }
  return ret;
}


int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  CIF_Common cmd;
  int ret = 0;
  try
    {
      if (cmd.init (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Error: Unable to initalize\n"),
                            1);

        }
      ::Components::Receptacles_var rec = cmd.get_receptacle_interface ();
      ::Components::Navigation_var nav = cmd.get_navigation_interface ();

      if (::CORBA::is_nil (rec.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get receptacles interface\n"),
                            1);
        }
      if (::CORBA::is_nil (nav.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get navigation interface\n"),
                            1);
        }

      ::CORBA::Object_var facet = nav->provide_facet ("provide_cif_foo");
      if (::CORBA::is_nil (facet.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get Provide facet 'provide_cif_foo'\n"),
                            1);
        }

      ret = run_test (rec.in (), facet.in ());

      cmd.shutdown ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Navigation main");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Error: Caught unknown exception\n"),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ACE_DEBUG ((LM_DEBUG, "SUMMARY : \n"));
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "\tACE_TMAIN : "
              " %d error(s) found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "\tACE_TMAIN : "
              " No problems found during tests.\n"));
    }
  return ret;
}
