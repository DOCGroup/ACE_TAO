#include "ace/Get_Opt.h"
#include "testC.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int iterations {100};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior);

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior),
                            1);
        }

      for (int i = 0; i != options.iterations; ++i)
        {
          if (server->test_method (i) != i)
            {
              ACE_ERROR ((LM_ERROR,
                          "Unexpected result from test_method\n"));
            }

          try
            {
              server->test_raise (i);
              ACE_ERROR ((LM_ERROR,
                          "The test_raise call didn't raise\n"));
            }
          catch (Simple_Server::Failure const &fail)
            {
              ACE_UNUSED_ARG (fail);
              // Do nothing, this is the normal behavior...
            }
        }

      server->shutdown ();
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
