// -*- C++ -*-
// $Id$

#include <vector>
#include <fstream>
#include <algorithm>

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/Read_Buffer.h"
#include "ace/Array_Base.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Event.h"
#include "ace/Barrier.h"

#include "tao/PortableServer/Servant_Base.h"
#include "tao/ORBInitializer_Registry.h"

#include "orbsvcs/orbsvcs/LWFT/ForwardingAgent.h"
#include "orbsvcs/orbsvcs/LWFT/ForwardingAgent_Thread.h"
#include "orbsvcs/orbsvcs/LWFT/Client_ORBInitializer.h"

#include "testC.h"

static const char *ior = "file://s1.ior";
static const char *rates_file = "rates";
static int shutdown_server = 0;
static int do_dump_history = 0;
static const char *history_file_name;
static ACE_UINT32 gsf = 0;
static CORBA::ULong continuous_workers = 0;
static int done = 0;
static CORBA::ULong time_for_test = 10;
static CORBA::ULong work = 10;
static CORBA::ULong max_throughput_timeout = 5;
static int individual_continuous_worker_stats = 0;
static int print_missed_invocations = 0;
static ACE_hrtime_t test_start;
static CORBA::ULong prime_number = 9619;
static int count_missed_end_deadlines = 0;
const char *agent_ior_file = "agent.ior";
std::vector <int> sample_vector (3100);
size_t count = 0;
const char *dummy_file_name = "temp1";

typedef ACE_Array_Base<CORBA::ULong> ULong_Array;

int
get_values (const char *test_type,
            const char *file_name,
            const char *name,
            ULong_Array &values,
            int debug)
{
  //
  // Read lanes from a file.
  //
  FILE* file =
    ACE_OS::fopen (file_name, "r");

  if (file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s\n",
                       file_name),
                      -1);

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty lanes file.
  if (string == 0)
    {
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    "\n%s: No %s set!\n\n",
                    test_type,
                    name));
      return 0;
    }

  size_t length =
    reader.replaced () + 1;

  values.size (length);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "\n%s: There are %d %s: ",
                test_type,
                length,
                name));

  int result = 1;
  char* working_string = string;
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // sscanf with "%ul" doesn't seem to work properly on HP-UX.  So,
      // we will use strtoul instead.
      char* endptr = 0;
      values[i] = ACE_OS::strtoul (working_string, &endptr, 10);

      if (endptr != working_string && endptr != 0 && *endptr != '\0')
	      {
	        result = 0;
          break;
	      }

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    "[%u] ",
                    values[i]));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Parsing error in file %s\n",
                       file_name),
                      -1);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "\n\n"));

  return 0;
}

struct Synchronizers
{
  Synchronizers (void)
    : worker_lock_ (),
      workers_ (1),
      workers_ready_ (0),
      number_of_workers_ (0)
  {
  }

  ACE_SYNCH_MUTEX worker_lock_;
  ACE_Event workers_;
  CORBA::ULong workers_ready_;
  CORBA::ULong number_of_workers_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        "c:e:g:hi:k:m:q:r:t:v:w:x:z:a:" //client options
                        "b:f:hl:n:o:s:" // server options
                        );
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        continuous_workers =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'e':
        count_missed_end_deadlines =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'g':
        do_dump_history =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        individual_continuous_worker_stats =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior =
          get_opts.opt_arg ();
        break;

      case 'm':
        print_missed_invocations =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'q':
        prime_number =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        rates_file =
          get_opts.opt_arg ();
        break;

      case 't':
        time_for_test =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        work =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        max_throughput_timeout =
          ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'a':
        history_file_name = get_opts.opt_arg();
        break;

      case 'b':
      case 'f':
      case 'l':
      case 'n':
      case 'o':
      case 's':
        // server options: ignored.
        break;

      case 'h':
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-c <number of continuous workers> (defaults to %d)\n"
                           "\t-e <count missed end deadlines> (defaults to %d)\n"
                           "\t-g <show history> (defaults to %d)\n"
                           "\t-h <help: shows options menu>\n"
                           "\t-i <print stats of individual continuous workers> (defaults to %d)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\t-m <print missed invocations for paced workers> (defaults to %d)\n"
                           "\t-q <prime number> (defaults to %d)\n"
                           "\t-r <rates file> (defaults to %s)\n"
                           "\t-t <time for test> (defaults to %d)\n"
                           "\t-w <work> (defaults to %d)\n"
                           "\t-x <shutdown server> (defaults to %d)\n"
                           "\t-z <timeout for max throughput measurement> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           continuous_workers,
                           count_missed_end_deadlines,
                           do_dump_history,
                           individual_continuous_worker_stats,
                           ior,
                           print_missed_invocations,
                           prime_number,
                           rates_file,
                           time_for_test,
                           work,
                           shutdown_server,
                           max_throughput_timeout),
                          -1);
      }

  return 0;
}

