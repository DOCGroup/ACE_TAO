// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Context_Test : public Benchmark
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
      options.thr_work_count[ni]++;
    }

  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Context_Test)
ACE_SVC_FACTORY_DEFINE  (Context_Test)

// ACE_Service_Object_Type ct (&context_test, "Context_Test");
#endif /* ACE_HAS_THREADS */
