// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "testC.h"


ACE_RCSID (ORB_Shutdown,
           client,
           "$Id$")


const char *ior = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':

        if (ior == 0)
          ior = get_opts.opt_arg ();

        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k IOR\n",
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var server = test::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      server->hello (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO,
              "PortableInterceptor ORB::shutdown() test passed.\n"));

  return 0;
}
