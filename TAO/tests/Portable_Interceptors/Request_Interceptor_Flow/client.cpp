// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/SString.h"

#include "testC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

struct Options
{
  ACE_TString ior {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  if (argc != 3)  // foo -k IOR
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        if (options.ior.is_empty ())
          options.ior = get_opts.opt_arg ();
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
  Test::TestScenario constexpr MAX_CLIENT_SCENARIO { 4 };

  for (Test::TestScenario i { 1 }; i <= MAX_CLIENT_SCENARIO; ++i)
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
      catch (Test::X const &)
        {
          // Expected exception.  Ignore it.
        }
      catch (CORBA::NO_PERMISSION const &)
        {
          // Expected exception.  Ignore it.
        }
      catch (Test::UnknownScenario const &ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from client_test() "
                      "operation.\n",
                      ex.scenario));
          throw;
        }
      catch (CORBA::Exception const &)
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
  Test::TestScenario constexpr MAX_SERVER_SCENARIO { 4 };

  for (Test::TestScenario i { 1 }; i <= MAX_SERVER_SCENARIO; ++i)
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
      catch (Test::X const &)
        {
          // Expected exception.  Ignore it.
        }
      catch (CORBA::NO_PERMISSION const &)
        {
          // Expected exception.  Ignore it.
        }
      catch (Test::UnknownScenario const &ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Unknown scenario <%d> condition "
                      "returned from server_test() "
                      "operation.\n",
                      ex.scenario));
          throw;
        }
      catch (CORBA::Exception const &)
        {
          ACE_ERROR ((LM_ERROR,
                      "\nERROR: Exception thrown from server_test() "
                      "operation.\n"));
          throw;
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      // The ORB initializer var assumes ownership of the ACE_NEW allocation.
      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var const orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "Client ORB");

      if (::parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      Test_var const server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      ::client_test (server.in ());

      ::server_test (server.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  ACE_DEBUG ((LM_INFO,
              "Request interceptor flow test passed.\n"));

  return 0;
}
