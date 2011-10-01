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
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"

class Roundtrip_Task : public ACE_Task_Base
{
public:
  Roundtrip_Task (Test::Roundtrip_ptr roundtrip,
                  ACE_Barrier *the_barrier)
    : roundtrip_ (Test::Roundtrip::_duplicate (roundtrip))
    , barrier_ (the_barrier)
  {
  }

  virtual void run_test (void) = 0;

  virtual int svc (void)
  {
    this->barrier_->wait ();
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
  Test::Roundtrip_var roundtrip_;

  ACE_Barrier *barrier_;
};

class High_Priority_Task : public Roundtrip_Task
{
public:
  High_Priority_Task (Test::Roundtrip_ptr roundtrip,
                      ACE_Barrier *the_barrier,
                      int iterations,
                      int period_in_usecs,
                      int workload)
    : Roundtrip_Task (roundtrip, the_barrier)
    , sample_history (iterations)
    , iterations_ (iterations)
    , period_in_usecs_ (period_in_usecs)
    , workload_ (workload)
  {
  }

  virtual void run_test (void)
  {
    for (int i = 0; i != this->iterations_; ++i)
      {
        ACE_Time_Value period (0, this->period_in_usecs_);
        ACE_OS::sleep (period);

        try{
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          (void) this->roundtrip_->test_method (start,
                                                this->workload_);
          ACE_hrtime_t elapsed = ACE_OS::gethrtime () - start;

          this->sample_history.sample (elapsed);

        } catch (const CORBA::Exception&) {
        }
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
                     ACE_Barrier *the_barrier,
                     int period_in_usecs,
                     int workload)
    : Roundtrip_Task (roundtrip, the_barrier)
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

  virtual void run_test (void)
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

        try{
          CORBA::ULongLong dummy = 0;
          (void) this->roundtrip_->test_method (dummy,
                                                this->workload_);

        } catch (const CORBA::Exception&) {
        }
      }
  }

private:
  TAO_SYNCH_MUTEX mutex_;

  int stopped_;

  int period_in_usecs_;

  int workload_;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
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

      RTClient_Setup rtclient_setup (options.use_rt_corba,
                                     orb,
                                     rt_class,
                                     options.nthreads);

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      CORBA::PolicyList_var inconsistent_policies;
      (void) roundtrip->_validate_connection (inconsistent_policies);

      int thread_count = 1 + options.nthreads;
      ACE_Barrier the_barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      int per_thread_period = options.low_priority_period;
      if (options.global_low_priority_rate)
        per_thread_period = options.low_priority_period * options.nthreads;
      Low_Priority_Task low_priority (roundtrip.in (), &the_barrier,
                                      per_thread_period,
                                      options.low_priority_workload);
      low_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             options.nthreads, 1,
                             rt_class.priority_low ());

      High_Priority_Task high_priority (roundtrip.in (), &the_barrier,
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
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results (ACE_TEXT("High Priority"), gsf);

      low_priority.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

      roundtrip->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting cleanup\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
