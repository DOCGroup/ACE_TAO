//$Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include <unistd.h>

#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "EDF_Scheduler.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "Task_Stats.h"
#include "cpuload.h"

#if defined (ACE_HAS_DSUI)
#include "ft_config.h"
#include "ft_dsui_families.h"
#include <dsui.h>
#include <cstdlib>
#endif 

ACE_RCSID(MT_Server, client, "client.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

const char *ior = "file://test.ior";
const char *ior2 = "file://test1.ior";
int do_shutdown = 1;
int enable_dynamic_scheduling = 1;
int enable_yield = 1;
int enable_rand = 0;
int niteration1 = 5;
int niteration2 = 1500;
int workload1 = 100;
int period1 = 200;
int workload2 = 50;
int period2 = 200;
int left_work = 50;
int count = 0;

int ID_BEGIN = 1;

/*
  class Time_Handler : public ACE_Event_Handler
  {
  public:
  //Method which is called back by the Reactor when timeout occurs.
  virtual int handle_timeout (const ACE_Time_Value &tv,
  const void *arg){
  ACE_DEBUG ((LM_DEBUG, "Timer #%d timed out at %d!\n",
  count++, tv.sec()));

  //Keep yourself registered with the Reactor.
  return 0;
  }
  };
*/

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
          Simple_Server_ptr server2_ptr,
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

  Simple_Server_var server_;
  Simple_Server_var server2_;
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
  ds_control ds_cntrl ("DT_FT_Client", "ft_enable.dsui");
  ACE_High_Res_Timer non_dsui_timer;
  non_dsui_timer.calibrate ();
  non_dsui_timer.start();

  ACE_LOG_MSG->priority_mask (LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY, ACE_Log_Msg::PROCESS);
  EDF_Scheduler* scheduler=0;
  RTScheduling::Current_var current;
  int prio;
  int max_prio;
  ACE_Sched_Params::Policy  sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;
  long flags;

  if (sched_policy == ACE_SCHED_RR) {
    /* MEASURE (DP): Schedule policy */
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  }
  else {
    /* MEASURE (DP): Schedule policy */
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
  Object_ID oid = ACE_OBJECT_COUNTER->increment();

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

      CORBA::Object_var object2 =
        orb->string_to_object (ior2 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server2 =
        Simple_Server::_narrow (object2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      if (CORBA::is_nil (server2.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior2),
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
	  DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_STARTED, 0, sizeof(Object_ID), (char*)&oid);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }


      /*
        Time_Handler *th=new Time_Handler;
        int i;


        for (i = 1; i <= niteration1; i++)
        orb->orb_core()->reactor()->schedule_timer (th,
        (const void *) (orb.in()), // argument sent to handle_timeout()
        ACE_Time_Value (i*period1 )); //set timer to go off with delay

        orb->orb_core()->reactor()->handle_events ();

        i=1;
        ACE_Time_Value check_timer = ACE_Time_Value(period1);
        while(i<niteration1) {
        orb->orb_core()->reactor()->handle_events ();
        i++;
        }
      */

      /*
        ACE_Reactor react_;
        react_.open(0);
        ACE_Time_Value tv;
        tv = ACE_Time_Value(period1);
        Time_Handler *th=new Time_Handler;
        long timer_id = react_.schedule_timer(th,
        0, //NULL arg
        tv, tv);
        if (timer_id < 0)
        {
        ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("(%T|%t) cannot schedule reactor timer.")
        ACE_TEXT ("ACE_Reactor.schedule_timer() returned -1\n")),
        -1);
        }
      */
      int importance=0;

      /* For CV version worker1 should start before worker2, but for Direct version vice versa*/
      /*      Worker_c worker1 (orb.in (),
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
      */
//print out the start time of the program.
  ACE_Time_Value start_time=ACE_OS::gettimeofday();
  ACE_OS::printf ( ACE_TEXT ("The Start time: %u (sec), %u (usec)\n"), start_time.sec(), start_time.usec());
  DSTRM_EVENT(MAIN_GROUP_FAM, START,0,sizeof(Object_ID), (char*)&oid);

      Worker worker2 (orb.in (),
                      server.in (),
                      server2.in (),
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

      /*
        int j=0;
        while(j<20) {
	react_.handle_events();
        ACE_DEBUG((LM_DEBUG,"RETURN From handle_events~~~~~\n"));
        j++;
        }
      */
      //      worker1.wait ();
      worker2.wait ();

      /* MEASURE: Wait for worker thread done in main thread */
      // char* msg = "(%t): wait for worker threads done in main thread\n";
      // Get thread id
      // DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_WAIT_DONE, 1, strlen(msg), msg);

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): wait for worker threads done in main thread\n"));

      DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 0,  sizeof(Object_ID), (char*)&oid);

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
          DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 0, sizeof(Object_ID), (char*)&oid); 

          server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          server2->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          /* MEASURE: After call to server shutdown */
          DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 0,  sizeof(Object_ID), (char*)&oid); 
          ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));


          if (enable_dynamic_scheduling)
            {
              current->end_scheduling_segment (0 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }

      scheduler->shutdown ();

      /* MEASURE: Scheduler stop time */
      DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 0,  sizeof(Object_ID), (char*)&oid); 
      ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  non_dsui_timer.stop();
  ACE_hrtime_t dsui_ovhd_time;
  non_dsui_timer.elapsed_time (dsui_ovhd_time);

  ACE_OS::printf ( ACE_TEXT ("Elapsed time: %lu (nsec)\n"), dsui_ovhd_time);
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb,
                Simple_Server_ptr server_ptr,
                Simple_Server_ptr server2_ptr,
                RTScheduling::Current_ptr current,
                EDF_Scheduler* scheduler,
                long importance,
                CORBA::Long server_load,
                CORBA::Long period,
                CORBA::Long niteration,
		int worker_id)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (server_ptr)),
     server2_ (Simple_Server::_duplicate (server2_ptr)),
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
  Object_ID oid = ACE_OBJECT_COUNTER->increment();
  oid.task_id = ID_BEGIN++; 
  DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, sizeof(Object_ID), (char*)&oid);

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
  int first_task_id, second_task_id;

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

  int sec_period = period_/1000;
  int usec_period = (period_%1000)*1000;

  if (enable_dynamic_scheduling)
    {
      sched_param.importance = importance_;
      ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (sec_period,usec_period)); 
      sched_param.period = period_*10000;
      sched_param.task_id = oid.task_id;
      first_task_id = sched_param.task_id;
      second_task_id = ID_BEGIN++;
      sched_param.id = oid.id;
      sched_param.pid = oid.pid;
      sched_param.tid = oid.tid;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      oid.task_id = sched_param.task_id;
      DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
      //      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
      //      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

  ACE_Time_Value start_t, repair_t;
  repair_t=ACE_Time_Value(0,0);

  for(int i=0;i<niteration_;i++)
    {
      if(i>0 && enable_dynamic_scheduling)
        {
          sched_param.deadline = sched_param.deadline+period_*10000;
          sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);
          oid = ACE_OBJECT_COUNTER->increment();
          oid.task_id = sched_param.task_id;
          sched_param.id = oid.id;
          sched_param.tid = oid.tid;
          sched_param.pid = oid.pid;

          //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
          implicit_sched_param = sched_param_policy;
          DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, 0, sizeof(Object_ID), (char*)&oid);
          scheduler_current_->update_scheduling_segment(name,
                                                        sched_param_policy.in (),
                                                        implicit_sched_param.in ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, 0, sizeof(Object_ID), (char*)&oid);
        }

      //  TAO_debug_level = 1;
      if (i==0)
	start_t =  ACE_OS::gettimeofday ();
      else {
        repair_t = start_t+ACE_Time_Value(sec_period*i,usec_period*i)-ACE_OS::gettimeofday ();
      }

      timeval tv;

      DSTRM_EVENT (TEST_ONE_FAM, START_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

      tv.tv_sec = server_load_/1000;
      tv.tv_usec = (server_load_%1000)*1000;

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG((LM_DEBUG,"(%t|%T)before running the client workload\n"));
#endif

      CPULoad::run(tv);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG((LM_DEBUG,"(%t|%T)after running the client workload\n"));
#endif
      DSTRM_EVENT (TEST_ONE_FAM, STOP_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

      int need_ft=0;
      if (rand() > RAND_MAX/2 ) 
         need_ft = 1;
      DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);

      if (need_ft) {
      server_->test_method (left_work,2 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      ACE_DEBUG ((LM_DEBUG, "BRGIN to make next one way call\n"));
      sched_param.task_id= second_task_id;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);
      implicit_sched_param = sched_param_policy;
      scheduler_current_->update_scheduling_segment(name,
                                                        sched_param_policy.in (),
                                                        implicit_sched_param.in ()
                                                        ACE_ENV_ARG_PARAMETER);

      server2_->test_method (left_work,2 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      sched_param.task_id= first_task_id;
      }
      else {
        server_->test_method (left_work,1 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
      }
 
      /*DTTIME:
        oneway call done on the client side.
      */
      /* MEASURE: One way call done */
      DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);

      scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()),
                                                      Kokyu::BLOCK);
      rand2 = 0.1*rand()/RAND_MAX;
      if(enable_rand)
        {
          int sleep_t = period_ * 1000 -
            (int)(period_ * rand2 * 1000) +
            repair_t.sec()*1000000 + repair_t.usec();

          if(sleep_t > 0)
            {
              ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d.\n",
                         (int)(period_*1000-period_*rand2*1000)));
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
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG((LM_DEBUG,"(%t|%T)NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));
#endif
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
  Object_ID oid = ACE_OBJECT_COUNTER->increment();
  oid.task_id = m_id;
  DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, sizeof(Object_ID), (char*)&oid);

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
      sched_param.id = oid.id;
      sched_param.tid = oid.tid;
      sched_param.pid = oid.pid;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
    }

  ACE_Time_Value start_t, repair_t;
  repair_t=ACE_Time_Value(0,0);

  for(int i=0;i<niteration_;i++)
    {
      if(i>0 && enable_dynamic_scheduling)
        {
          sched_param.deadline = sched_param.deadline+period_*10000000;
          sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

          //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
          implicit_sched_param = sched_param_policy;
          oid = ACE_OBJECT_COUNTER->increment();
          sched_param.id = oid.id;
          sched_param.tid = oid.tid;
          sched_param.pid = oid.pid;

          DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, 0, sizeof(Object_ID), (char*)&oid);
          scheduler_current_->update_scheduling_segment(name,
                                                        sched_param_policy.in (),
                                                        implicit_sched_param.in ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, 0, sizeof(Object_ID), (char*)&oid);
        }

      if (i==0)
        start_t =  ACE_OS::gettimeofday ();
      else {
        repair_t = start_t+ACE_Time_Value(period_*i,0)-ACE_OS::gettimeofday ();
      }

      timeval tv;

      tv.tv_sec = server_load_;
      tv.tv_usec = 0;

      DSTRM_EVENT (WORKER_GROUP_FAM, RUNNING_SUBTASK, 0, sizeof(Object_ID), (char*)&oid);

      CPULoad::run(tv);

      DSTRM_EVENT (WORKER_GROUP_FAM, FINISHING_SUBTASK, 0, sizeof(Object_ID), (char*)&oid);

      scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()),
                                                      Kokyu::BLOCK);
      rand2 = 0.1*rand()/RAND_MAX;
      if(enable_rand)
        {
          int sleep_t = period_*1000000-((int)(period_*rand2*1000000))+repair_t.sec()*1000000+repair_t.usec();
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
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG((LM_DEBUG,"(%t|%T)NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));
#endif
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

