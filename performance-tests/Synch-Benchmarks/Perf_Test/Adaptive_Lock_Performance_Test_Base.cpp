// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Adaptive_Lock_Performance_Test_Base.h"

#if defined (ACE_HAS_THREADS)

ACE_Lock *Adaptive_Lock_Performance_Test_Base::lock_ = 0;

int
Adaptive_Lock_Performance_Test_Base::fini (void)
{
  delete Adaptive_Lock_Performance_Test_Base::lock_;
  Adaptive_Lock_Performance_Test_Base::lock_ = 0;
  return 0;
}

int
Adaptive_Lock_Performance_Test_Base::svc (void)
{
  // Extract out the unique thread-specific value to be used as an
  // index...
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      Adaptive_Lock_Performance_Test_Base::lock_->acquire ();
      performance_test_options.thr_work_count[ni]++;
      buffer++;
      Adaptive_Lock_Performance_Test_Base::lock_->release ();
    }
  /* NOTREACHED */
  return 0;
}

int
Adaptive_Lock_Performance_Test_Base::set_lock (ACE_Lock *lock)
{
  delete Adaptive_Lock_Performance_Test_Base::lock_;
  Adaptive_Lock_Performance_Test_Base::lock_ = lock;
  return (Adaptive_Lock_Performance_Test_Base::lock_ != 0 ? 0 : -1);
}

#endif /* ACE_HAS_THREADS */