double
to_seconds (ACE_UINT64 hrtime,
            ACE_UINT32 sf)
{
  double seconds =
#if defined ACE_LACKS_LONGLONG_T
    hrtime / sf;
#else  /* ! ACE_LACKS_LONGLONG_T */
  static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER (hrtime / sf));
#endif /* ! ACE_LACKS_LONGLONG_T */
  seconds /= ACE_HR_SCALE_CONVERSION;

  return seconds;
}

ACE_UINT64
to_hrtime (double seconds,
           ACE_UINT32 sf)
{
  return ACE_UINT64 (seconds * sf * ACE_HR_SCALE_CONVERSION);
}

int
start_synchronization (test_ptr test,
                       Synchronizers &synchronizers)
{
  CORBA::ULong synchronization_iterations = 1;
  try
    {
      for (CORBA::ULong i = 0;
           i < synchronization_iterations;
           ++i)
        {
          test->method (0, 0, work,
                        prime_number, 0);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      synchronizers.worker_lock_,
                      -1);

    if (synchronizers.workers_ready_ == 0)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));
      }

    ++synchronizers.workers_ready_;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "%d worker ready\n",
                  synchronizers.workers_ready_));

    if (synchronizers.workers_ready_ ==
        synchronizers.number_of_workers_)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));

        test_start =
          ACE_OS::gethrtime ();

        synchronizers.workers_.signal ();

        return 0;
      }
  }

  synchronizers.workers_.wait ();

  return 0;
}

int
end_synchronization (Synchronizers &synchronizers)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      synchronizers.worker_lock_,
                      -1);

    if (synchronizers.workers_ready_ ==
        synchronizers.number_of_workers_)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));

        synchronizers.workers_.reset ();
      }

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "%d worker completed\n",
                  synchronizers.workers_ready_));

    --synchronizers.workers_ready_;

    if (synchronizers.workers_ready_ == 0)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));

        synchronizers.workers_.signal ();

        return 0;
      }
  }

  synchronizers.workers_.wait ();

  return 0;
}

