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
	ior = get_opts.optarg;
	break;

      case 'i':
	iterations = ACE_OS::atoi (get_opts.optarg);
	break;

      case 'n':
	threads = ACE_OS::atoi (get_opts.optarg);
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
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Sleep_Service_var sleep_service =
        Test::Sleep_Service::_narrow(tmp.in (), ACE_TRY_ENV);
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

      sleep_service->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Task 0: Successful calls = %d, timed out calls = %d\n",
                  task0.successful_calls (),
                  task0.timed_out_calls ()));

      ACE_DEBUG ((LM_DEBUG,
                  "Task 1: Successful calls = %d, timed out calls = %d\n",
                  task1.successful_calls (),
                  task1.timed_out_calls ()));
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
