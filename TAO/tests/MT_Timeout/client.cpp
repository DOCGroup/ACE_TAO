// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int iterations = 500;
int threads = 4;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:n:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Sleep_Service_var sleep_service =
        Test::Sleep_Service::_narrow(tmp.in ());

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
      sleep_service->go_to_sleep (0);

      sleep_service->shutdown ();

      orb->destroy ();

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
          //FUZZ: disable check_for_lack_ACE_OS
          ACE_DEBUG ((LM_DEBUG,
                      "Warning: Too many calls have a too big difference between "
                      "timeout and elapsed time (task0: %d, task1: %d)\n",
                      task0.too_big_difference_calls (),
                      task1.too_big_difference_calls ()));
          //FUZZ: enable check_for_lack_ACE_OS
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}







