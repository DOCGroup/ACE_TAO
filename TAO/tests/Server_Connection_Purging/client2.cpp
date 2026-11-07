#include "ace/Get_Opt.h"
#include "TestC.h"
#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <IOR>\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // Get an object reference from the argument string.
      CORBA::Object_var object = orb->string_to_object (options.ior.c_str ());

      // Try to narrow the object reference to a <test> reference.
      test_var const test_object = test::_narrow (object.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Sending a shutdown call..\n"));
      test_object->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
