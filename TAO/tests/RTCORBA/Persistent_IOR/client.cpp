#include "ace/Get_Opt.h"
#include "testC.h"

struct Client_Options
{
  int iterations {5};
  bool shutdown_server {};
  ACE_TCHAR const *ior {};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:x"));
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

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k ior "
                           "-i iterations "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error: No IOR specified\n"),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      test_var test =
        test::_narrow (object.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil reference <%s>\n",
                             options.ior),
                            1);
        }

      for (int i {}; i < options.iterations; ++i)
        {
          test->method ();
        }

      if (options.shutdown_server)
        {
          test->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception!");
      return -1;
    }

  return 0;
}
