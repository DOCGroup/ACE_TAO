// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/Read_Buffer.h"
#include "ace/Array_Base.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "testC.h"

ACE_RCSID(Thread_Pool, client, "$Id$")

static const char *ior = "file://ior";
static int shutdown_server = 0;
static int do_dump_history = 0;
static ACE_UINT32 gsf = 0;
static const char *rates_file = "rates";
static CORBA::ULong continuous_workers = 0;
static int done = 0;
static CORBA::ULong time_for_test = 10;
static CORBA::ULong work = 10;
static CORBA::ULong max_throughput_timeout = 5;
static int set_priority = 1;
static int individual_continuous_worker_stats = 0;
static ACE_Time_Value start_of_test;
static ACE_hrtime_t test_start;
static int print_missed_invocations = 0;

struct Synchronizers
{
  Synchronizers (void)
    : worker_lock_ (),
      workers_ (1),
      main_thread_ (),
      workers_ready_ (0),
      number_of_workers_ (0)
  {
  }

  ACE_SYNCH_MUTEX worker_lock_;
  ACE_Event workers_;
  ACE_Event main_thread_;
  CORBA::ULong workers_ready_;
  CORBA::ULong number_of_workers_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hxk:r:c:w:t:p:i:m:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case 'r':
        rates_file = get_opts.optarg;
        break;

      case 'c':
        continuous_workers = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'w':
        work = ACE_OS::atoi (get_opts.optarg);
        break;

      case 't':
        time_for_test = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'p':
        set_priority = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'i':
        individual_continuous_worker_stats =
          ACE_OS::atoi (get_opts.optarg);
        break;

      case 'm':
        print_missed_invocations =
          ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h <show history> "
                           "-x [shutdown server] "
                           "-k <ior> "
                           "-r <rates file> "
                           "-c <number of continuous workers> "
                           "-w <work> "
                           "-t <time for test> "
                           "-p <set priorities> "
                           "-i <print stats of individual continuous workers> "
                           "-m <print missed invocations for paced workers> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

typedef ACE_Array_Base<CORBA::ULong> Rates;

int
get_rates (const char *file_name,
           Rates &rates,
           CORBA::ULong &lowest_rate)
{
  //
  // Read rates from a file.
  //
  FILE* file =
    ACE_OS::fopen (file_name, "r");

  if (file == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open rates file %s\n",
                         file_name),
                        -1);
    }

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty lanes file.
  if (string == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nNo rates set!\n"));
      return 0;
    }

  CORBA::ULong length =
    reader.replaced () + 1;

  rates.size (length);

  ACE_DEBUG ((LM_DEBUG,
              "\nThere are %d rates: ",
              length));

  int result = 1;
  char* working_string = string;
  lowest_rate = ACE_UINT32_MAX;
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      result = ::sscanf (working_string,
                         "%d",
                         &rates[i]);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      if (lowest_rate > rates[i])
        lowest_rate = rates[i];

      ACE_DEBUG ((LM_DEBUG,
                  "[%d] ",
                  rates[i]));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Parsing error in rates file %s\n",
                         file_name),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  return 0;
}

int
synchronize (test_ptr test,
             Synchronizers &synchronizers)
{
  ACE_TRY_NEW_ENV
    {
      test->method (work,
                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      synchronizers.worker_lock_,
                      -1);

    if (synchronizers.workers_ready_ == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\n"));
      }

    ++synchronizers.workers_ready_;

    ACE_DEBUG ((LM_DEBUG,
                "%d worker ready\n",
                synchronizers.workers_ready_));

    if (synchronizers.workers_ready_ ==
        synchronizers.number_of_workers_)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\n"));

        synchronizers.main_thread_.signal ();
      }
  }

  synchronizers.workers_.wait ();

  return 0;
}

class Paced_Worker :
  public ACE_Task_Base
{
public:
  Paced_Worker (ACE_Thread_Manager &thread_manager,
                test_ptr test,
                CORBA::ULong rate,
                CORBA::ULong iterations,
                CORBA::Short priority,
                RTCORBA::Current_ptr current,
                RTCORBA::PriorityMapping &priority_mapping,
                Synchronizers &synchronizers);

  int svc (void);
  ACE_Time_Value deadline_for_current_call (CORBA::ULong i);

  test_var test_;
  int rate_;
  Synchronizers &synchronizers_;
  ACE_Time_Value interval_between_calls_;
  ACE_Sample_History history_;
  CORBA::Short priority_;
  RTCORBA::Current_var current_;
  RTCORBA::PriorityMapping &priority_mapping_;
};

Paced_Worker::Paced_Worker (ACE_Thread_Manager &thread_manager,
                            test_ptr test,
                            CORBA::ULong rate,
                            CORBA::ULong iterations,
                            CORBA::Short priority,
                            RTCORBA::Current_ptr current,
                            RTCORBA::PriorityMapping &priority_mapping,
                            Synchronizers &synchronizers)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    rate_ (rate),
    synchronizers_ (synchronizers),
    interval_between_calls_ (),
    history_ (iterations),
    priority_ (priority),
    current_ (RTCORBA::Current::_duplicate (current)),
    priority_mapping_ (priority_mapping)
{
  this->interval_between_calls_.set (1 / double (this->rate_));
}

