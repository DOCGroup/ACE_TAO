// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "Baseline_Test.h"

#if !defined (__ACE_INLINE__)
#include "Baseline_Test.i"
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
  char c;

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
  char c;

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
  : current_test_ (0),
    get_lock_ (2),
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
  this->current_test_ = (Benchmark_Baseline_Test_Base *) bb;

  if (baseline_options.test_try_lock ())
    {
      ACE_Thread_Manager::instance ()->spawn
        (ACE_THR_FUNC (Benchmark_Baseline::hold_lock),
         (void *) this);

      this->get_lock_.wait ();
      // Wait until the lock is held by the spawning thread.
    }
  return 0;
}

int
Benchmark_Baseline::run_test (void)
{
  return -1;
}

int
Benchmark_Baseline::post_run_test (void)
{
  if (baseline_options.test_try_lock ())
    // Release the lock we hold.
    this->let_go_lock_.wait ();

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

int
Benchmark_Baseline::valid_test_object (Benchmark_Base *bb)
{
  return (bb->benchmark_type () == Benchmark_Base::BASELINE);
}

void *
Benchmark_Baseline::hold_lock (void *arg)
{
  Benchmark_Baseline *this_test = (Benchmark_Baseline *) arg;

  ACE_UNUSED_ARG (this_test);
  return 0;
}

ACE_SVC_FACTORY_DEFINE (Benchmark_Baseline)
