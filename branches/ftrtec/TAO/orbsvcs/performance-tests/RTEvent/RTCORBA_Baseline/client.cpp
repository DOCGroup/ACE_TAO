// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "RIR_Narrow.h"
#include "RTClient_Setup.h"
#include "Client_Options.h"
#include "TestC.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Task.h"

ACE_RCSID(TAO_RTEC_PERF_RTCORBA_Baseline, client, "$Id$")

class Roundtrip_Task : public ACE_Task_Base
{
public:
  Roundtrip_Task (Test::Roundtrip_ptr roundtrip,
                  ACE_Barrier *barrier)
    : roundtrip_ (Test::Roundtrip::_duplicate (roundtrip))
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
  Test::Roundtrip_var roundtrip_;

  ACE_Barrier *barrier_;
};

class High_Priority_Task : public Roundtrip_Task
{
public:
  High_Priority_Task (Test::Roundtrip_ptr roundtrip,
                      ACE_Barrier *barrier,
                      int iterations,
                      int period_in_usecs,
                      int workload)
    : Roundtrip_Task (roundtrip, barrier)
    , sample_history (iterations)
    , iterations_ (iterations)
    , period_in_usecs_ (period_in_usecs)
    , workload_ (workload)
  {
  }

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL)
  {
    for (int i = 0; i != this->iterations_; ++i)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        ACE_TRY {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          (void) this->roundtrip_->test_method (start,
                                                this->workload_
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_hrtime_t elapsed = ACE_OS::gethrtime () - start;

          this->sample_history.sample (elapsed);

        } ACE_CATCHANY {
        } ACE_ENDTRY;
      }
  }

  ACE_Sample_History sample_history;

private:
  int iterations_;

  int period_in_usecs_;

  int workload_;
};

class Low_Priority_Task : public Roundtrip_Task
{
public:
  Low_Priority_Task (Test::Roundtrip_ptr roundtrip,
                     ACE_Barrier *barrier,
                     int period_in_usecs,
                     int workload)
    : Roundtrip_Task (roundtrip, barrier)
    , stopped_ (0)
    , period_in_usecs_ (period_in_usecs)
    , workload_ (workload)
  {
  }

  void stop (void)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->stopped_ = 1;
  }

  virtual void run_test (ACE_ENV_SINGLE_ARG_DECL)
  {
    for (;;)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        {
          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
          if (this->stopped_)
            return;
        }

        ACE_TRY {
          CORBA::ULongLong dummy = 0;
          (void) this->roundtrip_->test_method (dummy,
                                                this->workload_
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        } ACE_CATCHANY {
        } ACE_ENDTRY;
      }
  }

private:
  TAO_SYNCH_MUTEX mutex_;

  int stopped_;

  int period_in_usecs_;

  int workload_;
};

int main (int argc, char *argv[])
{
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

      RTClient_Setup rtclient_setup (options.use_rt_corba,
                                     orb,
                                     rt_class,
                                     options.nthreads
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (options.ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList_var inconsistent_policies;
      (void) roundtrip->_validate_connection (inconsistent_policies
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
      Low_Priority_Task low_priority (roundtrip.in (), &barrier,
                                      per_thread_period,
                                      options.low_priority_workload);
      low_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             options.nthreads, 1,
                             rt_class.priority_low ());

      High_Priority_Task high_priority (roundtrip.in (), &barrier,
                                        options.iterations,
                                        options.high_priority_period,
                                        options.high_priority_workload);
      high_priority.activate (rt_class.thr_sched_class ()
                              | THR_NEW_LWP | THR_JOINABLE,
                              1, 1,
                              rt_class.priority_low ());

      high_priority.wait ();
      low_priority.stop ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high prio task joined\n"));

      ACE_Sample_History &history = high_priority.sample_history;
      if (options.dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results ("High Priority", gsf);

      low_priority.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

      roundtrip->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
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
