// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID(DSI_Gateway, client, "$Id$")

const char *ior = "file://gateway.ior";
int niterations = 5;
int do_shutdown = 0;
int test_user_exception = 0;
int test_system_exception = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xusk:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'u':
        test_user_exception = 1;
        break;

      case 's':
        test_system_exception = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-x "
                           "-u "
                           "-s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      Structure the_in_structure;
      the_in_structure.seq.length (10);

      if (test_user_exception == 1)
        {
          server->raise_user_exception (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (test_system_exception == 1)
        {
          server->raise_system_exception (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          for (int i = 0; i != niterations; ++i)
            {
              the_in_structure.i = i;
              CORBA::String_var name = CORBA::string_dup ("the name");

              Structure_var the_out_structure;

              CORBA::Long r =
                server->test_method (i,
                                     the_in_structure,
                                     the_out_structure.out (),
                                     name.inout ()
                                     TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "DSI_Simpler_Server ====\n"
                              "    x = %d\n"
                              "    i = %d\n"
                              "    length = %d\n"
                              "    name = <%s>\n",
                              r,
                              the_out_structure->i,
                              the_out_structure->seq.length (),
                              name.in ()));
                }

              if (r != i)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "(%P|%t) unexpected result = %d for %d",
                              r, i));
                }
            }
        }

      if (do_shutdown)
        {
          server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (test_exception, ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception caught - ");

      ACE_DEBUG ((LM_DEBUG,
                  "error code: %d\n"
                  "error info: %s\n"
                  "status: %s\n",
                  ex.error_code,
                  ex.error_message.in (),
                  ex.status_message.in ()));

      return 0;
    }
  ACE_CATCH (CORBA::NO_PERMISSION, ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception caught - ");

      return 0;
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
