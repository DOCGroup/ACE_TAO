// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Client_Leaks, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 100;
int threads = 12;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:n:");
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

      case 'n':
        threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <iterations> "
                           "-i <threads> "
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
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Process_Factory_var process_factory =
        Test::Process_Factory::_narrow(tmp.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (process_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil process reference <%s>\n",
                             ior),
                            1);
        }

      Client_Task client_task (process_factory.in (),
                               iterations);

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                threads, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      ACE_Thread_Manager::instance ()->wait ();

      process_factory->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only pass the test if 90% of the calls worked
      if (client_task.successful_calls () < 0.9 * iterations * threads)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: no calls were successful\n"));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
