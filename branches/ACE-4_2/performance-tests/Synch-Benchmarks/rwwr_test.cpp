#include "ace/Synch.h"
// $Id$

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class RWWR_Test : public Benchmark
{
public:
  virtual int svc (void);

private:
  static ACE_RW_Mutex rw_lock;
};

ACE_RW_Mutex RWWR_Test::rw_lock;

int
RWWR_Test::svc (void)
{
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      rw_lock.acquire_write ();
      options.thr_work_count[ni]++;
      buffer++;
      rw_lock.release ();
    }

  /* NOTREACHED */
  return 0;
}

extern "C" ACE_Service_Object *rwwr_test (void);

ACE_Service_Object *rwwr_test (void)
{
  return new RWWR_Test;
}

// ACE_Service_Object_Type rwwrt (&rwwr_test, "RWWR_Mutext_Test");
#endif /* ACE_HAS_THREADS */