ACE_Time_Value
Paced_Worker::deadline_for_current_call (CORBA::ULong i)
{
  ACE_Time_Value deadline_for_current_call =
    this->interval_between_calls_;

  deadline_for_current_call *= i;

  deadline_for_current_call += start_of_test;

  return deadline_for_current_call;
}

int
Paced_Worker::svc (void)
{
  CORBA::ULong deadlines_missed = 0;

  ACE_TRY_NEW_ENV
    {
      if (set_priority)
        {
          this->current_->the_priority (this->priority_,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          this->current_->the_priority (0,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      CORBA::Short CORBA_priority =
        this->current_->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Short native_priority = 0;
      CORBA::Boolean result =
        priority_mapping_.to_native (CORBA_priority,
                                     native_priority);
      if (!result)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in converting CORBA priority %d to native priority\n",
                           CORBA_priority),
                          -1);

      typedef ACE_Array_Base<CORBA::ULong> Missed_Invocations;
      Missed_Invocations missed_invocations (this->history_.max_samples ());

      int synchronize_result =
        synchronize (this->test_.in (),
                     this->synchronizers_);
      if (synchronize_result != 0)
        return synchronize_result;

      for (CORBA::ULong i = 0;
           i != this->history_.max_samples ();
           ++i)
        {
          ACE_Time_Value deadline_for_current_call =
            this->deadline_for_current_call (i);

          ACE_Time_Value time_before_call =
            ACE_OS::gettimeofday ();

          if (time_before_call > deadline_for_current_call)
            {
              missed_invocations[deadlines_missed] = i + 1;
              deadlines_missed++;
              continue;
            }

          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->test_->method (work,
                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          this->history_.sample (end - start);

          ACE_Time_Value time_after_call =
            ACE_OS::gettimeofday ();

          if (time_after_call > deadline_for_current_call)
            continue;

          ACE_Time_Value sleep_time =
            deadline_for_current_call - time_after_call;

          ACE_OS::sleep (sleep_time);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                        mon,
                        this->synchronizers_.worker_lock_,
                        -1);

      ACE_DEBUG ((LM_DEBUG,
                  "\n************ Statistics for thread %t ************\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Priority = %d/%d; Rate = %d/sec; Iterations = %d; "
                  "deadlines made = %d; deadlines missed = %d; Success = %d%%\n",
                  CORBA_priority,
                  native_priority,
                  this->rate_,
                  this->history_.max_samples (),
                  this->history_.sample_count (),
                  deadlines_missed,
                  this->history_.sample_count () * 100 /
                  this->history_.max_samples ()));

      if (do_dump_history)
        {
          this->history_.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      this->history_.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

      if (print_missed_invocations)
        {
          ACE_DEBUG ((LM_DEBUG, "\nMissed invocations are: "));

          for (CORBA::ULong j = 0;
               j != deadlines_missed;
               ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%d ",
                          missed_invocations[j]));
            }

          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

class Continuous_Worker :
  public ACE_Task_Base
{
public:
  Continuous_Worker (test_ptr test,
                     CORBA::ULong iterations,
                     RTCORBA::Current_ptr current,
                     Synchronizers &synchronizers);

  int svc (void);

  test_var test_;
  CORBA::ULong iterations_;
  Synchronizers &synchronizers_;
  RTCORBA::Current_var current_;
  ACE_Basic_Stats collective_stats_;
  ACE_hrtime_t time_for_test_;
};

Continuous_Worker::Continuous_Worker (test_ptr test,
                                      CORBA::ULong iterations,
                                      RTCORBA::Current_ptr current,
                                      Synchronizers &synchronizers)
  : test_ (test::_duplicate (test)),
    iterations_ (iterations),
    synchronizers_ (synchronizers),
    current_ (RTCORBA::Current::_duplicate (current)),
    time_for_test_ (0)
{
}

int
Continuous_Worker::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->current_->the_priority (0,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Sample_History history (this->iterations_);

      int synchronize_result =
        synchronize (this->test_.in (),
                     this->synchronizers_);
      if (synchronize_result != 0)
        return synchronize_result;

      for (CORBA::ULong i = 0;
           i != history.max_samples () && !done;
           ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->test_->method (work,
                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          history.sample (end - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                        mon,
                        this->synchronizers_.worker_lock_,
                        -1);

      if (individual_continuous_worker_stats)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\n************ Statistics for thread %t ************\n\n"));

          ACE_DEBUG ((LM_DEBUG,
                      "Iterations = %d\n",
                      history.sample_count ()));

          if (do_dump_history)
            {
              history.dump_samples ("HISTORY", gsf);
            }

          ACE_Basic_Stats stats;
          history.collect_basic_stats (stats);
          stats.dump_results ("Total", gsf);

          ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                                 test_end - test_start,
                                                 stats.samples_count ());
        }

      // Collective samples.
      history.collect_basic_stats (this->collective_stats_);
      ACE_hrtime_t elapsed_time_for_current_thread =
        test_end - test_start;
      if (elapsed_time_for_current_thread > this->time_for_test_)
        this->time_for_test_ = elapsed_time_for_current_thread;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
max_throughput (test_ptr test,
                RTCORBA::Current_ptr current,
                RTCORBA::PriorityMapping &priority_mapping,
                CORBA::ULong &max_rate)
{
  CORBA::ULong calls_made = 0;
  CORBA::Short CORBA_priority = 0;
  CORBA::Short native_priority = 0;

  ACE_TRY_NEW_ENV
    {
      CORBA_priority =
        current->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Boolean result =
        priority_mapping.to_native (CORBA_priority,
                                    native_priority);
      if (!result)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in converting CORBA priority %d to native priority\n",
                           CORBA_priority),
                          -1);

      ACE_Time_Value start =
        ACE_OS::gettimeofday ();

      ACE_Time_Value end =
        start + ACE_Time_Value (max_throughput_timeout);

      for (;;)
        {
          ACE_Time_Value now =
            ACE_OS::gettimeofday ();

          if (now > end)
            break;

          test->method (work,
                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ++calls_made;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  max_rate =
    calls_made / max_throughput_timeout;

  ACE_DEBUG ((LM_DEBUG,
              "\nPriority = %d/%d; Max rate calculations => %d calls in %d seconds; Max rate = %d\n",
              CORBA_priority,
              native_priority,
              calls_made,
              max_throughput_timeout,
              max_rate));

  return 0;
}

int
main (int argc, char *argv[])
{
  Synchronizers synchronizers;

  gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (),
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("PriorityMappingManager",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in (),
                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::PriorityMapping &priority_mapping =
        *mapping_manager->mapping ();

      Rates rates;
      CORBA::ULong lowest_rate = 0;

      result =
        get_rates (rates_file,
                   rates,
                   lowest_rate);
      if (result != 0)
        return result;

      synchronizers.number_of_workers_ =
        rates.size () + continuous_workers;

      CORBA::ULong max_rate = 0;
      result =
        max_throughput (test.in (),
                        current.in (),
                        priority_mapping,
                        max_rate);
      if (result != 0)
        return result;

      CORBA::Short priority_range =
        RTCORBA::maxPriority - RTCORBA::minPriority;

      ACE_Thread_Manager paced_workers_manager;

      CORBA::ULong i = 0;
      Paced_Worker **paced_workers =
        new Paced_Worker *[rates.size ()];

      for (i = 0;
           i < rates.size ();
           ++i)
        {
          CORBA::Short priority =
            CORBA::Short ((priority_range /
                           double (rates.size ())) *
                          (i + 1));

          paced_workers[i] =
            new Paced_Worker (paced_workers_manager,
                              test.in (),
                              rates[i],
                              time_for_test * rates[i],
                              priority,
                              current.in (),
                              priority_mapping,
                              synchronizers);
        }

      Continuous_Worker continuous_worker (test.in (),
                                           max_rate * time_for_test,
                                           current.in (),
                                           synchronizers);
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE;

      result =
        continuous_worker.activate (flags,
                                    continuous_workers);
      if (result != 0)
        return result;

      for (i = 0;
           i < rates.size ();
           ++i)
        {
          flags =
            THR_NEW_LWP |
            THR_JOINABLE |
            orb->orb_core ()->orb_params ()->scope_policy () |
            orb->orb_core ()->orb_params ()->sched_policy ();

          result =
            paced_workers[i]->activate (flags);
          if (result != 0)
            return result;
        }

      if (synchronizers.number_of_workers_ > 0)
        synchronizers.main_thread_.wait ();

      start_of_test =
        ACE_OS::gettimeofday ();

      test_start =
        ACE_OS::gethrtime ();

      synchronizers.workers_.signal ();

      if (rates.size () != 0)
        {
          paced_workers_manager.wait ();
          done = 1;
        }

      ACE_Thread_Manager::instance ()->wait ();

      if (continuous_workers > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\n************ Statistics for continuous workers ************\n\n"));

          ACE_DEBUG ((LM_DEBUG,
                      "Collective iterations = %d; Workers = %d; Average = %d\n",
                      continuous_worker.collective_stats_.samples_count (),
                      continuous_workers,
                      continuous_worker.collective_stats_.samples_count () /
                      continuous_workers));

          continuous_worker.collective_stats_.dump_results ("Collective", gsf);

          ACE_Throughput_Stats::dump_throughput ("Collective", gsf,
                                                 continuous_worker.time_for_test_,
                                                 continuous_worker.collective_stats_.samples_count () /
                                                 continuous_workers);

          ACE_Throughput_Stats::dump_throughput ("Individual", gsf,
                                                 continuous_worker.time_for_test_,
                                                 continuous_worker.collective_stats_.samples_count ());
        }

      for (i = 0;
           i < rates.size ();
           ++i)
        {
          delete paced_workers[i];
        }
      delete[] paced_workers;

      if (shutdown_server)
        {
          test->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
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
template class ACE_Array_Base<CORBA::ULong>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<CORBA::ULong>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
