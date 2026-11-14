#define  ACE_BUILD_SVC_DLL
#include "ace/RW_Mutex.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export RWWR_Test : public Benchmark_Performance
{
public:
  RWWR_Test () = default;
  RWWR_Test (RWWR_Test const &) = delete;
  RWWR_Test (RWWR_Test &&) = delete;
  RWWR_Test &operator= (RWWR_Test const &) = delete;
  RWWR_Test &operator= (RWWR_Test &&) = delete;
  int svc () override;

private:
  static ACE_RW_Mutex rw_lock;
};

ACE_RW_Mutex RWWR_Test::rw_lock;

int
RWWR_Test::svc ()
{
  int const ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      rw_lock.acquire_write ();
      ++performance_test_options.thr_work_count[ni];
      ++buffer;
      rw_lock.release ();
    }

  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (RWWR_Test)
ACE_SVC_FACTORY_DEFINE  (RWWR_Test)

// ACE_Service_Object_Type rwwrt (&rwwr_test, "RWWR_Mutext_Test");
#endif /* ACE_HAS_THREADS */
