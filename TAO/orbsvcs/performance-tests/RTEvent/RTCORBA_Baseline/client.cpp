// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Send_Task.h"
#include "Low_Priority_Setup.h"
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

ACE_RCSID(TAO_RTEC_PERF_RTCORBA_Baseline, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 10000;
int nthreads   = 0;
int do_dump_history = 0;
int high_priority_period = 0;
int high_priority_workload = 0;
int low_priority_period  = 0;
int low_priority_workload = 0;
int disable_low_priority = 0;
int use_rt_corba = 0;

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
                      int period_in_usecs)
    : Roundtrip_Task (roundtrip, barrier)
    , sample_history (iterations)
    , iterations_ (iterations)
    , period_in_usecs_ (period_in_usecs)
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
          (void) this->roundtrip_->test_method (start
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
};

class Low_Priority_Task : public Roundtrip_Task
{
public:
  Low_Priority_Task (Test::Roundtrip_ptr roundtrip,
                      ACE_Barrier *barrier)
    : Roundtrip_Task (roundtrip, barrier)
    , stopped_ (0)
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
        ACE_TRY {
          CORBA::ULongLong dummy = 0;
          (void) this->roundtrip_->test_method (dummy
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
          if (this->stopped_)
            return;

        } ACE_CATCHANY {
        } ACE_ENDTRY;
      }
  }

private:
  TAO_SYNCH_MUTEX mutex_;

  int stopped_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:n:l:h:w:v:zdr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'h':
        high_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        low_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        high_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'v':
        low_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        do_dump_history = 1;
        break;

      case 'z':
        disable_low_priority = 1;
        break;

      case 'r':
        use_rt_corba = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <IOR> "
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
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  RT_Class rt_class;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      RTServer_Setup rtserver_setup (use_rt_corba,
                                     orb,
                                     rt_class
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList_var inconsistent_policies;
      (void) roundtrip->_validate_connection (inconsistent_policies
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int thread_count = 1;
      if (disable_low_priority == 0)
        thread_count += nthreads;
      ACE_Barrier barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      Low_Priority_Task low_priority (roundtrip.in (), &barrier);
      low_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             nthreads, 1,
                             rt_class.priority_low ());

      High_Priority_Task high_priority (roundtrip.in (), &barrier,
                                        iterations, high_priority_period);
      high_priority.activate (rt_class.thr_sched_class ()
                             | THR_NEW_LWP | THR_JOINABLE,
                             nthreads, 1,
                             rt_class.priority_low ());

      high_priority.wait ();
      low_priority.stop ();
      low_priority.wait ();
                                        
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

      ACE_Sample_History &history = high_priority.sample_history;
      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results ("High Priority", gsf);

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

template class Servant_var<Supplier>;
template class Servant_var<Consumer>;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<Supplier> >;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<Consumer> >;
template class ACE_Auto_Basic_Array_Ptr<Send_Task>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<Supplier>
#pragma instantiate Servant_var<Consumer>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<Supplier> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<Consumer> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Send_Task>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
