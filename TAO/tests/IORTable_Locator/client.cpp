#include "simple_testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

struct Client_Options
{
  ACE_TString test_ior {ACE_TEXT ("corbaloc:iiop:localhost:15989/SIMPLE_TEST_KEY")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.test_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <cobraloc>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "MY_TEST_ORB");

      if (parse_args (argc, argv, options) != 0)
        return 1;

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "OK, now try to resolve a remote "
                      "IORTable::Locator entry...\n"));

          CORBA::Object_var const client_object =
            orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.test_ior.c_str ()));

          simple::SimpleTest_T_var const simple_test_client =
            simple::SimpleTest_T::_narrow (client_object.in ());

          if (CORBA::is_nil (simple_test_client.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Simpe client is nil!\n"));
              return 1;
            }

          ACE_DEBUG ((LM_DEBUG,
                      "We narrowed the object, now let's invoke it!\n"));

          simple_test_client->shutdown ();
        }
      catch (CORBA::OBJ_ADAPTER const &obj_adapter)
        {
          obj_adapter._tao_print_exception ("CAUGHT OBJ_ADAPTER!!!");
        }
      catch (CORBA::OBJECT_NOT_EXIST const &obj_not_exist)
        {
          obj_not_exist._tao_print_exception ("CAUGHT OBJECT NOT EXIST!!!");
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Leaving!\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CAUGHT EXCEPTION: ");
      return -1;
    }

  return 0;
}
