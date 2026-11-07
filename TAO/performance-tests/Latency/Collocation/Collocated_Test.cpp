#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"
#include "ace/Manual_Event.h"

#include "tao/Strategies/advanced_resource.h"

struct Test_Options
{
  int iterations = 250000;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        // This is a hack but that is okay!
        return 0;
      }
  // Indicates successful parsing of the command line
  return 0;
}

void
set_priority ()
{
  int const base_priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  int const priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                        base_priority);
  // Enable FIFO scheduling
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  //Use Real-time Scheduling class if possible
  set_priority ();

  try
    {
      CORBA::ORB_var sorb =
        CORBA::ORB_init (argc, argv);

      Test_Options options;
      if (parse_args (argc, argv, options) == -1)
        return -1;

      ACE_Manual_Event wait_for_event;

      Server_Task server_task (sorb.in (),
                               wait_for_event,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      // Wait for the server thread to do some processing
      wait_for_event.wait ();

      // Obtain the object reference
      Test::Roundtrip_var reference = server_task.get_reference ();

      Client_Task client_task (reference.in (),
                               options.iterations,
                               ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }
  return 0;
}
