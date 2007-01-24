// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

ACE_RCSID(Bug_1361_Regression, shutdown, "$Id$")

#if defined (ACE_OPENVMS)
// need this to circumvent link error on OpenVMS
// has to do with interference in template instantiations
// because multiple executables are build in one directory
// reuising eachothers object files.
ACE_Time_Value dum = ACE_Time_Value::zero;
#endif

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[]);

int
main (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
             "Starting shutdown\n"));
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior
                              ACE_ENV_ARG_PARAMETER);

      Test::Echo_Caller_var server =
        Test::Echo_Caller::_narrow(tmp.in ()
                              ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo_Caller reference <%s>\n",
                             ior),
                            1);
        }

      server->shutdown ();

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
