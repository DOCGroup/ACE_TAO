// $Id$

#include "Callback.h"

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Client_Options.h"
#include "Implicit_Deactivator.h"
#include "Shutdown.h"
#include "ORB_Task.h"
#include "ORB_Task_Activator.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "ace/Auto_Functor.h"
#include "ace/Auto_Ptr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Task.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"

class Roundtrip_Task : public ACE_Task_Base
{
public:
  Roundtrip_Task (Test::Session_Factory_ptr session_factory,
                  ACE_Barrier *the_barrier)
    : session_factory_ (Test::Session_Factory::_duplicate (session_factory))
    , the_barrier_ (the_barrier)
  {
  }

  virtual void run_test (void) = 0;

  virtual int svc (void)
  {
    this->the_barrier_->wait ();
    try
      {
        this->run_test ();
      }
    catch (const CORBA::Exception&)
      {
      }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) done...\n"));
    return 0;
  }

protected:
  Test::Session_Factory_var session_factory_;

  ACE_Barrier *the_barrier_;
};

class High_Priority_Task : public Roundtrip_Task
{
public:
  High_Priority_Task (Test::Session_Factory_ptr session_factory,
                      ACE_Barrier *the_barrier,
                      PortableServer::POA_ptr poa,
                      int iterations,
                      int period_in_usecs)
    : Roundtrip_Task (session_factory, the_barrier)
    , callback (new Callback (iterations, poa))
    , iterations_ (iterations)
    , period_in_usecs_ (period_in_usecs)
  {
  }

  virtual void run_test (void)
  {
    Test::Callback_var cb =
      callback->_this ();

    Implicit_Deactivator deactivator (callback);

    Test::Session_var session =
      this->session_factory_->create_new_session (cb.in ());

    ACE_Utils::Auto_Functor<Test::Session,Shutdown<Test::Session> > auto_shutdown (session.in ());

    for (int i = 0; i != this->iterations_; ++i)
      {
        if ((i + 1) % 1000 == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Thread has sent %d messages @ %T\n",
                        i + 1));
          }

        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        try{
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          (void) session->sample (start);
        } catch (const CORBA::Exception&) {
        }
      }
  }

  Servant_var<Callback> callback;

private:
  int iterations_;

  int period_in_usecs_;
};

class Low_Priority_Task : public Roundtrip_Task
{
public:
  Low_Priority_Task (Test::Session_Factory_ptr session_factory,
                     ACE_Barrier *the_barrier,
                     PortableServer::POA_ptr poa,
                     int period_in_usecs)
    : Roundtrip_Task (session_factory, the_barrier)
    , callback (new Callback (1, poa))
    , stopped_ (0)
    , period_in_usecs_ (period_in_usecs)
  {
  }

  void stop (void)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->stopped_ = 1;
  }

  virtual void run_test (void)
  {
    Test::Callback_var cb =
      callback->_this ();

    Implicit_Deactivator deactivator (callback);

    Test::Session_var session =
      this->session_factory_->create_new_session (cb.in ());

    ACE_Utils::Auto_Functor<Test::Session,Shutdown<Test::Session> > auto_shutdown (session.in ());

    for (;;)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        {
          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
          if (this->stopped_)
            return;
        }

        try{
          CORBA::ULongLong dummy = 0;
          (void) session->sample (dummy);

        } catch (const CORBA::Exception&) {
        }
      }
  }

private:
  TAO_SYNCH_MUTEX mutex_;

  Servant_var<Callback> callback;

  int stopped_;

  int period_in_usecs_;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  /// Move the test to the real-time class if it is possible.
  RT_Class rt_class;

  try
    {
      ORB_Holder orb (argc, argv, "");

      Client_Options options (argc, argv);
      if (argc != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
                             "-i iterations (iterations) "
                             "-h high_priority_period (usecs) "
                             "-l low_priority_period (usecs) "
                             "-w high_priority_workload (usecs) "
                             "-v low_priority_workload (usecs) "
                             "-r (enable RT-CORBA) "
                             "-n nthreads (low priority thread) "
                             "-d (dump history) "
                             "-z (disable low priority) "
                             "\n",
                             argv [0]),
                            1);
        }

      RTServer_Setup rtserver_setup (options.use_rt_corba,
                                     orb,
                                     rt_class,
                                     options.nthreads);

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA");

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      PortableServer::POA_var the_poa (rtserver_setup.poa ());

      ORB_Task orb_task (orb);
      ORB_Task_Activator orb_task_activator (rt_class.priority_high (),
                                             rt_class.thr_sched_class (),
                                             options.nthreads,
                                             &orb_task);

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      Test::Session_Factory_var session_factory =
        Test::Session_Factory::_narrow (object.in ());

      CORBA::PolicyList_var inconsistent_policies;
      (void) session_factory->_validate_connection (inconsistent_policies);

      int thread_count = 1 + options.nthreads;
      ACE_Barrier the_barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      int per_thread_period = options.low_priority_period;
      if (options.global_low_priority_rate)
        per_thread_period = options.low_priority_period * options.nthreads;
      Low_Priority_Task low_priority (session_factory.in (),
                                      &the_barrier,
                                      the_poa.in (),
                                      per_thread_period);
      low_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             options.nthreads, 1,
                             rt_class.priority_low ());

      High_Priority_Task high_priority (session_factory.in (),
                                        &the_barrier,
                                        the_poa.in (),
                                        options.iterations,
                                        options.high_priority_period);
      high_priority.activate (rt_class.thr_sched_class ()
                              | THR_NEW_LWP | THR_JOINABLE,
                              1, 1,
                              rt_class.priority_low ());

      high_priority.wait ();
      low_priority.stop ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high prio task joined\n"));

      ACE_Sample_History &history =
        high_priority.callback->sample_history ();
      if (options.dump_history)
        {
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results (ACE_TEXT("High Priority"), gsf);

      low_priority.wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

      session_factory->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting cleanup\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
