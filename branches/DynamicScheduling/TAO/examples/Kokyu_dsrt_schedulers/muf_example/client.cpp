// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "MUF_Scheduler.h"

ACE_RCSID(MT_Server, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int do_shutdown = 0;
int enable_dynamic_scheduling = 0;

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb,
          Simple_Server_ptr server_ptr,
          RTScheduling::Current_ptr current,
          MUF_Scheduler* scheduler,
	  MUF_Scheduling::SchedulingParameter sched_param,
          CORBA::Long server_load);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb

  Simple_Server_var server_;
  RTScheduling::Current_var scheduler_current_;
  MUF_Scheduler* scheduler_;
  MUF_Scheduling::SchedulingParameter sched_param_;  
  CORBA::Long server_load_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:i:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        enable_dynamic_scheduling = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-d"
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
  MUF_Scheduler* scheduler;
  RTScheduling::Current_var current;

  ACE_hthread_t main_thr_handle;
  ACE_Thread::self (main_thr_handle);

  ACE_OS::thr_setprio (main_thr_handle, 
		       ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD));

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      if (enable_dynamic_scheduling)
        {
          ACE_DEBUG ((LM_DEBUG, "Dyn Sched enabled\n"));
          CORBA::Object_ptr manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;


          ACE_NEW_RETURN (scheduler,
                          MUF_Scheduler (orb.in ()), -1);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

        }

      MUF_Scheduling::SchedulingParameter sched_param;
      sched_param.criticality = 0;
      sched_param.deadline = 0;
      sched_param.estimated_initial_execution_time = 0;

      Worker worker1 (orb.in (), server.in (), current.in (), scheduler, sched_param, 500000 );

      long flags = THR_NEW_LWP | THR_JOINABLE ;

      if (worker1.activate (flags, 1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client thread 1\n"),
                          1);

      ACE_OS::sleep(5);
      sched_param.criticality = 1;
      sched_param.deadline = 0;
      sched_param.estimated_initial_execution_time = 0;
      Worker worker2 (orb.in (), server.in (), current.in (), scheduler, sched_param, 20000);
      if (worker2.activate (flags, 1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client thread 2\n"),
                          1);

      worker1.wait ();
      worker2.wait ();

      ACE_DEBUG ((LM_DEBUG, "(%t): wait for worker threads done in main thread\n"));

      if (do_shutdown)
        {
          if (enable_dynamic_scheduling)
            {
              MUF_Scheduling::SchedulingParameter sched_param;
	      sched_param.criticality = 0;
              CORBA::Policy_var sched_param_policy = 
		scheduler->create_scheduling_parameter (sched_param);
              CORBA::Policy_ptr implicit_sched_param = 0;
              current->begin_scheduling_segment (0,
                                                 sched_param_policy.in (),
                                                 implicit_sched_param
                                                 ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }

            ACE_DEBUG ((LM_DEBUG, "(%t): about to call server shutdown\n"));
            server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;

            ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));


            if (enable_dynamic_scheduling)
            {
              current->end_scheduling_segment (0 ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }

      scheduler->shutdown ();
      ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));
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

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb,
                Simple_Server_ptr server_ptr,
                RTScheduling::Current_ptr current,
                MUF_Scheduler* scheduler,
		MUF_Scheduling::SchedulingParameter sched_param,
                CORBA::Long server_load)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (server_ptr)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     sched_param_ (sched_param),
     server_load_ (server_load)
{
}

int
Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  const char * name = 0;

  if (enable_dynamic_scheduling)
    {
      CORBA::Policy_var sched_param_policy;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param_);
      CORBA::Policy_ptr implicit_sched_param = 0;
      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  ACE_DEBUG ((LM_DEBUG, "about to make two way call\n"));
  server_->test_method (server_load_ ACE_ENV_ARG_PARAMETER);
  ACE_DEBUG ((LM_DEBUG, "two way call done\n"));
  ACE_TRY_CHECK;

  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
      ACE_CHECK;
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}
