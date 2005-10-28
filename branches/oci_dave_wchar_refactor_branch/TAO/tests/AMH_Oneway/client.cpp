// $Id$

#include "ace/Get_Opt.h"
#include "TestC.h"
#include "ace/Argv_Type_Converter.h"

const char *ior = "file://test.ior";
int num_calls = 10;

int
parse_args (int argc, char *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  ACE_Get_Arg_Opt<char> get_opts (convert.get_argc(), convert.get_ASCII_argv(), "n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        num_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        break;
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (convert.get_argc(), convert.get_ASCII_argv(), "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (convert.get_argc(), convert.get_ASCII_argv()) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      // Do a couple of calls on the server. If the sever is trying to
      // do something stupid like sending an exception to us, then it
      // won't be able to handle more than 1 request from us.
      Test::Timestamp time = 10;
      for (int i = 0; i < num_calls; i++)
        {
          roundtrip->test_method (time ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Sent call # %d \n", i));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
