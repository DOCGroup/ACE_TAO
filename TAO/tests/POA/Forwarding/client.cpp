
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a simple test client implementation.  Also looks out
 *   for forwarding exceptions
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "testC.h"

struct Client_Options
{
  ACE_TCHAR const *ior {};
  int servers { 2 };
  int iterations { 3 };
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:i:k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 's':
        options.servers = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR\n"
                           "-i iterations\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
do_calls (test_ptr test, int servers, int iterations)
{
  for (int server = 1; server <= servers; ++server)
    {
      for (int iteration = 1; iteration <= iterations; ++iteration)
        {
          // Invoke the doit() method of the test reference.
          CORBA::Long const result = test->doit ();

          // Print the result of doit () method of the test reference.
          ACE_DEBUG ((LM_DEBUG,
                      "doit() returned %d\n",
                      result));
        }

      // Don't forward the last server
      if (server != servers)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Asking server to forward next time\n"));
          test->forward ();
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Client_Options options;
      // Initialize options based on command-line arguments.
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      // Try to narrow the object reference to a test reference.
      test_var test =
        test::_narrow (object.in ());

      do_calls (test.in (), options.servers, options.iterations);

      test->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client");
      return -1;
    }

  return 0;
}
