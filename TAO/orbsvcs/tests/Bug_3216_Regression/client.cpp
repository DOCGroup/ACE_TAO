// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"

struct Client_Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result {};
  try
    {
      Client_Options options;
      int one { 1 };
      // ORB_init accepts mutable argv strings and may adjust them.
      char CLIENT[] = "client";
      char *argv1[] = { CLIENT, nullptr };

      CORBA::ORB_var const orb1 =
        CORBA::ORB_init (one, argv1, "orb1");

      int three { 3 };
      char OPTION[] = "-ORBFTSendFullGroupTC";
      char VALUE[] = "1";
      char *argv2[] = { CLIENT, OPTION, VALUE, nullptr };
      CORBA::ORB_var orb2;

      try
        {
          orb2 =
            CORBA::ORB_init (three, argv2, "orb2");
        }
      catch (CORBA::BAD_PARAM const &)
        {
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - ORB doesn't even accept the config option\n"));
          return 1;
        }

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb1->string_to_object (options.ior.c_str ());

      Test::Hello_var hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil Test::Hello "
                                       "reference <%s>\n"),
                             options.ior.c_str ()),
                            1);
        }

      try
        {
          hello->check_normal_context ();
        }
      catch (Test::MyException const &)
        {
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - normal format service context not transmitted\n"));
          result = 1;
        }

      tmp =
        orb2->string_to_object (options.ior.c_str ());

      hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil Test::Hello "
                                       "reference <%s>\n"),
                             options.ior.c_str ()),
                            1);
        }

      try
        {
          hello->check_extended_context ();
        }
      catch (Test::MyException const &)
        {
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - extended format service context not transmitted\n"));
          result = 1;
        }

      hello->shutdown ();

      orb1->destroy ();
      orb2->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
                           "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified!!!\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Test passed !!!\n"));
    }
  return result;
}
