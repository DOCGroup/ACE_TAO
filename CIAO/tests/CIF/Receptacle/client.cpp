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

//============================================================
// test_already_connected_exception
//============================================================
int
test_already_connected_exception (::Components::Receptacles_ptr rec,
                                  ::CORBA::Object_ptr facet)
{
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

int
run_test (::Components::Receptacles_ptr rec,
          ::CORBA::Object_ptr facet)
{
  int ret = 0;
  try
    {
      ret += test_connect_disconnect (rec, facet);
      ret += test_invalid_name_exception (rec, facet);
      ret += test_already_connected_exception (rec, facet);

//       TODO:
//       ret += test_invalid_connection_exception ();
//       ret += test_exceeded_limit_exception ();

#if !defined (CCM_LW)
//       get_connections is not implement->don't test.
//       ReceptacleDescriptions get_all_receptacles ();
//       ReceptacleDescriptions get_named_receptacles (in NameList names)
//         raises (InvalidName);
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
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " %d error found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " No error found during tests.\n"));
    }
  return ret;
}
