// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"

static const ACE_TCHAR *ior = ACE_TEXT("file://ior");
static int shutdown_server = 1;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'x':
        shutdown_server = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage %s:\n"
                           "\t-k <ior> [defaults to %s]\n"
                           "\t-x <shutdown server> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           ior,
                           shutdown_server),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int const result = parse_args (argc, argv);

      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      test_factory_var test_factory =
        test_factory::_narrow (object.in ());

      test_var test =
        test_factory->create_test ();

      test->normal_method ();

      int unknown_exception_raised = 0;

      try
        {
          test->unknown_exception_in_method ();
        }
      catch (CORBA::UNKNOWN)
        {
          unknown_exception_raised = 1;

          ACE_DEBUG ((LM_DEBUG,
                      "\nCORBA::UNKNOWN was thrown by the server during test::unknown_exception_in_method()\n"
                      "\tThis is expected behavior\n\n"));
        }

      if  (unknown_exception_raised != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Failure: unknown excep not raised\n"),
                            -1);
        }

      unknown_exception_raised = 0;

      test->unknown_exception_during_deactivation ();

      if (shutdown_server)
        test_factory->shutdown ();

      orb->destroy ();
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failure: Unexpected exception caught\n"),
                        -1);
    }

  return 0;
}
