// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

const char *ior_input_file = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	      ior_input_file = get_opts.optarg;
	      break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			                     "-k <iorfile>",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int 
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
                                            argv, 
                                            "" 
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj = 
        orb->string_to_object (ior_input_file
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "string_to_object failed.\n"),
                            1);
        }

      foo_var objref = foo::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (objref.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "_narrow failed.\n"),
                            1);
        }

      const char * inarg = "in";
      CORBA::String_var inoutarg = CORBA::string_dup ("old_inout");
      CORBA::String_var outarg;

      CORBA::String_var retval = objref->all_str (inarg, 
                                                  inoutarg,
                                                  outarg.out ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      cout << "retval: " << retval.in () << endl;
      cout << "inoutarg: " << inoutarg.in () << endl;
      cout << "outarg: " << outarg.in () << endl;

      for (int i = 0 ; i != 10; ++i)
        {
          objref->push ("push_event"
		        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
	}

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception caught - ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
