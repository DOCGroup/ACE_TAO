// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"

static const char *ior = "file://ior";
static int shutdown_server = 0;
static int iterations = 5;
static int invocation_rate = 5;
static int count_missed_end_deadlines = 0;
static ACE_UINT32 gsf = 0;
static int do_dump_history = 0;
static int print_missed_invocations = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "d:e:i:k:m:r:x:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        do_dump_history = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'e':
        count_missed_end_deadlines = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'm':
        print_missed_invocations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        invocation_rate = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-d <show history> (defaults to %d)\n"
                           "\t-e <count missed end deadlines> (defaults to %d)\n"
                           "\t-h <help: shows options menu>\n"
                           "\t-i <iterations> (defaults to %d)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\t-m <print missed invocations for paced workers> (defaults to %d)\n"
                           "\t-r <invocation rate> (defaults to %d)\n"
                           "\t-x <shutdown server> (defaults to %d)\n"
                           "\n",
                           argv[0],
                           do_dump_history,
                           count_missed_end_deadlines,
                           iterations,
                           ior,
                           print_missed_invocations,
                           invocation_rate,
                           shutdown_server),
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
  ACE_static_cast (double,
                   ACE_UINT64_DBLCAST_ADAPTER (hrtime / sf));
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

class Paced_Worker
{
public:
  Paced_Worker (test_ptr test,
                CORBA::Short rate,
                CORBA::ULong iterations);

  void run (ACE_ENV_SINGLE_ARG_DECL);

private:

  ACE_hrtime_t deadline_for_current_call (CORBA::ULong i);
  void print_stats (void);
  void missed_start_deadline (CORBA::ULong invocation);
  void missed_end_deadline (CORBA::ULong invocation);

  test_var test_;
  int rate_;
  ACE_Sample_History history_;
  ACE_hrtime_t interval_between_calls_;
  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_end_;
  CORBA::ULong missed_start_deadlines_;
  CORBA::ULong missed_end_deadlines_;

  typedef ACE_Array_Base<CORBA::ULong> Missed_Invocations;
  Missed_Invocations missed_start_invocations_;
  Missed_Invocations missed_end_invocations_;
};

Paced_Worker::Paced_Worker (test_ptr test,
                            CORBA::Short rate,
                            CORBA::ULong iterations)
  : test_ (test::_duplicate (test)),
    rate_ (rate),
    history_ (iterations),
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

  deadline_for_current_call *= (i + 1);

  deadline_for_current_call += this->test_start_;

  return deadline_for_current_call;
}

void
Paced_Worker::print_stats (void)
{
  CORBA::ULong missed_total_deadlines =
    this->missed_start_deadlines_ + this->missed_end_deadlines_;

  CORBA::ULong made_total_deadlines =
    this->history_.max_samples () - missed_total_deadlines;

  ACE_DEBUG ((LM_DEBUG,
              "\n************ Statistics ************\n\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Rate = %d/sec; Iterations = %d; ",
              this->rate_,
              this->history_.max_samples ()));

  if (count_missed_end_deadlines)
    ACE_DEBUG ((LM_DEBUG,
                "Deadlines made/missed[start,end]/%% = %d/%d[%d,%d]/%.2f%%; Effective Rate = %.2f\n",
                made_total_deadlines,
                missed_total_deadlines,
                this->missed_start_deadlines_,
                this->missed_end_deadlines_,
                made_total_deadlines * 100 / (double) this->history_.max_samples (),
                made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Deadlines made/missed/%% = %d/%d/%.2f%%; Effective Rate = %.2f\n",
                made_total_deadlines,
                missed_total_deadlines,
                made_total_deadlines * 100 / (double) this->history_.max_samples (),
                made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));


  if (do_dump_history)
    {
      this->history_.dump_samples ("HISTORY", gsf);
    }

  ACE_Basic_Stats stats;
  this->history_.collect_basic_stats (stats);
  stats.dump_results ("Total", gsf);

  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                         this->test_end_ - this->test_start_,
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

void
Paced_Worker::run (ACE_ENV_SINGLE_ARG_DECL)
{
  // To get things going...
  this->test_->method (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->test_start_ =
    ACE_OS::gethrtime ();

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

      this->test_->method (ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_hrtime_t time_after_call =
        ACE_OS::gethrtime ();
      this->history_.sample (time_after_call - time_before_call);

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

  this->test_end_ = ACE_OS::gethrtime ();

  this->print_stats ();
}

int
main (int argc, char **argv)
{
  gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->string_to_object (ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Paced_Worker paced_worker (test,
                                 invocation_rate,
                                 iterations);
      paced_worker.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (shutdown_server)
        {
          test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error!");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
