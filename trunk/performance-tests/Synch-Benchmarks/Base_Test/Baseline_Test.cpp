// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Service_Repository.h"
#include "ace/Synch.h"

#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"
#include "Performance_Test.h"

ACE_RCSID(Synch_Benchmarks, Baseline_Test, "$Id$")

Benchmark_Baseline_Test_Base::Benchmark_Baseline_Test_Base (void)
  : Benchmark_Base (Benchmark_Base::BASELINE)
{
}

Baseline_Test::Baseline_Test (void)
  : n_lwps_ (0),
    orig_n_lwps_ (0)
{
}

// Initialize and run the benchmarks tests.

int
Performance_Test::init (int argc, char **argv)
{
  options.parse_args (argc, argv);
  return 0;
}

int
Performance_Test::pre_run_test (Benchmark_Base *bb)
{
  this->orig_n_lwps_ = ACE_Thread::getconcurrency ();
  this->n_lwps_ = options.n_lwps ();
  Benchmark_Performance *bp = (Benchmark_Performance *) bb;

  if (this->n_lwps_ > 0)
    ACE_Thread::setconcurrency (this->n_lwps_);

      // We should probably use a "barrier" here rather than
      // THR_SUSPENDED since many OS platforms lack the ability to
      // create suspended threads...
  if (ACE_Thread_Manager::instance ()->spawn_n
      (options.thr_count (), ACE_THR_FUNC (bp->svc_run),
       (void *) bp, options.t_flags () | THR_SUSPENDED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "couldn't spawn threads", 1));
  return 0;
}

int
Performance_Test::run_test (void)
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

  ACE_DEBUG ((LM_DEBUG, "------------------------------------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "targ 0x%x (%s, %s, %s)\n"
	     "n_lwps_orig = %d, n_lwps_set = %d, n_lwps_end = %d\n",
	     options.t_flags (),
	     (options.t_flags () & THR_DETACHED) ? "THR_DETACHED" : "Not Detached",
	     (options.t_flags () & THR_BOUND)	? "THR_BOUND"    : "Not Bound",
	     (options.t_flags () & THR_NEW_LWP)  ? "THR_NEW_LWP"  : "No New_LWP",
	     this->orig_n_lwps_, this->n_lwps_, ACE_Thread::getconcurrency ()));

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
Performance_Test::post_run_test (void)
{
  return 0;
}

int
Performance_Test::valid_test_object (Benchmark_Base *bb)
{
  return (bb->benchmark_type () == Benchmark_Base::PERFORMANCE);
}

ACE_SVC_FACTORY_DEFINE (Performance_Test)
