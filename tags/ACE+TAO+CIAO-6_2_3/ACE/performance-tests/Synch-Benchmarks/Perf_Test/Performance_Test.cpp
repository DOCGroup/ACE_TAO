// $Id$

#define  ACE_BUILD_SVC_DLL

#include "Performance_Test.h"

# if defined (ACE_HAS_THREADS)

#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#include "ace/Service_Repository.h"
#include "ace/Reactor.h"

Performance_Test::Performance_Test (void)
  : n_lwps_ (0),
    orig_n_lwps_ (0)
{
}

// Initialize and run the benchmarks tests.

int
Performance_Test::init (int argc, ACE_TCHAR **argv)
{
  ACE_DEBUG ((LM_DEBUG, "Performance_Test::init\n"));
  performance_test_options.parse_args (argc, argv);
  return 0;
}

int
Performance_Test::pre_run_test (Benchmark_Base *bb)
{
  this->orig_n_lwps_ = ACE_Thread::getconcurrency ();
  this->n_lwps_ = performance_test_options.n_lwps ();
  Benchmark_Performance *bp = (Benchmark_Performance *) bb;

  if (this->n_lwps_ > 0)
    ACE_Thread::setconcurrency (this->n_lwps_);

      // We should probably use a "barrier" here rather than
      // THR_SUSPENDED since many OS platforms lack the ability to
      // create suspended threads...
  if (ACE_Thread_Manager::instance ()->spawn_n
      (performance_test_options.thr_count (), ACE_THR_FUNC (bp->svc_run),
       (void *) bp, performance_test_options.t_flags () | THR_SUSPENDED) == -1)
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

  ACE_Time_Value timeout (performance_test_options.sleep_time ());

  ACE_DEBUG ((LM_DEBUG, "starting timer\n"));
  performance_test_options.start_timer ();

  // Use Reactor as a timer (which can be interrupted by a signal).
  ACE_Reactor::run_event_loop (timeout);

  performance_test_options.stop_timer ();
  ACE_DEBUG ((LM_DEBUG, "\nstopping timer\n"));

  return 0;
}

int
Performance_Test::post_run_test (void)
{
  // Stop thread(s) from making any further progress.
  ACE_Thread_Manager::instance ()->suspend_all ();

  // Tell the threads that we are finished.
  Benchmark_Performance::done (1);

  ACE_DEBUG ((LM_DEBUG, "------------------------------------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "targ 0x%x (%s, %s, %s)\n"
              "n_lwps_orig = %d, n_lwps_set = %d, n_lwps_end = %d\n",
              performance_test_options.t_flags (),
              (performance_test_options.t_flags () & THR_DETACHED) ? "THR_DETACHED" : "Not Detached",
              (performance_test_options.t_flags () & THR_BOUND) ? "THR_BOUND"    : "Not Bound",
              (performance_test_options.t_flags () & THR_NEW_LWP)  ? "THR_NEW_LWP"  : "No New_LWP",
              this->orig_n_lwps_, this->n_lwps_, ACE_Thread::getconcurrency ()));
  int count = performance_test_options.count ();
  float rate  = count / (float (performance_test_options.sleep_time ()));

  ACE_DEBUG ((LM_DEBUG,
              "to count = %d\nrate = %.3f ops/sec, per operation = %.2f usec\n",
              count,
              rate,
              (1.0e6 / rate) / synch_count));
  performance_test_options.print_results ();
  // Allow thread(s) to finish up.
  ACE_Thread_Manager::instance ()->resume_all ();

  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  performance_test_options.init ();

  return 0;
}

int
Performance_Test::valid_test_object (Benchmark_Base *bb)
{
  return (bb->benchmark_type () == Benchmark_Base::PERFORMANCE);
}

ACE_SVC_FACTORY_DEFINE (Performance_Test)

#endif  /* ACE_HAS_THREADS */
