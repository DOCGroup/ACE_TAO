// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Synch.h"
#include "Baseline_Test.h"

class ACE_Svc_Export Baseline_Mutex_Test : public Baseline_Test_Base
{
public:
  virtual int acquire ();
  virtual int release ();
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release ();
  virtual int test_try_lock ();
  // Real test methods.

private:
  ACE_Thread_Mutex lock_;
  //
};
ACE_SVC_FACTORY_DECLARE (Baseline_Mutex_Test)

int
Baseline_Mutex_Test::acquire ()
{
  return this->lock_.acquire ();
}

int
Baseline_Mutex_Test::release ()
{
  return this->lock_.release ();
}

int
Baseline_Mutex_Test::test_acquire_release ()
{
  ACE_Profile_Timer ptimer;
  ACE_Profile_Timer::ACE_Elapsed_Time et;

  for (; baseline_options.inc_loop_counter () ; )
    {
      this->yield ();
      ptimer.start ();

      for (size_t i=0; i < baseline_options.current_multiply_factor (); i++)
        {
          this->lock_.acquire ();
          this->lock_.release ();
        }

      ptimer.stop ();
      ptimer.elapsed_time (et);
      baseline_options.add_time (et);
    }
  return 0;
}

int
Baseline_Mutex_Test::test_try_lock ()
{
  ACE_Profile_Timer ptimer;
  ACE_Profile_Timer::ACE_Elapsed_Time et;

  for (; baseline_options.inc_loop_counter () ; )
    {
      this->yield ();
      ptimer.start ();

      for (size_t i=0; i < baseline_options.current_multiply_factor (); i++)
          this->lock_.tryacquire (); // This should always fail.

      ptimer.stop ();
      ptimer.elapsed_time (et);
      baseline_options.add_time (et);
    }
  return 0;
}

ACE_SVC_FACTORY_DEFINE (Baseline_Mutex_Test)
