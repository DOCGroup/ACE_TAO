#include "ace/Synch.h"
// $Id$

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class Memory_Test : public Benchmark
{
public:
  virtual int svc (void);
};

int
Memory_Test::svc (void)
{
  int ni = this->thr_id ();
  size_t length = options.msg_size ();
  char	 *from  = new char[length];
  char	 *to    = new char[length];

  synch_count = 1;

  while (!this->done ())
    {
      ACE_OS::memcpy (to, from, length);
      options.thr_work_count[ni]++;
    }
  /* NOTREACHED */
  return 0;
}

extern "C" ACE_Service_Object *memory_test (void);

ACE_Service_Object *memory_test (void)
{
  return new Memory_Test;
}

// ACE_Service_Object_Type mt (&memory_test, "Memory_Test");
#endif /* ACE_HAS_THREADS */
