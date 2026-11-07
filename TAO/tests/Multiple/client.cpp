#include "Collocation_Tester.h"
#include "ace/Get_Opt.h"
#include <string>

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior.assign (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv[0]),
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
      // ORB Initialization
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "TAO");

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior.c_str ());

      if (CORBA::is_nil (object.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Invalid IOR file\n"));
          return 1;
        }

      Collocation_Tester tester (object.in ());
      tester.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA Exception Raised");
      return 1;
    }

  return 0;
}
