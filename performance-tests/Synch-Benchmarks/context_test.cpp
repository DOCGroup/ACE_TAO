#include "ace/Synch.h"
// $Id$

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class Context_Test : public Benchmark
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

extern "C" ACE_Service_Object *context_test (void);

ACE_Service_Object *context_test (void)
{
  return new Context_Test;
}

// ACE_Service_Object_Type ct (&context_test, "Context_Test");
#endif /* ACE_HAS_THREADS */
