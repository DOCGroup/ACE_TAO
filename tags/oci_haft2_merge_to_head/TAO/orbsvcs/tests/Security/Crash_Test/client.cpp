// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

ACE_RCSID(Send_File, client, "$Id$")

const char *ior = "file://test.ior";
CORBA::Boolean call_shutdown = 0;
int wait_time = 10;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "sk:w:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 's':
        call_shutdown = 1;
        break;
      case 'w':
        wait_time = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "CLIENT (%P): usage:  %s "
                           "-k <ior> [-s] -w <wait-time>"
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
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CLIENT (%P): Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      for (CORBA::ULong i = 0; i < 2; i++)
        {
          ACE_TRY_EX(nested_try)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CLIENT (%P): Connecting to the server...\n"));

              // If we are retrying then make just one request.
              do
                {
                  server->send_line ("some data" ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (nested_try);

                }
              while (i == 0);
            }
          ACE_CATCH (CORBA::COMM_FAILURE, ex)
            {
              // If this happens second time then we are done.
              if (i != 0) ACE_RE_THROW;

              // Waiting for server to come back
              ACE_DEBUG ((LM_DEBUG,
                          "CLIENT (%P): Caught CORBA::COMM_FAILURE. "
                          "Assuming server crashed and will come up soon.\n"
                          "CLIENT (%P): Waiting for %d seconds...\n",
                          wait_time));
              ACE_OS::sleep (wait_time);
            }
          ACE_ENDTRY;
          ACE_TRY_CHECK;
        }

      if (call_shutdown)
        {

          // Let other clients to finish their task if any
          ACE_OS::sleep (3);

          ACE_DEBUG ((LM_DEBUG,
                      "CLIENT (%P): Calling shutdown...\n"));

          server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }


      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CLIENT (%P): Done.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CLIENT (%P): Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