int
max_throughput (test_ptr test,
                CORBA::ULong &max_rate)
{
  CORBA::ULong calls_made = 0;

  try
    {
      ACE_hrtime_t start =
        ACE_OS::gethrtime ();

      ACE_hrtime_t end =
        start +
        to_hrtime (max_throughput_timeout, gsf);

      for (;;)
        {
          ACE_hrtime_t now =
            ACE_OS::gethrtime ();

          if (now > end)
            break;

          test->method (0, 0, work,
                        prime_number, 0);

          ++calls_made;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  max_rate =
    calls_made / max_throughput_timeout;

  ACE_DEBUG ((LM_DEBUG,
              "\nMax rate calculations => %d calls in %d seconds; Max rate = %.2f\n",
              calls_made,
              max_throughput_timeout,
              calls_made / (double) max_throughput_timeout));

  return 0;
}

class Paced_Worker : public ACE_Task_Base
{
public:
  Paced_Worker (ACE_Thread_Manager &thread_manager,
                test_ptr test,
                CORBA::Short rate,
                CORBA::ULong iterations,
                Synchronizers &synchronizers);

  int svc (void);
  ACE_hrtime_t deadline_for_current_call (CORBA::ULong i);
  void print_stats (ACE_hrtime_t test_end);
  int setup (void);
  void missed_start_deadline (CORBA::ULong invocation);
  void missed_end_deadline (CORBA::ULong invocation);

  test_var test_;
  int rate_;
  ACE_Sample_History history_;
  Synchronizers &synchronizers_;
  ACE_hrtime_t interval_between_calls_;
  CORBA::ULong missed_start_deadlines_;
  CORBA::ULong missed_end_deadlines_;

  typedef ACE_Array_Base<CORBA::ULong> Missed_Invocations;
  Missed_Invocations missed_start_invocations_;
  Missed_Invocations missed_end_invocations_;
};

Paced_Worker::Paced_Worker (ACE_Thread_Manager &thread_manager,
                            test_ptr test,
                            CORBA::Short rate,
                            CORBA::ULong iterations,
                            Synchronizers &synchronizers)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    rate_ (rate),
    history_ (iterations),
    synchronizers_ (synchronizers),
    interval_between_calls_ (),
    missed_start_deadlines_ (0),
    missed_end_deadlines_ (0),
    missed_start_invocations_ (iterations),
    missed_end_invocations_ (iterations)
{
  this->interval_between_calls_ =
    to_hrtime (1 / double (this->rate_), gsf);
}

ACE_hrtime_t
Paced_Worker::deadline_for_current_call (CORBA::ULong i)
{
  ACE_hrtime_t deadline_for_current_call =
    this->interval_between_calls_;

  deadline_for_current_call *= i;

  deadline_for_current_call += test_start;

  return deadline_for_current_call;
}

void
Paced_Worker::print_stats (ACE_hrtime_t test_end)
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             mon,
             this->synchronizers_.worker_lock_);

  CORBA::ULong missed_total_deadlines =
    this->missed_start_deadlines_ + this->missed_end_deadlines_;

  CORBA::ULong made_total_deadlines =
    this->history_.max_samples () - missed_total_deadlines;

  ACE_DEBUG ((LM_DEBUG,
              "\n************ Statistics for thread %t ************\n\n"));

  if (count_missed_end_deadlines)
    ACE_DEBUG ((LM_DEBUG,
                "Deadlines made/missed[start,end]/%% = %d/%d[%d,%d]/%.2f%%; Effective Rate = %.2f\n",
                made_total_deadlines,
                missed_total_deadlines,
                this->missed_start_deadlines_,
                this->missed_end_deadlines_,
                made_total_deadlines * 100 / (double) this->history_.max_samples (),
                made_total_deadlines / to_seconds (test_end - test_start, gsf)));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Deadlines made/missed/%% = %d/%d/%.2f%%; Effective Rate = %.2f\n",
                made_total_deadlines,
                missed_total_deadlines,
                made_total_deadlines * 100 / (double) this->history_.max_samples (),
                made_total_deadlines / to_seconds (test_end - test_start, gsf)));


  if (do_dump_history)
    {
      //this->history_.dump_samples (history_file_name, gsf);
      std::ofstream output_file (history_file_name);
      std::copy (sample_vector.begin(), sample_vector.end(), 
                 std::ostream_iterator <int> (output_file, "\n"));
    }

  ACE_Basic_Stats stats;
  this->history_.collect_basic_stats (stats);
  stats.dump_results ("Total", gsf);

  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                         test_end - test_start,
                                         stats.samples_count ());

  if (print_missed_invocations)
    {
      ACE_DEBUG ((LM_DEBUG, "\nMissed start invocations are: "));

      for (CORBA::ULong j = 0;
           j != this->missed_start_deadlines_;
           ++j)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%d ",
                      this->missed_start_invocations_[j]));
        }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      if (count_missed_end_deadlines)
        {
          ACE_DEBUG ((LM_DEBUG, "\nMissed end invocations are: "));

          for (CORBA::ULong j = 0;
               j != this->missed_end_deadlines_;
               ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%d ",
                          this->missed_end_invocations_[j]));
            }

          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
    }
}

