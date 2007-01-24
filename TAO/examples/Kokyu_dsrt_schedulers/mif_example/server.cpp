// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "MIF_Scheduler.h"
#include "Task_Stats.h"

ACE_RCSID(MT_Server, server, "$Id$")

const char *ior_output_file = "test.ior";

int nthreads = 2;
int enable_dynamic_scheduling = 0;
const CORBA::Short max_importance = 100;
int enable_yield = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:ds");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        enable_dynamic_scheduling = 1;
        break;

      case 's':
        enable_yield = 0;
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
  // Indicates sucessful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

Task_Stats task_stats;

int
main (int argc, char *argv[])
{
  MIF_Scheduler* scheduler=0;
  RTScheduling::Current_var current;
  long flags;
  int sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  task_stats.init (100000);

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      if (enable_dynamic_scheduling)
        {
          CORBA::Object_ptr manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager"
                                             ACE_ENV_ARG_PARAMETER);

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);

          Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type;
          if (enable_yield)
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

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                              ACE_ENV_ARG_PARAMETER);

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
        }

      Simple_Server_i server_impl (orb.in (),
                                   current.in (),
                                   task_stats,
                                   enable_yield);

      Simple_Server_var server =
        server_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (flags,
                           nthreads,
                           0,
                           ACE_Sched_Params::priority_max(sched_policy,
                                                          sched_scope)) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate threads in RT class.",
                      "Trying to activate in non-RT class\n"));

          flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
          if (worker.activate (flags, nthreads) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot activate server threads\n"),
                                1);
            }
        }

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "shutting down scheduler\n"));
      scheduler->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Exiting main...\n"));
  task_stats.dump_samples ("timeline.txt",
                            "Time\t\tGUID",
                            ACE_High_Res_Timer::global_scale_factor ());
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_Time_Value tv(120);

  ACE_TRY
    {
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): Worker thread exiting...\n"));
  return 0;
}
