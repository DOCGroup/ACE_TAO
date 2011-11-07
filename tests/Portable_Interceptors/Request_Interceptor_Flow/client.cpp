// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"

#include "testC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc != 3)  // foo -k IOR
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
client_test (Test_ptr server)
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

      try
        {
          server->client_test (i);

          if (i == 1)
            {
              ACE_ERROR ((LM_ERROR,
              "\nERROR: No exception has been thrown from client_test() "
              "operation.\n"));
            }
        }
      catch (const Test::X&)
        {
          // Expected exception.  Ignore it.
        }
      catch (const CORBA::NO_PERMISSION&)
        {
          // Expected exception.  Ignore it.
        }
      catch (const Test::UnknownScenario& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from client_test() "
                      "operation.\n",
                      ex.scenario));
          throw;
        }
      catch (const CORBA::Exception&)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Exception thrown from client_test() "
                      "operation.\n"));
          throw;
        }
    }
}

void
server_test (Test_ptr server)
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

      try
        {
          CORBA::ULongSeq_var ulongseq;
          server->server_test (i, ulongseq.out ());

          if (i == 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "\nERROR: No exception has been thrown from server_test() "
                          "operation.\n"));
            }
        }
      catch (const Test::X&)
        {
          // Expected exception.  Ignore it.
        }
      catch (const CORBA::NO_PERMISSION&)
        {
          // Expected exception.  Ignore it.
        }
      catch (const Test::UnknownScenario& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from server_test() "
                      "operation.\n",
                      ex.scenario));
          throw;
        }
      catch (const CORBA::Exception&)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Exception thrown from server_test() "
                      "operation.\n"));
          throw;
        }
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB");

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      ::client_test (server.in ());

      ::server_test (server.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  ACE_DEBUG ((LM_INFO,
              "Request interceptor flow test passed.\n"));

  return 0;
}
