#include "ace/Synch.h"
// @(#)rwrd_test.cpp	1.1	10/18/96

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class RWRD_Test : public Benchmark
{
public:
  virtual int svc (void);

private:
  static ACE_RW_Mutex rw_lock;
};

ACE_RW_Mutex RWRD_Test::rw_lock;

int
RWRD_Test::svc (void)
{
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      rw_lock.acquire_read ();
      options.thr_work_count[ni]++;
      buffer++;
      rw_lock.release ();
    }

  /* NOTREACHED */
  return 0;
}

extern "C" ACE_Service_Object *rwrd_test (void);

ACE_Service_Object *rwrd_test (void)
{
  return new RWRD_Test;
}

// ACE_Service_Object_Type rwrdt (&rwrd_test, "RWRD_Mutex_Test");
#endif /* ACE_HAS_THREADS */
