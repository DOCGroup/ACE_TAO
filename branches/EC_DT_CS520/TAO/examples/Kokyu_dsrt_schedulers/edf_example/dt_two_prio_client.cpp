//$Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "test1C.h"
#include "EDF_Scheduler.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "Task_Stats.h"
#include "cpuload.h"

#include "dt_oneway_config.h"
#include "dt_oneway_dsui_families.h"
#include <dsui.h>
#include <cstdlib>

ACE_RCSID(MT_Server, client, "client.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

const char *ior = "file://test1.ior";
int do_shutdown = 1;
int enable_dynamic_scheduling = 1;
int enable_yield = 1;
int enable_rand = 0;
int niteration1 = 5;
int niteration2 = 4;
int workload1 = 2;
int period1 = 4;
int workload2 = 2;
int period2 = 6;

int ID_BEGIN = 0;
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
          long importance,
          CORBA::Long server_load,
          CORBA::Long period,
          CORBA::Long niteration,
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
  long importance_;
  CORBA::Long server_load_;
  CORBA::Long period_;
  CORBA::Long niteration_;
  int sleep_time_;
  unsigned int m_id;
};

class Worker_c : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker_c (CORBA::ORB_ptr orb,
          RTScheduling::Current_ptr current,
          EDF_Scheduler* scheduler,
          long importance,
          CORBA::Long server_load,
          CORBA::Long period,
          CORBA::Long niteration,
          int worker_id);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb

  RTScheduling::Current_var scheduler_current_;
  EDF_Scheduler* scheduler_;
  long importance_;
  CORBA::Long server_load_;
  CORBA::Long period_;
  CORBA::Long niteration_;
  int sleep_time_;
  unsigned int m_id;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:sN:n:W:P:w:p:r");
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

      case 'N':
        niteration1 = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        niteration2 = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'W':
        workload1 = ACE_OS::atoi (get_opts.opt_arg ());
        break;
  
      case 'P':
        period1 = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        workload2 = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        period2 = ACE_OS::atoi (get_opts.opt_arg ());
        break;
  
      case 'r':
        enable_rand = 1;
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

  ACE_DEBUG ((LM_DEBUG, "(%t|%T): main thread prio is %d\n", prio));

  CPULoad::calibrate(10);
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

      int importance=0;

      Worker worker2 (orb.in (),
                      server.in (),
                      current.in (),
                      scheduler,
                      importance,
                      workload2,
                      period2,
                      niteration2,
                      2);

      if (worker2.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate worker thread.\n"));
        }

//	usleep(5000);
      Worker_c worker1 (orb.in (),
                      current.in (),
                      scheduler,
                      importance,
                      workload1,
                      period1,
                      niteration1,
                      1);

      if (worker1.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate worker thread.\n"));
        }


      worker2.wait ();
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
              ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (10,0);
              sched_param.deadline = deadline_tv.sec ()*10000000 + deadline_tv.usec ()*10;
              sched_param.task_id = ID_BEGIN++;
              sched_param.period = 0;
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
                long importance,
                CORBA::Long server_load,
                CORBA::Long period,
                CORBA::Long niteration,
		int worker_id)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server1::_duplicate (server_ptr)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     importance_ (importance),
     server_load_ (server_load),
     period_(period),
     niteration_(niteration),
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

//  ACE_DEBUG ((LM_DEBUG, "(%t|%T) worker activated with prio %d AND iteration is %d\n", prio,niteration_));

     EDF_Scheduling::SchedulingParameter sched_param;
     CORBA::Policy_var sched_param_policy;
      CORBA::Policy_var implicit_sched_param;
  double rand2=0.0;

  if (enable_dynamic_scheduling)
    {
      sched_param.importance = importance_;
      ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (period_,0) );
      sched_param.period = period_*10000000;
      sched_param.task_id = ID_BEGIN++;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, m_id, 0, NULL);
//      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, m_id, 0, NULL);
//      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

ACE_Time_Value start_t, repair_t;
   repair_t=ACE_Time_Value(0,0);

for(int i=0;i<niteration_;i++)
{
  if(i>0 && enable_dynamic_scheduling) 
  {
/*      if(enable_rand)
        ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         sched_param.deadline +
                                         ACE_Time_Value (period_,0) -
                                         ACE_Time_Value (0,rand) );
      else

        ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         sched_param.deadline +
                                         ACE_Time_Value (period_,0) );
*/
       sched_param.deadline = sched_param.deadline+period_*10000000;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, m_id, 0, NULL);
      scheduler_current_->update_scheduling_segment(name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
     ACE_CHECK_RETURN (-1);
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, m_id, 0, NULL);
  }

  /*DTTIME:
    oneway call start on the client side.
  */
  /* MEASURE: One way call start */
