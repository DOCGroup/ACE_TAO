// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "testC.h"

ACE_RCSID(MT_Server, client, "$Id$")

const char *ior_input_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
run_test (Simple_Server_ptr server
          TAO_ENV_ARG_DECL);

int
main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var objref =
        orb->string_to_object (ior_input_file);
      ACE_TRY_CHECK;

      if (objref.in () == 0)
        {
          ACE_ERROR_RETURN  ((LM_ERROR,
                              "The received objref is NULL \n"),
                             -1);
        }

      Simple_Server_var server =
        Simple_Server::_narrow (objref.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference  is nil\n"),
                            -1);
        }

      run_test (server.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught an exception \n");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void run_test (Simple_Server_ptr server
               TAO_ENV_ARG_DECL)
{
  for (int loop = 0; loop < 10; loop++)
    {
      ACE_TRY
        {
          ACE_DEBUG ((LM_DEBUG,
                      "About to make remote call \n"));
          ACE_OS::sleep (2);

          // Make a remote call
          server->remote_call (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          /*ACE_DEBUG ((LM_DEBUG,
                      "Kill  the primary . . . "));
          ACE_OS::sleep (25);
          ACE_DEBUG ((LM_DEBUG, " hope you did\n")); */
          ACE_DEBUG ((LM_DEBUG,
                      "I am going to shutdown \n"));
          server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_OS::sleep (23);
        }
      ACE_CATCH (CORBA::UserException, x)
        {
          ACE_PRINT_EXCEPTION (x, "Caught an ");
        }
      ACE_CATCH (CORBA::TRANSIENT, t)
        {
          ACE_PRINT_EXCEPTION (t, "Caught Exception");
        }
      ACE_CATCH (CORBA::COMM_FAILURE, f)
        {
          ACE_PRINT_EXCEPTION (f, "A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      "Automagically re-issuing request on COMM_FAILURE\n"));
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Unexpected exception");
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}
