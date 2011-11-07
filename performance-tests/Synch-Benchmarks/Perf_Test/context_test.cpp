// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Context_Test : public Benchmark_Performance
{
public:
  virtual int svc (void);
};

int
Context_Test::svc (void)
{
  int ni = this->thr_id ();

  synch_count = 1;

  while (!this->done ())
    {
      ACE_Thread::yield ();
      performance_test_options.thr_work_count[ni]++;
    }

  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Context_Test)
ACE_SVC_FACTORY_DEFINE  (Context_Test)

// ACE_Service_Object_Type ct (&context_test, "Context_Test");
#endif /* ACE_HAS_THREADS */
