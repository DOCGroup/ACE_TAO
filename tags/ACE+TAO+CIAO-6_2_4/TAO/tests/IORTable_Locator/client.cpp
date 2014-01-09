// $Id$

#include "simple_testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *testIOR = ACE_TEXT ("corbaloc:iiop:localhost:15989/SIMPLE_TEST_KEY");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        testIOR = get_opts.opt_arg ();
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "MY_TEST_ORB");

      if (parse_args (argc, argv) != 0)
        return 1;

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "OK, now try to resolve a remote "
                      "IORTable::Locator entry...\n"));

          CORBA::Object_var clientObj = orb->string_to_object (testIOR);

          simple::SimpleTest_T_var simpleTestClient =
            simple::SimpleTest_T::_narrow (clientObj.in ());

          if (CORBA::is_nil (simpleTestClient.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Simpe client is nil!\n"));
              return 1;
            }

          ACE_DEBUG ((LM_DEBUG,
                      "We narrowed the object, now let's invoke it!\n"));

          simpleTestClient->shutdown ();
        }
      catch(const CORBA::OBJ_ADAPTER &objAdapter)
        {
          objAdapter._tao_print_exception ("CAUGHT OBJ_ADAPTER!!!");
        }
      catch(const CORBA::OBJECT_NOT_EXIST &objNE)
        {
          objNE._tao_print_exception ("CAUGHT OBJECT NOT EXIST!!!");
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Leaving!\n"));
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("CAUGHT EXCEPTION: ");
      return -1;
    }

  return 0;
}
