// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "testC.h"
#include "Client_ORBInitializer.h"

ACE_RCSID (Request_Interceptor_Flow,
           client,
           "$Id$")

const char *ior = 0;

int
parse_args (int argc, char *argv[])
{
  if (argc != 3)  // foo -k IOR
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

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

void
client_test (Test_ptr server ACE_ENV_ARG_DECL)
{
  // Currently, there are only four scenarios for the client side
  // tests.
  const Test::TestScenario MAX_CLIENT_SCENARIO = 4;

  for (Test::TestScenario i = 1; i <= MAX_CLIENT_SCENARIO; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "\nCLIENT SCENARIO %d\n"
                  "------------------\n",
                  i));

      ACE_TRY
        {
          server->client_test (i ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (Test::X, ex)
        {
          // Expected exception.  Ignore it.
        }
      ACE_CATCH (CORBA::NO_PERMISSION, ex)
        {
          // Expected exception.  Ignore it.
        }
      ACE_CATCH (Test::UnknownScenario, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from client_test() "
                      "operation.\n",
                      ex.scenario));
          ACE_RE_THROW;
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Exception thrown from client_test() "
                      "operation.\n"));
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

void
server_test (Test_ptr server ACE_ENV_ARG_DECL)
{
  // Currently, there are only four scenarios for the server side
  // tests.
  const Test::TestScenario MAX_SERVER_SCENARIO = 4;

  for (Test::TestScenario i = 1; i <= MAX_SERVER_SCENARIO; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "\nSERVER SCENARIO %d\n"
                  "------------------\n",
                  i));

      ACE_TRY
        {
          server->server_test (i ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (Test::X, ex)
        {
          // Expected exception.  Ignore it.
        }
      ACE_CATCH (CORBA::NO_PERMISSION, ex)
        {
          // Expected exception.  Ignore it.
        }
      ACE_CATCH (Test::UnknownScenario, ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from server_test() "
                      "operation.\n",
                      ex.scenario));
          ACE_RE_THROW;
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Exception thrown from server_test() "
                      "operation.\n"));
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      ::client_test (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ::server_test (server.in () ACE_ENV_ARG_PARAMETER);
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
              "Request interceptor flow test passed.\n"));

  return 0;
}
