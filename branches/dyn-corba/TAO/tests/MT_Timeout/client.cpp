// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Client_Leaks, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 500;
int threads = 4;

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
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Sleep_Service_var sleep_service =
        Test::Sleep_Service::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (sleep_service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil process reference <%s>\n",
                             ior),
                            1);
        }

      Client_Task task0 (orb.in (),
                         sleep_service.in (),
                         iterations,
                         10 * 1000,
                         20 * 1000);
      Client_Task task1 (orb.in (),
                         sleep_service.in (),
                         iterations,
                         50 * 1000,
                         20 * 1000);

      if (task0.activate (THR_NEW_LWP | THR_JOINABLE, threads, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      if (task1.activate (THR_NEW_LWP | THR_JOINABLE, threads, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      ACE_Thread_Manager::instance ()->wait ();

      // Get back in sync with the server..
      sleep_service->go_to_sleep (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      sleep_service->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Task 0: Successful calls = %d, timed out calls = %d\n",
                  task0.successful_calls (),
                  task0.timed_out_calls ()));

      ACE_DEBUG ((LM_DEBUG,
                  "Task 1: Successful calls = %d, timed out calls = %d\n",
                  task1.successful_calls (),
                  task1.timed_out_calls ()));

      // No more than 5 % of the calls are allowed to have a too big
      // difference
      if (task0.too_big_difference_calls () > iterations/20
          || task1.too_big_difference_calls () > iterations/20)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR: Too many calls have a too big difference between "
                      "timeout and elapsed time (task0: %d, task1: %d)\n",
                      task0.too_big_difference_calls (),
                      task1.too_big_difference_calls ()));
        }
      else  if (task0.too_big_difference_calls () != 0
                || task1.too_big_difference_calls () != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Warning: some calls (task0: %d, task1: %d) "
                      "exceeded their expected elapsed times\n",
                      task0.too_big_difference_calls (),
                      task1.too_big_difference_calls ()));
        }

      if (task0.timed_out_calls () == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: non-zero number of timeouts "
                      "expected for task 0\n"));
        }

      if (task1.successful_calls () != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: zero successful calls "
                      "expected for task 1\n"));
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







