#include "SA_POP_Input_AdapterC.h"
#include "ace/Get_Opt.h"

// IOR file of the SA_POP_IA.
const char * ior = 0;
const char * filename = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:f:");
  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
	        break;

        case 'f':
	        filename = get_opts.opt_arg ();
	        break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Usage:  %s\n"
                             "-k <SA_POP_IA IOR> (default is file://SA_POP_IA_Component.ior)\n",
                             "-f <Deployment plan filename>\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (ior  == 0)
    {
      ior = "file://SA_POP_IA_Component.ior";
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "");

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      ::CIAO::RACE::SA_POP_IA::Test_var test =
        ::CIAO::RACE::SA_POP_IA::Test::_narrow (obj.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire SA_POP_IA's objref\n"),
                            -1);
        }

      if (filename)
      {
        test->deploy_string (filename);
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception \n");
      return -1;
    }

  return 0;
}
