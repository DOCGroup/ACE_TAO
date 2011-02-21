// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Mutex_Test : public Benchmark_Performance
{
public:
  virtual int svc (void);

private:
  static ACE_Thread_Mutex mutex;
};

ACE_Thread_Mutex Mutex_Test::mutex;

int
Mutex_Test::svc (void)
{
  // Extract out the unique thread-specific value to be used as an
  // index...
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      mutex.acquire ();
      performance_test_options.thr_work_count[ni]++;
      buffer++;
      mutex.release ();
    }
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Mutex_Test)
ACE_SVC_FACTORY_DEFINE  (Mutex_Test)

// ACE_Service_Object_Type mut (&mutex_test, "Mutex_Test");
#endif /* ACE_HAS_THREADS */
