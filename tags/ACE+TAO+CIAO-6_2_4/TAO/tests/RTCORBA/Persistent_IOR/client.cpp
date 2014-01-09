// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"

static int iterations = 5;
static int shutdown_server = 0;

static const ACE_TCHAR *ior = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
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

  if (ior == 0)
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

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      test_var test =
        test::_narrow (object.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil reference <%s>\n",
                             ior),
                            1);
        }

      for (int i = 0; i < iterations; i++)
        {
          test->method ();
        }

      if (shutdown_server)
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
