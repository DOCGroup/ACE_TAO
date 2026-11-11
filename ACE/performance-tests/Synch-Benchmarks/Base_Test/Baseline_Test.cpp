#define  ACE_BUILD_SVC_DLL

#include "Baseline_Test.h"

# if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_unistd.h"
#include "ace/Service_Repository.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"

#if !defined (__ACE_INLINE__)
#include "Baseline_Test.inl"
#endif /* __ACE_INLINE__ */

Baseline_Test_Options baseline_options;
// Static Baseline Options holds the test configuration information
// and the test statistics.

Baseline_Test_Base::Baseline_Test_Base ()
  : Benchmark_Base (Benchmark_Base::BASELINE)
{
}

int
Baseline_Test_Base::init (int argc, ACE_TCHAR *argv[])
{
  return this->parse_args (argc, argv);
}

int
Baseline_Test_Base::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:ylrw"), 0);
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':                 // Total iterations
        {
          int const tmp = ACE_OS::atoi (get_opt.opt_arg ());
          if (tmp <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d is not a valid value for iteration\n",
                               tmp), -1);
          else
            this->iteration_ = static_cast<std::size_t> (tmp);
        }
        break;

      case 'y':                 // Use thr_yield.
        this->yield_method_ = Baseline_Yield_Method::thread_yield;
        break;

      case 'l':
        this->what_ = Lock_Type::lock;
        break;

      case 'r':
        this->what_ = Lock_Type::read_lock;
        break;

      case 'w':
        this->what_ = Lock_Type::write_lock;
        break;

      default:
        ACE_ERROR ((LM_ERROR, "Invalid argument %c used\n", c));
        break;
      }
  return 0;
}

void
Baseline_Test_Base::yield () const
{
  if (this->yield_method_ == Baseline_Yield_Method::sleep_zero)
    ACE_OS::sleep (0);
  else
    ACE_OS::thr_yield ();
}

int
Baseline_Test_Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("tv"), 0);
  int c;

  while ((c = getopt ()) != -1)
  //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
      {
      case 't':
        this->test_try_lock_ = true;
        break;

      case 'v':
        this->verbose_ = true;
        break;

      default:
        ACE_ERROR ((LM_ERROR, "Invalid arguemnt %c used.\n", c));
        break;
      }
  return 0;
}

void
Baseline_Test_Options::reset_params (std::size_t iteration,
                                     Baseline_Yield_Method yield)
{
  this->current_iteration_ = 0;
  this->timer.reset ();

  this->current_yield_method_ = yield;
  this->total_iteration_ = iteration;
}

void
Baseline_Test_Options::print_result ()
{
  ACE_Time_Value tv {};
  ACE_hrtime_t nsec {};

  this->timer.elapsed_time_incr (tv);
  this->timer.elapsed_time_incr (nsec);
  ACE_DEBUG ((LM_DEBUG,
              "Total Time: %d sec %d usec for a "
              "total of %B iterations\n"
              "Average time: %d nanoseconds.\n",
              tv.sec (), tv.usec (),
              this->current_iteration_,
              (int) (nsec / this->current_iteration_)));
}

Baseline_Test::Baseline_Test ()
  : get_lock_ (2),
    let_go_lock_ (2)
{
}

// Initialize and run the benchmarks tests.

int
Baseline_Test::init (int argc, ACE_TCHAR **argv)
{
  return baseline_options.parse_args (argc, argv);
}

int
Baseline_Test::pre_run_test (Benchmark_Base *bb)
{
  this->current_test_ = static_cast<Baseline_Test_Base *> (bb);
  baseline_options.reset_params (this->current_test_->iteration (),
                                 this->current_test_->yield_method ());
  if (baseline_options.test_try_lock ())
    {
      ACE_Thread_Manager::instance ()->spawn
        (&Baseline_Test::hold_lock, this);

      this->get_lock_.wait ();
      // Wait until the lock is held by the spawning thread.
    }

  return 0;
}

int
Baseline_Test::run_test ()
{
  if (baseline_options.test_try_lock ())
    return this->current_test_->test_try_lock ();
  else
    return this->current_test_->test_acquire_release ();
}

int
Baseline_Test::post_run_test ()
{
  if (baseline_options.test_try_lock ())
    {
      // Release the lock we hold.
      this->let_go_lock_.wait ();

      ACE_Thread_Manager::instance ()->wait ();
    }

  baseline_options.print_result ();

  return 0;
}

int
Baseline_Test::valid_test_object (Benchmark_Base *bb)
{
  return (bb->benchmark_type () == Benchmark_Base::BASELINE);
}

ACE_THR_FUNC_RETURN
Baseline_Test::hold_lock (void *arg)
{
  Baseline_Test * const this_test = static_cast<Baseline_Test *> (arg);
  this_test->current_test_->acquire ();
  this_test->get_lock_.wait ();

  this_test->let_go_lock_.wait ();
  return 0;
}

ACE_SVC_FACTORY_DEFINE (Baseline_Test)

#endif  /* ACE_HAS_THREADS */
