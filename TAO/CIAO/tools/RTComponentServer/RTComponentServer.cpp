// $Id$

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ComponentServer_Task.h"
#include "Server_init.h"
#include "tao/RTCORBA/RTCORBA.h"

/// The following check is taken from $(TAO_ROOT)/tests/RTCORBA/

int
parse_args (int argc,
            char *argv[],
            CIAO::ComponentServer_Task::Options &opts)
{
  ACE_Get_Opt get_opts (argc, argv, "nk:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        opts.use_callback_ = 0;
        break;

      case 'o':  // get the file name to write to
       opts.ior_output_filename_ = get_opts.opt_arg ();
      break;

      case 'k':  // get the activator callback IOR
       opts.callback_ior_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-n Don't not try to callback ServerActivator (testing)\n"
                           "-o <ior_output_file>\n"
                           "-k <activator_callback_ior>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (opts.use_callback_ && opts.callback_ior_.length () == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Callback IOR to ServerActivator is required.\n"),
                      -1);

  return 0;
}

const char *
sched_policy_name (int sched_policy)
{
  const char *name = 0;

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}

void
check_supported_priorities (CORBA::ORB_ptr orb)
{
  int sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run,
  // i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels with the %s scheduling policy\n"
                  "on this platform to run, terminating program....\n"
                  "Check svc.conf options\n",
                  sched_policy_name (sched_policy)));

      ACE_OS::exit (2);
    }
}


int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register value factories for the server side.
      CIAO::Server_init (orb.in ());

      CIAO::ComponentServer_Task::Options options;

      int result =
        parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in());

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      CIAO::ComponentServer_Task cs_task (thread_manager,
                                          orb.in (),
                                          options);

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        cs_task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
