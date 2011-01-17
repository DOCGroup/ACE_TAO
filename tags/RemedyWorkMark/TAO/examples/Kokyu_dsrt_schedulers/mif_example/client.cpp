// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_unistd.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "MIF_Scheduler.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int do_shutdown = 0;
int enable_dynamic_scheduling = 0;
int enable_yield = 1;

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
          MIF_Scheduler* scheduler,
          CORBA::Short importance,
          CORBA::Long server_load);
    //          int sleep_time);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb

  Simple_Server_var server_;
  RTScheduling::Current_var scheduler_current_;
  MIF_Scheduler* scheduler_;
  CORBA::Short importance_;
  CORBA::Long server_load_;
  int sleep_time_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:ds"));
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

      case 's':
        enable_yield = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-d (enable dynamic scheduling)"
                           "-s (disable yield)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  MIF_Scheduler* scheduler=0;
  RTScheduling::Current_var current;
  int prio;
  int max_prio;
  ACE_Sched_Params::Policy  sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;
  long flags;

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  ACE_hthread_t main_thr_handle;
  ACE_Thread::self (main_thr_handle);

  max_prio = ACE_Sched_Params::priority_max (sched_policy,
                                             sched_scope);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Sched_Params sched_params (sched_policy, max_prio);
  //FUZZ: enable check_for_lack_ACE_OS

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

  try
    {
      RTScheduling::Scheduler_var sched_owner;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      if (enable_dynamic_scheduling)
        {
          ACE_DEBUG ((LM_DEBUG, "Dyn Sched enabled\n"));
          CORBA::Object_var manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager");

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

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
          sched_owner = scheduler;

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current");

          current  =
            RTScheduling::Current::_narrow (object.in ());

        }

      Worker worker1 (orb.in (), server.in (), current.in (), scheduler, 10, 15);

      if (worker1.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate worker thread.\n"));
        }

      ACE_OS::sleep(2);

      Worker worker2 (orb.in (), server.in (), current.in (), scheduler, 12, 5);

      if (worker2.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate scheduler thread in RT mode.\n"));
        }

      worker1.wait ();
      worker2.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): wait for worker threads done in main thread\n"));

      if (do_shutdown)
        {
          if (enable_dynamic_scheduling)
            {
              MIF_Scheduling::SchedulingParameter sched_param;
              sched_param.importance = 0;
              CORBA::Policy_var sched_param_policy =
                scheduler->create_scheduling_parameter (sched_param);
              CORBA::Policy_ptr implicit_sched_param = 0;
              current->begin_scheduling_segment (0,
                                                 sched_param_policy.in (),
                                                 implicit_sched_param);
            }

            ACE_DEBUG ((LM_DEBUG, "(%t): about to call server shutdown\n"));
            server->shutdown ();

            ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));


            if (enable_dynamic_scheduling)
            {
              current->end_scheduling_segment (0);
            }
        }

      scheduler->shutdown ();
      ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb,
                Simple_Server_ptr server_ptr,
                RTScheduling::Current_ptr current,
                MIF_Scheduler* scheduler,
                CORBA::Short importance,
                CORBA::Long server_load)
  //                int sleep_time)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (server_ptr)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     importance_ (importance),
     server_load_ (server_load)
     //     sleep_time_ (sleep_time)
{
}

int
Worker::svc (void)
{
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
      MIF_Scheduling::SchedulingParameter sched_param;
      CORBA::Policy_var sched_param_policy;
      sched_param.importance = importance_;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);
      CORBA::Policy_ptr implicit_sched_param = 0;
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));
      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%t|%T):about to make two way call\n"));
  server_->test_method (server_load_);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):two way call done\n"));

  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}
