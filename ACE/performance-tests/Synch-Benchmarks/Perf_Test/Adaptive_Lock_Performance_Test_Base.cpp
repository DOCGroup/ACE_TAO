#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Adaptive_Lock_Performance_Test_Base.h"

#include <utility>

#if defined (ACE_HAS_THREADS)

std::unique_ptr<ACE_Lock> Adaptive_Lock_Performance_Test_Base::lock_;

int
Adaptive_Lock_Performance_Test_Base::fini ()
{
  Adaptive_Lock_Performance_Test_Base::lock_.reset ();
  return 0;
}

int
Adaptive_Lock_Performance_Test_Base::svc ()
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
Adaptive_Lock_Performance_Test_Base::set_lock (std::unique_ptr<ACE_Lock> lock)
{
  Adaptive_Lock_Performance_Test_Base::lock_ = std::move (lock);
  return Adaptive_Lock_Performance_Test_Base::lock_ ? 0 : -1;
}

#endif /* ACE_HAS_THREADS */