int
Paced_Worker::setup (void)
{
  return
    start_synchronization (this->test_.in (),
                           this->synchronizers_);
}

void
Paced_Worker::missed_start_deadline (CORBA::ULong invocation)
{
  this->missed_start_invocations_[this->missed_start_deadlines_++] =
    invocation;
}

void
Paced_Worker::missed_end_deadline (CORBA::ULong invocation)
{
  if (count_missed_end_deadlines)
    this->missed_end_invocations_[this->missed_end_deadlines_++] =
      invocation;
}

int
Paced_Worker::svc (void)
{
  try
    {
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor (); 
      //size_t terminate_count =  this->history_.max_samples () / 2;
      size_t terminate_count = 500;
      int result =
        this->setup ();

      if (result != 0)
        return result;

      CORBA::ULong testing_start = 0;
      CORBA::ULong testing_end = 0;
      CORBA::ULong end_iteration = this->history_.max_samples () - 1;

      for (CORBA::ULong i = 0;
           i != this->history_.max_samples ();
           ++i)
        {
          ACE_hrtime_t deadline_for_current_call =
            this->deadline_for_current_call (i);

          ACE_hrtime_t time_before_call =
            ACE_OS::gethrtime ();

          if (time_before_call > deadline_for_current_call)
            {
              this->missed_start_deadline (i + 1);
              continue;
            }

          if (i == 0)
            {
              testing_start = 1;
            }
          else if (i == end_iteration)
            {
              testing_end = 1;
            }

          if (i == terminate_count)
          {
            sample_vector[count++] = 0;
            this->test_->method (testing_start, testing_end, work,
                                 prime_number, 1);
          }
          else
          {
            this->test_->method (testing_start, testing_end, work,
                                 prime_number, 0);
          }

          ACE_hrtime_t time_after_call =
            ACE_OS::gethrtime ();
          this->history_.sample (time_after_call - time_before_call);
          sample_vector[count++] = 
            static_cast<int> ((time_after_call - time_before_call) / gsf);

          if (time_after_call > deadline_for_current_call)
            {
              this->missed_end_deadline (i + 1);
              continue;
            }

          ACE_hrtime_t sleep_time =
            deadline_for_current_call - time_after_call;

          ACE_OS::sleep (ACE_Time_Value (0,
                                         long (to_seconds (sleep_time, gsf) *
                                               ACE_ONE_SECOND_IN_USECS)));
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      done = 1;

      end_synchronization (this->synchronizers_);

      this->print_stats (test_end);

      ACE_DEBUG ((LM_DEBUG, "******************************\n"));

      this->test_->dump ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

class Continuous_Worker : public ACE_Task_Base
{
public:
  Continuous_Worker (ACE_Thread_Manager &thread_manager,
                     test_ptr test,
                     CORBA::ULong iterations,
                     Synchronizers &synchronizers);

  int svc (void);
  void print_stats (ACE_Sample_History &history,
                    ACE_hrtime_t test_end);
  int setup (void);
  void print_collective_stats (void);

  test_var test_;
  CORBA::ULong iterations_;
  Synchronizers &synchronizers_;
  ACE_Basic_Stats collective_stats_;
  ACE_hrtime_t time_for_test_;
};

Continuous_Worker::Continuous_Worker (ACE_Thread_Manager &thread_manager,
                                      test_ptr test,
                                      CORBA::ULong iterations,
                                      Synchronizers &synchronizers)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    iterations_ (iterations),
    synchronizers_ (synchronizers),
    collective_stats_ (),
    time_for_test_ (0)
{
}

void
Continuous_Worker::print_stats (ACE_Sample_History &history,
                                ACE_hrtime_t test_end)
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             mon,
             this->synchronizers_.worker_lock_);

  if (individual_continuous_worker_stats)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n************ Statistics for thread %t ************\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Iterations = %d\n",
                  history.sample_count ()));

      if (do_dump_history)
        {
          history.dump_samples (history_file_name, gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());
    }

  history.collect_basic_stats (this->collective_stats_);
  ACE_hrtime_t elapsed_time_for_current_thread =
    test_end - test_start;
  if (elapsed_time_for_current_thread > this->time_for_test_)
    this->time_for_test_ = elapsed_time_for_current_thread;
}