//  ACE_DEBUG ((LM_DEBUG, "(%t|%T):Task %d's %d iter about to make one way call and deadline is %Q\n",sched_param.task_id, i, sched_param.deadline));
  //  TAO_debug_level = 1;
   if (i==0)
	start_t =  ACE_OS::gettimeofday ();
   else {
    repair_t = start_t+ACE_Time_Value(period_*i,0)-ACE_OS::gettimeofday ();
//    ACE_DEBUG((LM_DEBUG,"start %d,%d, repair %d, %d, period %d, i %d\n",
//		start_t.sec(), start_t.usec(), repair_t.sec(), repair_t.usec(),
//		period_, i));
    }
//  ACE_Time_Value actual = ACE_OS::gettimeofday ()+ACE_Time_Value (period_,0);
//  TimeBase::TimeT deadline = actual.sec ()*10000000 + actual.usec ()*10;

  timeval tv;

tv.tv_sec = server_load_;
tv.tv_usec = 0;

CPULoad::run(tv);

  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_START, m_id, 0, NULL);
  server_->test_method (server_load_, sched_param.task_id, i, sched_param.deadline ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  /*DTTIME:
    oneway call done on the client side.
  */
  /* MEASURE: One way call done */
  DSUI_EVENT_LOG (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, m_id, 0, NULL);
//  ACE_DEBUG ((LM_DEBUG, "(%t|%T):one way call done\n"));

  scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()),
                                      Kokyu::BLOCK);
//  rand2 = 0.03+0.1*rand()/RAND_MAX;
  rand2 = 0.1*rand()/RAND_MAX;
  if(enable_rand)
  {
    int sleep_t = period_*1000000-period_*rand2*1000000+repair_t.sec()*1000000+repair_t.usec();
    if(sleep_t > 0) 
    { 
    ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d.\n", 
		(int)(period_*1000000-period_*rand2*1000000)));
    usleep(sleep_t);
    }
    else
    {
    ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d\n", 0));
    }
  }
  else
  {
    ACE_Time_Value current = ACE_OS::gettimeofday ();
    int sleep_t = sched_param.deadline/10-current.sec()*1000000-current.usec();
    ACE_DEBUG((LM_DEBUG,"(%t|%T)NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));
    
    usleep(sleep_t);
  }
}
  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
      ACE_CHECK_RETURN (-1);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}

//--------------------------------------------------------------
Worker_c::Worker_c (CORBA::ORB_ptr orb,
                RTScheduling::Current_ptr current,
                EDF_Scheduler* scheduler,
                long importance,
                CORBA::Long server_load,
                CORBA::Long period,
                CORBA::Long niteration,
                int worker_id)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     importance_ (importance),
     server_load_ (server_load),
     period_(period),
     niteration_(niteration),
     m_id (worker_id)
     //     sleep_time_ (sleep_time)
{
}

int
Worker_c::svc (void)
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

//  ACE_DEBUG ((LM_DEBUG, "(%t|%T) worker activated with prio %d AND iteration is %d\n", prio,niteration_));

     EDF_Scheduling::SchedulingParameter sched_param;
     CORBA::Policy_var sched_param_policy;
      CORBA::Policy_var implicit_sched_param;
  double rand2=0.0;

  if (enable_dynamic_scheduling)
    {
      sched_param.importance = importance_;
      ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (period_,0) );
      sched_param.period = period_*10000000;
      sched_param.task_id = ID_BEGIN++;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, m_id, 0, NULL);
//      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, m_id, 0, NULL);
//      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

ACE_Time_Value start_t, repair_t;
   repair_t=ACE_Time_Value(0,0);

for(int i=0;i<niteration_;i++)
{
  if(i>0 && enable_dynamic_scheduling)
  {
/*      if(enable_rand)
        ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         sched_param.deadline +
                                         ACE_Time_Value (period_,0) -
                                         ACE_Time_Value (0,rand) );
      else

        ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         sched_param.deadline +
                                         ACE_Time_Value (period_,0) );
*/
       sched_param.deadline = sched_param.deadline+period_*10000000;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, m_id, 0, NULL);
      scheduler_current_->update_scheduling_segment(name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
     ACE_CHECK_RETURN (-1);
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, m_id, 0, NULL);
  }

   if (i==0)
        start_t =  ACE_OS::gettimeofday ();
   else {
    repair_t = start_t+ACE_Time_Value(period_*i,0)-ACE_OS::gettimeofday ();
    }

  timeval tv;

tv.tv_sec = server_load_;
tv.tv_usec = 0;

CPULoad::run(tv);

  scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()),
                                      Kokyu::BLOCK);
  rand2 = 0.1*rand()/RAND_MAX;
  if(enable_rand)
  {
    int sleep_t = period_*1000000-period_*rand2*1000000+repair_t.sec()*1000000+repair_t.usec();
    if(sleep_t > 0)
    {
    ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d.\n",
                (int)(period_*1000000-period_*rand2*1000000)));
    usleep(sleep_t);
    }
    else
    {
    ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d\n", 0));
    }
  }
  else
  {
    ACE_Time_Value current = ACE_OS::gettimeofday ();
    int sleep_t = sched_param.deadline/10-current.sec()*1000000-current.usec();
    ACE_DEBUG((LM_DEBUG,"(%t|%T)NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));

    usleep(sleep_t);
  }
}
  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
      ACE_CHECK_RETURN (-1);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}

