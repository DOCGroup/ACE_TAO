// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "ace/Get_Opt.h"

#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"
#include "Performance_Test.h"

#if !defined (__ACE_INLINE__)
#include "ace/Baseline_Test.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Synch_Benchmarks, Benchmark_Baseline, "$Id$")

Baseline_Options baseline_options;
// Static Baseline Options holds the test configuration information
// and the test statistics.

Benchmark_Baseline_Test_Base::Benchmark_Baseline_Test_Base (void)
  : Benchmark_Base (Benchmark_Base::BASELINE)
{
}

int
Benchmark_Baseline_Test_Base::init (int argc, char *argv[])
{
  return baseline_options.parse_test_args (argc, argv);
}

Baseline_Options::Baseline_Options (void)
  : test_try_lock_ (0),
    multiply_factor_ (10),
    iteration_ (10000)
{
}

int
Baseline_Options::parse_method_args (int argc, char *argv[])
{
  ACE_Get_Opt getopt (argc, argv, "t");

  while ((c = getopt ()) != -1)
    switch (c)
      {
      case 't':
        this->test_try_lock_ = 1;
        break;

      default:
        ACE_ERROR ((LM_ERROR, "Invalid arguemnt %c used.\n", c));
        break;
      }
  return 0;
}

int
Baseline_Options::parse_test_args (int argc, char *argv[])
{
  this->total_iteration_ = 0;
  this->real_ = 0;
  this->system_ = 0;
  this->user_ = 0;
  // Start a new test, reset statistic info.

  ACE_Get_Opt getopt (argc, argv, "m:i:");

  while ((c = getopt ()) != -1)
    switch (c)
      {
      case 'm':
        {
          int tmp = ACE_OS::atoi (getopt.optarg);
          if (tmp <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d is not a valid value for multiply_factor\n",
                               tmp), -1);
          else
            this->multiply_factor_ = ACE_static_cast (size_t, tmp);
        }
        break;

      case 'i':
        {
          int tmp = ACE_OS::atoi (getopt.optarg);
          if (tmp <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d is not a valid value for iteration\n",
                               tmp), -1);
          else
            this->iteration_ = ACE_static_cast (size_t, tmp);
        }
        break;

      default:
        ACE_ERROR ((LM_ERROR, "Invalid argument %c used\n", c));
        break;
      }
  return 0;
}

Benchmark_Baseline::Benchmark_Baseline (void)
  : get_lock_ (2),
    let_go_lock_ (2)
{
}

// Initialize and run the benchmarks tests.

int
Benchmark_Baseline::init (int argc, char **argv)
{
  return baseline_options.parse_method_args (argc, argv);
}

int
Benchmark_Baseline::pre_run_test (Benchmark_Base *bb)
{
  Benchmark_Baseline_Test_Base *bp = (Benchmark_Baseline_Test_Base *) bb;

  if (baseline_options.test_try_lock ())
    {
      // @@ spawn a thread and acquire the lock.
      ACE_Thread_Manager::instance ()->
        spawn (ACE_static_cast (ACE_THR_FUNC, this->svc_run));

      this->get_lock_.wait ();
      // Wait until the lock is held by the spawning thread.
    }
  return 0;
}

int
Benchmark_Baseline::run_test (void)
{
  // Tell the threads that we are not finished.
  Benchmark_Performance::done (0);

  // Allow thread(s) to make progress.
  ACE_Thread_Manager::instance ()->resume_all ();

  ACE_Time_Value timeout (options.sleep_time ());

  ACE_DEBUG ((LM_DEBUG, "starting timer\n"));
  options.start_timer ();

  // Use Reactor as a timer (which can be interrupted by a signal).
  ACE_Reactor::run_event_loop (timeout);

  options.stop_timer ();
  ACE_DEBUG ((LM_DEBUG, "\nstopping timer\n"));

  // Stop thread(s) from making any further progress.
  ACE_Thread_Manager::instance ()->suspend_all ();

  // Tell the threads that we are finished.
  Benchmark_Performance::done (1);

  int count = options.count ();
  float rate  = count / (float (options.sleep_time ()));

  ACE_DEBUG ((LM_DEBUG,
              "to count = %d\nrate = %.3f ops/sec, per operation = %.2f usec\n",
              count,
              rate,
              (1.0e6 / rate) / synch_count));
  options.print_results ();

  // Allow thread(s) to finish up.
  ACE_Thread_Manager::instance ()->resume_all ();

  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  options.init ();
  return 0;
}

int
Benchmark_Baseline::post_run_test (void)
{
  if (baseline_options.test_try_lock ())
    // Release the lock we hold.
    this->let_go_lock_.wait ();

  return 0;
}

int
Benchmark_Baseline::valid_test_object (Benchmark_Base *bb)
{
  return (bb->benchmark_type () == Benchmark_Base::BASELINE);
}

ACE_SVC_FACTORY_DEFINE (Performance_Test)