void
Continuous_Worker::print_collective_stats (void)
{
  if (continuous_workers > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n************ Statistics for continuous workers ************\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Collective iterations = %d; Workers = %d; Average = %d\n",
                  this->collective_stats_.samples_count (),
                  continuous_workers,
                  this->collective_stats_.samples_count () /
                  continuous_workers));

      this->collective_stats_.dump_results ("Collective", gsf);

      ACE_Throughput_Stats::dump_throughput ("Individual", gsf,
                                             this->time_for_test_,
                                             this->collective_stats_.samples_count () /
                                             continuous_workers);

      ACE_Throughput_Stats::dump_throughput ("Collective", gsf,
                                             this->time_for_test_,
                                             this->collective_stats_.samples_count ());
    }
}

int
Continuous_Worker::setup (void)
{
  return
    start_synchronization (this->test_.in (),
                           this->synchronizers_);
}

int
Continuous_Worker::svc (void)
{
  try
    {
      ACE_Sample_History history (this->iterations_);

      int result =
        this->setup ();

      if (result != 0)
        return result;

      for (CORBA::ULong i = 0;
           i != history.max_samples () && !done;
           ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->test_->method (0, 0, work,
                               prime_number, 0);

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          history.sample (end - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      end_synchronization (this->synchronizers_);

      this->print_stats (history,
                         test_end);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  Synchronizers synchronizers;

  gsf = ACE_High_Res_Timer::global_scale_factor ();

  try
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (ior);

      test_var test =
        test::_narrow (object.in ());

      ULong_Array rates;
      int result =
        get_values ("client",
                    rates_file,
                    "rates",
                    rates,
                    1);
                    
      if (result != 0)
        {
          return result;
        }

      synchronizers.number_of_workers_ =
        rates.size ();// + continuous_workers;

      CORBA::ULong max_rate = 0;
      result =
        max_throughput (test.in (),
                        max_rate);
      if (result != 0)
        return result;

      ACE_Thread_Manager paced_workers_manager;

      CORBA::ULong i = 0;
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
                              synchronizers);
        }

      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      for (i = 0; i < rates.size (); ++i)
        {
	        result =
	          paced_workers[i]->activate (flags);
      	  
	        if (result != 0)
	          {
	            ACE_ERROR_RETURN ((LM_ERROR,
			                           "Paced_Worker::activate failed\n"),
			                           result);
			      }
        }

      if (rates.size () != 0)
        {
          paced_workers_manager.wait ();
        }

      for (i = 0; i < rates.size (); ++i)
        {
          delete paced_workers[i];
        }
        
      delete[] paced_workers;

      if (shutdown_server)
        {
          test->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Task::svc - exception caught:");
      return -1;
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      ForwardingAgent_i *forwarding_agent = 0;
      ACE_NEW_RETURN (forwarding_agent,
                      ForwardingAgent_i,
                      1);
      PortableServer::ServantBase_var owner_transfer (forwarding_agent);

      // ******************************************************

      // ******************************************************
      // register request interceptor
      
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer (forwarding_agent),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
      
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      
      if (result != 0)
        {
          return result;
        }
        
      ACE_Barrier fa_barrier (2);
      ForwardingAgent_Thread fa_thread (orb.in (),
                                        forwarding_agent,
                                        fa_barrier);

      // Task activation flags.
      long flags =
        THR_NEW_LWP
        | THR_JOINABLE
        | orb->orb_core ()->orb_params ()->thread_creation_flags ();

      if (fa_thread.activate (flags, 1, 0, 0) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate forwarding agent thread\n"),
                            -1);
        }
        
      fa_barrier.wait ();

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager, orb.in ());

      // Activate task.
      result = task.activate (flags);
                       
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      // Wait for task to exit.
      result = thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client - exception caught:");
      return -1;
    }

  return 0;
}
