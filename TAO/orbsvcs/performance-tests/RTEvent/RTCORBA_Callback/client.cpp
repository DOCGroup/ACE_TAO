// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Client_Options.h"
#include "Callback.h"
#include "Implicit_Deactivator.h"
#include "Shutdown.h"
#include "Auto_Functor.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "ace/Auto_Ptr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Task.h"

ACE_RCSID(TAO_PERF_RTEC_RTCORBA_Baseline, client, "$Id$")

class Roundtrip_Task : public ACE_Task_Base
{
public:
  Roundtrip_Task (Test::Session_Factory_ptr session_factory,
                  ACE_Barrier *barrier)
    : session_factory_ (Test::Session_Factory::_duplicate (session_factory))
    , barrier_ (barrier)
  {
  }

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL) = 0;

  virtual int svc (void)
  {
    this->barrier_->wait ();
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        this->run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
      }
    ACE_ENDTRY;

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) done...\n"));
    return 0;
  }

protected:
  Test::Session_Factory_var session_factory_;

  ACE_Barrier *barrier_;
};

class High_Priority_Task : public Roundtrip_Task
{
public:
  High_Priority_Task (Test::Session_Factory_ptr session_factory,
                      ACE_Barrier *barrier,
                      PortableServer::POA_ptr poa,
                      int iterations,
                      int period_in_usecs)
    : Roundtrip_Task (session_factory, barrier)
    , callback (new Callback (iterations, poa))
    , iterations_ (iterations)
    , period_in_usecs_ (period_in_usecs)
  {
  }

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL)
  {
    Test::Callback_var cb =
      callback->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    Implicit_Deactivator deactivator (callback);

    Test::Session_var session =
      this->session_factory_->create_new_session (cb.in ()
                                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Auto_Functor<Test::Session,Shutdown<Test::Session> > auto_shutdown (session.in ());

    for (int i = 0; i != this->iterations_; ++i)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        ACE_TRY {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          (void) session->sample (start
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        } ACE_CATCHANY {
        } ACE_ENDTRY;
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
                     ACE_Barrier *barrier,
                     PortableServer::POA_ptr poa,
                     int period_in_usecs)
    : Roundtrip_Task (session_factory, barrier)
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

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL)
  {
    Test::Callback_var cb =
      callback->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    Implicit_Deactivator deactivator (callback);

    Test::Session_var session =
      this->session_factory_->create_new_session (cb.in ()
                                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Auto_Functor<Test::Session,Shutdown<Test::Session> > auto_shutdown (session.in ());

    for (;;)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
        if (this->stopped_)
          return;


        ACE_TRY {
          CORBA::ULongLong dummy = 0;
          (void) session->sample (dummy
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        } ACE_CATCHANY {
        } ACE_ENDTRY;
      }
  }

private:
  TAO_SYNCH_MUTEX mutex_;

  Servant_var<Callback> callback;

  int stopped_;

  int period_in_usecs_;
};

int main (int argc, char *argv[])
{
  /// Move the test to the real-time class if it is possible.
  RT_Class rt_class;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                                     options.nthreads
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var the_poa (rtserver_setup.poa ());

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (options.ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Session_Factory_var session_factory =
        Test::Session_Factory::_narrow (object.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList_var inconsistent_policies;
      (void) session_factory->_validate_connection (inconsistent_policies
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int thread_count = 1 + options.nthreads;
      ACE_Barrier barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      int per_thread_period = options.low_priority_period;
      if (options.global_low_priority_rate)
        per_thread_period = options.low_priority_period * options.nthreads;
      Low_Priority_Task low_priority (session_factory.in (),
                                      &barrier,
                                      the_poa.in (),
                                      per_thread_period);
      low_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             options.nthreads, 1,
                             rt_class.priority_low ());

      High_Priority_Task high_priority (session_factory.in (),
                                        &barrier,
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
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results ("High Priority", gsf);

      low_priority.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

      session_factory->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting cleanup\n"));
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
