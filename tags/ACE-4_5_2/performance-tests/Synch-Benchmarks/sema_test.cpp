// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Sema_Test : public Benchmark
{
public:
  virtual int svc (void);

private:
  static ACE_Semaphore sema;
};

ACE_Semaphore Sema_Test::sema (1);

int
Sema_Test::svc (void)
{
  int ni      = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      sema.acquire ();
      options.thr_work_count[ni]++;
      buffer++;
      sema.release ();
    }

  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Sema_Test)
ACE_SVC_FACTORY_DEFINE  (Sema_Test)

// ACE_Service_Object_Type semt (&sema_test, "Semaphore_Test");
#endif /* ACE_HAS_THREADS */
