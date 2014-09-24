// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Cond_Brdcast_Test : public Benchmark_Performance
{
public:
  virtual int svc (void);

private:
  static ACE_Thread_Mutex mutex;
  static int resources;

  static ACE_Condition_Thread_Mutex notfull;
  static ACE_Condition_Thread_Mutex notempty;
};

ACE_Thread_Mutex Cond_Brdcast_Test::mutex;
int Cond_Brdcast_Test::resources;
ACE_Condition_Thread_Mutex Cond_Brdcast_Test::notfull (Cond_Brdcast_Test::mutex);
ACE_Condition_Thread_Mutex Cond_Brdcast_Test::notempty (Cond_Brdcast_Test::mutex);

int
Cond_Brdcast_Test::svc (void)
{
  int ni = this->thr_id ();
  synch_count = 2;

  // Special case for first thread...
  if (ni == 4)
    while (!this->done ())
      {
        mutex.acquire ();
        while (resources > 0)
        notfull.wait ();
        performance_test_options.thr_work_count[ni]++;
        resources = performance_test_options.thr_count () - 1;
        buffer++;
        notempty.broadcast ();
        mutex.release ();
      }
  else
    while (!this->done ())
      {
        mutex.acquire ();
        while (resources == 0)
        notempty.wait ();
        performance_test_options.thr_work_count[ni]++;
        buffer++;
        if (--resources == 0)
        notfull.signal ();
        mutex.release ();
      }

  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Cond_Brdcast_Test)
ACE_SVC_FACTORY_DEFINE  (Cond_Brdcast_Test)

// ACE_Service_Object_Type cbt (&cond_brdcast_test, "Condition_Broadcast_Test");
#endif /* ACE_HAS_THREADS */
