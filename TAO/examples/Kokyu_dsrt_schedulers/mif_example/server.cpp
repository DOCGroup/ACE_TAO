#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "MIF_Scheduler.h"
#include "Task_Stats.h"
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT("test.ior");
  int nthreads = 2;
  bool enable_dynamic_scheduling = false;
  bool enable_yield = true;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:ds"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        options.nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        options.enable_dynamic_scheduling = true;
        break;

      case 's':
        options.enable_yield = false;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-d (enable dynamic scheduling)"
                           "-s (disable yield)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);
  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var orb_;
  // The orb
};

static Task_Stats task_stats;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  MIF_Scheduler *scheduler {};
  RTScheduling::Current_var current;
  long flags {};
  int const sched_policy = ACE_SCHED_RR;
  int const sched_scope = ACE_SCOPE_THREAD;

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  task_stats.init (100000);

  try
    {
      RTScheduling::Scheduler_var sched_owner;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (options.enable_dynamic_scheduling)
        {
          CORBA::Object_var manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager");

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

          Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type;
          if (options.enable_yield)
            {
              disp_impl_type = Kokyu::DSRT_CV_BASED;
            }
          else
            {
              disp_impl_type = Kokyu::DSRT_OS_BASED;
            }

          ACE_NEW_RETURN (scheduler,
                          MIF_Scheduler (orb.in (),
                                         disp_impl_type,
                                         sched_policy,
                                         sched_scope), -1);
          sched_owner = scheduler;

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current");

          current  =
            RTScheduling::Current::_narrow (object.in ());
        }

      Simple_Server_i server_impl (orb.in (),
                                   current.in (),
                                   task_stats,
                                   options.enable_yield);

      Simple_Server_var server =
        server_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (flags,
                           options.nthreads,
                           0,
                           ACE_Sched_Params::priority_max(sched_policy,
                                                          sched_scope)) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate threads in RT class.",
                      "Trying to activate in non-RT class\n"));

          flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
          if (worker.activate (flags, options.nthreads) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot activate server threads\n"),
                                1);
            }
        }

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "shutting down scheduler\n"));
      if (scheduler != nullptr)
        scheduler->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Exiting main...\n"));
  task_stats.dump_samples (ACE_TEXT("timeline.txt"),
                           ACE_TEXT("Time\t\tGUID"));
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
{
  ACE_Time_Value tv(120);

  try
    {
      this->orb_->run (tv);
    }
  catch (CORBA::Exception const &)
    {
    }
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): Worker thread exiting...\n"));
  return 0;
}
