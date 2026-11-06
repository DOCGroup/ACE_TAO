#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_unistd.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "MUF_Scheduler.h"
#include "orbsvcs/Time_Utilities.h"

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  int niterations = 5;
  bool do_shutdown = false;
  bool enable_dynamic_scheduling = false;
  bool enable_yield = true;
};

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb,
          Simple_Server_ptr server_ptr,
          RTScheduling::Current_ptr current,
          MUF_Scheduler *scheduler,
          TimeBase::TimeT deadline,
          TimeBase::TimeT estimated_initial_execution_time,
          long criticality,
          CORBA::Long server_load);
  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var orb_;
  // The orb

  Simple_Server_var server_;
  RTScheduling::Current_var scheduler_current_;
  // Borrowed from main; workers finish before the scheduler is shut down.
  MUF_Scheduler *scheduler_;
  TimeBase::TimeT deadline_;
  TimeBase::TimeT estimated_initial_execution_time_;
  long criticality_;
  CORBA::Long server_load_;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:ds"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.do_shutdown = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  MUF_Scheduler *scheduler {};
  RTScheduling::Current_var current;
  int prio {};
  ACE_Sched_Params::Policy const sched_policy = ACE_SCHED_RR;
  int const sched_scope = ACE_SCOPE_THREAD;
  int const max_prio = ACE_Sched_Params::priority_max (sched_policy,
                                                       sched_scope);
  long flags {};

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  ACE_hthread_t main_thr_handle {};
  ACE_Thread::self (main_thr_handle);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Sched_Params sched_params (sched_policy, max_prio);
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_OS::sched_params (sched_params);

  if (ACE_Thread::getprio (main_thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")));
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

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      if (options.enable_dynamic_scheduling)
        {
          ACE_DEBUG ((LM_DEBUG, "Dyn Sched enabled\n"));
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
                          MUF_Scheduler (orb.in (),
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

      TimeBase::TimeT deadline {};
      TimeBase::TimeT exec_time {};
      long criticality {};

      ORBSVCS_Time::Time_Value_to_TimeT (deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (50,0) );

      ORBSVCS_Time::Time_Value_to_TimeT (exec_time,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (10,0) );

      Worker worker1 (orb.in (),
                      server.in (),
                      current.in (),
                      scheduler,
                      deadline,
                      exec_time,
                      criticality,
                      30);

      if (worker1.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate worker thread.\n"));
        }

      ACE_OS::sleep(2);

      ORBSVCS_Time::Time_Value_to_TimeT (deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (30,0) );

      ORBSVCS_Time::Time_Value_to_TimeT (exec_time,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (10,0) );

      criticality = 0;
      Worker worker2 (orb.in (),
                      server.in (),
                      current.in (),
                      scheduler,
                      deadline,
                      exec_time,
                      criticality,
                      10);

      if (worker2.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate scheduler thread in RT mode.\n"));
        }

      ORBSVCS_Time::Time_Value_to_TimeT (deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (100,0) );

      ORBSVCS_Time::Time_Value_to_TimeT (exec_time,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (10,0) );
      criticality = 1;

      Worker worker3 (orb.in (),
                      server.in (),
                      current.in (),
                      scheduler,
                      deadline,
                      exec_time,
                      criticality,
                      5);
      if (worker3.activate (flags, 1, 0, max_prio) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t|%T) cannot activate scheduler thread in RT mode.\n"));
        }

      worker1.wait ();
      worker2.wait ();
      worker3.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): wait for worker threads done in main thread\n"));

      if (options.do_shutdown)
        {
          if (options.enable_dynamic_scheduling)
            {
              MUF_Scheduling::SchedulingParameter sched_param {};
              sched_param.criticality = 0;
              sched_param.deadline = 0;
              sched_param.estimated_initial_execution_time = 0;
              CORBA::Policy_var sched_param_policy =
                scheduler->create_scheduling_parameter (sched_param);
              CORBA::Policy_ptr implicit_sched_param {};
              current->begin_scheduling_segment (nullptr,
                                                 sched_param_policy.in (),
                                                 implicit_sched_param);
            }

          ACE_DEBUG ((LM_DEBUG, "(%t): about to call server shutdown\n"));
          server->shutdown ();
          ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));

          if (options.enable_dynamic_scheduling)
            {
              current->end_scheduling_segment (nullptr);
            }
        }

      if (scheduler != nullptr)
        scheduler->shutdown ();
      ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
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
                MUF_Scheduler *scheduler,
                TimeBase::TimeT deadline,
                TimeBase::TimeT estimated_initial_execution_time,
                long criticality,
                CORBA::Long server_load)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (server_ptr)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     deadline_ (deadline),
     estimated_initial_execution_time_ (estimated_initial_execution_time),
     criticality_ (criticality),
     server_load_ (server_load)
{
}

int
Worker::svc ()
{
  ACE_hthread_t thr_handle {};
  ACE_Thread::self (thr_handle);
  int prio {};

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t|%T) worker activated with prio %d\n", prio));

  if (this->scheduler_ != nullptr)
    {
      MUF_Scheduling::SchedulingParameter sched_param {};
      CORBA::Policy_var sched_param_policy;
      sched_param.criticality = criticality_;
      sched_param.deadline = deadline_;
      sched_param.estimated_initial_execution_time = 0;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);
      CORBA::Policy_ptr implicit_sched_param {};
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));
      scheduler_current_->begin_scheduling_segment (nullptr,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%t|%T):about to make two way call\n"));
  server_->test_method (server_load_);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):two way call done\n"));

  if (this->scheduler_ != nullptr)
    {
      scheduler_current_->end_scheduling_segment (nullptr);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}
