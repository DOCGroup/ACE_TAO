// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/Read_Buffer.h"
#include "ace/Array_Base.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "testC.h"

ACE_RCSID(Thread_Pool, client, "$Id$")

static const char *ior = "file://ior";
static size_t iterations_for_slowest_paced_worker = 1000;
static int shutdown_server = 0;
static int do_dump_history = 0;
static ACE_UINT32 gsf = 0;
static const char *rates_file = "rates";
static size_t continuous_worker_iterations_multipler = 1024;
static size_t continuous_workers = 2;
static int done = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hxk:i:r:c:");
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

      case 'i':
        iterations_for_slowest_paced_worker =
          ACE_OS::atoi (get_opts.optarg);
        break;

      case 'r':
        rates_file = get_opts.optarg;
        break;

      case 'c':
        continuous_workers = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h <show history> "
                           "-x [shutdown server] "
                           "-k <ior> "
                           "-i <iterations for slowest paced worker> "
                           "-r <rates file> "
                           "-c <number of continuous workers> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

typedef ACE_Array_Base<size_t> Rates;

int
get_rates (const char *file_name,
           Rates &rates,
           size_t &lowest_rate)
{
  //
  // Read lanes from a file.
  //
  FILE* file =
    ACE_OS::fopen (file_name, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty lanes file.
  if (string == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nNo rates set!\n\n"));
      return 0;
    }

  size_t length =
    reader.replaced () + 1;

  rates.size (length);

  ACE_DEBUG ((LM_DEBUG,
              "\nThere are %d rates: ",
              length));

  int result = 1;
  char* working_string = string;
  lowest_rate = ACE_UINT32_MAX;
  for (size_t i = 0; i < length; ++i)
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
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  return 0;
}

class Paced_Worker :
  public ACE_Task_Base
{
public:
  Paced_Worker (ACE_Thread_Manager &thread_manager,
                test_ptr test,
                size_t rate,
                size_t iterations,
                ACE_SYNCH_MUTEX &output_lock);

  int svc (void);

  ACE_Time_Value deadline_for_current_call (size_t i);

  test_var test_;
  int rate_;
  ACE_SYNCH_MUTEX &output_lock_;
  ACE_Time_Value interval_between_calls_;
  ACE_Time_Value start_of_test_;
  ACE_Sample_History history_;
};

Paced_Worker::Paced_Worker (ACE_Thread_Manager &thread_manager,
                            test_ptr test,
                            size_t rate,
                            size_t iterations,
                            ACE_SYNCH_MUTEX &output_lock)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    rate_ (rate),
    output_lock_ (output_lock),
    interval_between_calls_ (),
    start_of_test_ (),
    history_ (iterations)
{
  this->interval_between_calls_.set (1 / double (this->rate_));
}

ACE_Time_Value
Paced_Worker::deadline_for_current_call (size_t i)
{
  ACE_Time_Value deadline_for_current_call =
    this->interval_between_calls_;

  deadline_for_current_call *= i;

  deadline_for_current_call += this->start_of_test_;

  return deadline_for_current_call;
}

int
Paced_Worker::svc (void)
{
  size_t deadlines_missed = 0;

  ACE_TRY_NEW_ENV
    {
      this->start_of_test_ =
        ACE_OS::gettimeofday ();

      ACE_hrtime_t test_start =
        ACE_OS::gethrtime ();

      for (size_t i = 0;
           i != this->history_.max_samples ();
           ++i)
        {
          ACE_Time_Value deadline_for_current_call =
            this->deadline_for_current_call (i);

          ACE_Time_Value time_before_call =
            ACE_OS::gettimeofday ();

          if (time_before_call > deadline_for_current_call)
            {
              deadlines_missed++;
              continue;
            }

          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->test_->method (ACE_TRY_ENV);
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
                        this->output_lock_,
                        -1);

      ACE_DEBUG ((LM_DEBUG,
                  "\n************ Statistics for thread %t ************\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Rate = %d/sec; Iterations = %d; "
                  "deadlines made = %d; deadlines missed = %d; Success = %d%%\n",
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
                     ACE_SYNCH_MUTEX &output_lock);

  int svc (void);

  test_var test_;
  ACE_SYNCH_MUTEX &output_lock_;
};

Continuous_Worker::Continuous_Worker (test_ptr test,
                                      ACE_SYNCH_MUTEX &output_lock)
  : test_ (test::_duplicate (test)),
    output_lock_ (output_lock)
{
}

int
Continuous_Worker::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_Sample_History history
        (iterations_for_slowest_paced_worker *
         continuous_worker_iterations_multipler);

      ACE_hrtime_t test_start =
        ACE_OS::gethrtime ();

      for (size_t i = 0;
           i != history.max_samples () && !done;
           ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->test_->method (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          history.sample (end - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                        mon,
                        this->output_lock_,
                        -1);

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
main (int argc, char *argv[])
{
  gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_SYNCH_MUTEX output_lock;

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

      test->method (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Rates rates;
      size_t lowest_rate = 0;

      result =
        get_rates (rates_file,
                   rates,
                   lowest_rate);
      if (result != 0)
        return result;

      size_t time_for_test =
        iterations_for_slowest_paced_worker /
        lowest_rate;

      ACE_Thread_Manager paced_workers_manager;

      size_t i = 0;
      Paced_Worker **paced_workers =
        new Paced_Worker *[rates.size ()];

      for (i = 0;
           i < rates.size ();
           ++i)
        {
          paced_workers[i] =
            new Paced_Worker (paced_workers_manager,
                              test.in (),
                              rates[i],
                              time_for_test * rates[i],
                              output_lock);
        }

      Continuous_Worker continuous_worker (test.in (),
                                           output_lock);
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

      paced_workers_manager.wait ();
      done = 1;

      ACE_Thread_Manager::instance ()->wait ();

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
