// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/SV_Semaphore_Simple.h"
#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export SYSVSema_Test : public Benchmark
{
public:
  virtual int svc (void);

private:
  static ACE_SV_Semaphore_Simple sema;
};

ACE_SV_Semaphore_Simple SYSVSema_Test::sema ((key_t) 1234);

int
SYSVSema_Test::svc (void)
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

  sema.remove ();
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (SYSVSema_Test)
ACE_SVC_FACTORY_DEFINE  (SYSVSema_Test)

// ACE_Service_Object_Type st (&sysvsema_test, "SYSVSema_Test");
#endif /* ACE_HAS_THREADS */
