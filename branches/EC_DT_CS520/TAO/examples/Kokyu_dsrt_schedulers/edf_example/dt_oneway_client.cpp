//$Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "test1C.h"
#include "EDF_Scheduler.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "Task_Stats.h"

#include "dt_oneway_config.h"
#include "dt_oneway_dsui_families.h"
#include <dsui.h>

ACE_RCSID(MT_Server, client, "client.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

const char *ior = "file://test1.ior";
int do_shutdown = 1;
int enable_dynamic_scheduling = 1;
int enable_yield = 1;
int niteration = 1000;
int workload = 1;
int period = 2;
 
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
          Simple_Server1_ptr server_ptr,
          RTScheduling::Current_ptr current,
          EDF_Scheduler* scheduler,
          TimeBase::TimeT deadline,
          long importance,
          CORBA::Long server_load,
	  int worker_id);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb

  Simple_Server1_var server_;
  RTScheduling::Current_var scheduler_current_;
  EDF_Scheduler* scheduler_;
  TimeBase::TimeT deadline_;
  long importance_;
  CORBA::Long server_load_;
  int sleep_time_;
  unsigned int m_id;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:sn:w:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 's':
        enable_yield = 0;
        break;

      case 'n':
        niteration = ACE_OS::atoi (get_opts.opt_arg ());
        break;
    
      case 'w':
        workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;
  
      case 'p':
        period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-s (disable yield)"
			   "-n <niterations>"
                           "-w <workload>"
                           "-p <period>"
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
  ds_control ds_cntrl ("DT_Oneway", "dt_oneway_enable.dsui");
  ACE_High_Res_Timer non_dsui_timer;
  non_dsui_timer.calibrate ();
  non_dsui_timer.start();

  /* MEASURE: Program start time */
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, START,1,0,NULL);

  EDF_Scheduler* scheduler=0;
  RTScheduling::Current_var current;
  int prio;
  int max_prio;
  ACE_Sched_Params::Policy  sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;
  long flags;

  if (sched_policy == ACE_SCHED_RR) {
    /* MEASURE (DP): Schedule policy */
    char* policy = "ACE_SCHED_RR";
    DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULE_SETUP, 1, strlen (policy), policy);
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  }
  else {
    /* MEASURE (DP): Schedule policy */
    char* policy = "ACE_SCHED_FIFO";
    DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULE_SETUP, 1, strlen (policy), policy);
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;
  }

  ACE_hthread_t main_thr_handle;
  ACE_Thread::self (main_thr_handle);

  max_prio = ACE_Sched_Params::priority_max (sched_policy,
                                             sched_scope);

  ACE_Sched_Params sched_params (sched_policy, max_prio);

  ACE_OS::sched_params (sched_params);

  if (ACE_Thread::getprio (main_thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")
                     ));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t): main thread prio is %d\n", prio));

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

      Simple_Server1_var server =
        Simple_Server1::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
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
          ACE_TRY_CHECK;

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

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
                          EDF_Scheduler (orb.in (),
                                         disp_impl_type,
                                         sched_policy,
                                         sched_scope), -1);

	  /* MEASURE: Scheduler start time */
	  DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULER_STARTED, 1, 0, NULL);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }

      TimeBase::TimeT deadline;
      int importance=0;

      ORBSVCS_Time::Time_Value_to_TimeT (deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (50,0) );

      Worker worker1 (orb.in (),
                      server.in (),
                      current.in (),
                      scheduler,
                      deadline,
                      importance,
                      workload,
		      1);

      if (worker1.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate worker thread.\n"));
        }

      /* MEASURE: Worker thread activated */
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);

      worker1.wait ();

      /* MEASURE: Wait for worker thread done in main thread */
      // char* msg = "(%t): wait for worker threads done in main thread\n";
      // Get thread id
      // DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_WAIT_DONE, 1, strlen(msg), msg);

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): wait for worker threads done in main thread\n"));

      if (do_shutdown)
        {
          if (enable_dynamic_scheduling)
            {
              EDF_Scheduling::SchedulingParameter sched_param;
              sched_param.importance = 0;
              sched_param.deadline = 0;
              CORBA::Policy_var sched_param_policy =
                scheduler->create_scheduling_parameter (sched_param);
              CORBA::Policy_ptr implicit_sched_param = 0;
              current->begin_scheduling_segment (0,
                                                 sched_param_policy.in (),
                                                 implicit_sched_param
                                                 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

            ACE_DEBUG ((LM_DEBUG, "(%t): about to call server shutdown\n"));

	    /* MEASURE: Call to shutdown server */
	    // char* msg = "(%t): wait for worker threads done in main thread\n";
	    // Get thread id
	    DSUI_EVENT_LOG (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 1, 0, NULL);

            server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

	    /* MEASURE: After call to server shutdown */
	    DSUI_EVENT_LOG (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 1, 0, NULL);
            ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));


            if (enable_dynamic_scheduling)
            {
              current->end_scheduling_segment (0 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }

      scheduler->shutdown ();

      /* MEASURE: Scheduler stop time */
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 1, 0, NULL);
      ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  /* MEASURE: Program stop time */
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, STOP, 1, 0, NULL);

  non_dsui_timer.stop();
  ACE_hrtime_t dsui_ovhd_time;
  non_dsui_timer.elapsed_time (dsui_ovhd_time);

  ACE_OS::printf ( ACE_TEXT ("Elapsed time: %lu (nsec)\n"), dsui_ovhd_time);
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb,
                Simple_Server1_ptr server_ptr,
                RTScheduling::Current_ptr current,
                EDF_Scheduler* scheduler,
                TimeBase::TimeT deadline,
                long importance,
                CORBA::Long server_load,
		int worker_id)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server1::_duplicate (server_ptr)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     deadline_ (deadline),
     importance_ (importance),
     server_load_ (server_load),
     m_id (worker_id)
     //     sleep_time_ (sleep_time)
{
}

int
Worker::svc (void)
{
  /* MEASURE: Worker start time */
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);

  ACE_DECLARE_NEW_CORBA_ENV;
  const char * name = 0;
  /*
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):about to sleep for %d sec\n", sleep_time_));
  ACE_OS::sleep (sleep_time_);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):woke up from sleep for %d sec\n", sleep_time_));
  */
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")
                     ));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t|%T) worker activated with prio %d\n", prio));

  if (enable_dynamic_scheduling)
    {
      EDF_Scheduling::SchedulingParameter sched_param;
      CORBA::Policy_var sched_param_policy;
      sched_param.importance = importance_;
      sched_param.deadline = deadline_;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      CORBA::Policy_var implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 1, 0, NULL);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 1, 0, NULL);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

  for(int i=0; i<niteration; i++) 
  {
  /* MEASURE: One way call start */
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 1, 0, NULL);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):about to make one way call\n"));
  //  TAO_debug_level = 1;
  server_->test_method (server_load_ ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  /* MEASURE: One way call done */
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):one way call done\n"));
  
  ACE_OS::sleep(period);
  }
  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
      ACE_CHECK_RETURN (-1);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}
