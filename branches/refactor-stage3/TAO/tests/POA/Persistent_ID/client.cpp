// $Id$

// ================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a client implementation.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "testC.h"

ACE_RCSID(Generic_Servant, client, "$Id$")

static const char *IOR = "file://ior";
static int shutdown_server = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = ACE_OS::strdup (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR\n"), -1);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->string_to_object (IOR
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test1 =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test1->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test2 =
        test1->create_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test2->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test1->destroy_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test3 =
        test1->create_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test2->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test3->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (shutdown_server)
        {
          test1->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error!");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
