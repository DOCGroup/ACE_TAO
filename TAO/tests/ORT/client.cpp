// $Id$

#include "ORT_testC.h"

#include "ace/Get_Opt.h"

ACE_RCSID (ORT,
           client,
           "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-k IOR "
                           "\n",
                           argv[0]),
                          -1);
      }
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "client_sum_orb" TAO_ENV_ARG_DECL);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->string_to_object (ior TAO_ENV_ARG_DECL);
      ACE_TRY_CHECK;

      ObjectReferenceTemplate::ORT_test_var server =
        ObjectReferenceTemplate::ORT_test::_narrow (obj.in ()
                                                    TAO_ENV_ARG_DECL);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // @@ Priyanka, can't you just do the following:
      //      CORBA::Boolean result = server->request...
      //    No need initialize twice.
      CORBA::Boolean result = 1;

      result = server->request_server (TAO_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      if (result == 0)
        ACE_DEBUG ((LM_DEBUG, "Successful invocation\n"));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ORT test on client side :");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
