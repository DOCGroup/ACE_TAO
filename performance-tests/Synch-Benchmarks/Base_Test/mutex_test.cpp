// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Synch.h"
#include "Baseline_Test.h"

template<class LOCK>
class ACE_Svc_Export Baseline_Lock_Test : public Baseline_Test_Base
{
public:
  virtual int acquire ();
  virtual int release ();
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release ();
  virtual int test_try_lock ();
  // Real test methods.

private:
  LOCK lock_;
  //
};

template<class LOCK> int
Baseline_Lock_Test<LOCK>::acquire ()
{
  return this->lock_.acquire ();
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::release ()
{
  return this->lock_.release ();
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::test_acquire_release ()
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

template<class LOCK> int
Baseline_Lock_Test<LOCK>::test_try_lock ()
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

typedef Baseline_Lock_Test<ACE_Thread_Mutex> Baseline_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Mutex_Test)

typedef Baseline_Lock_Test<ACE_Recursive_Thread_Mutex> Baseline_Recursive_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Recursive_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Recursive_Mutex_Test)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Baseline_Lock_Test<ACE_Thread_Mutex>;
template class Baseline_Lock_Test<ACE_Recursive_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Baseline_Lock_Test<ACE_Thread_Mutex>
#pragma instantiate Baseline_Lock_Test<ACE_Recursive_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
