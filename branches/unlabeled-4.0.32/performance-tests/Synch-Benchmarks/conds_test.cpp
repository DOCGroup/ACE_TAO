#include "ace/Synch.h"
// @(#)conds_test.cpp	1.1	10/18/96

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class Cond_Signal_Test : public Benchmark
{
public:
  virtual int svc (void);

private:
  static ACE_Thread_Mutex mutex;
  static int resources;

  static ACE_Condition_Thread_Mutex notfull;
  static ACE_Condition_Thread_Mutex notempty;
};

ACE_Thread_Mutex Cond_Signal_Test::mutex;
int Cond_Signal_Test::resources;
ACE_Condition_Thread_Mutex Cond_Signal_Test::notfull (Cond_Signal_Test::mutex);
ACE_Condition_Thread_Mutex Cond_Signal_Test::notempty (Cond_Signal_Test::mutex);

int
Cond_Signal_Test::svc (void)
{
  int ni = this->thr_id ();
  synch_count = 2;

  // This is a horrible hack and only works for Solaris threads.  This
  // clearly needs to change...
  if (ni == 4)
    while (!this->done ())
      {
	mutex.acquire ();

	while (resources > 0)
	  notfull.wait ();

	options.thr_work_count[ni]++;
	resources = options.thr_count () - 1;
	buffer++;
	notempty.signal ();
	mutex.release ();
      }
  else
    while (!this->done ())
      {
	mutex.acquire ();
	while (resources == 0)
	  notempty.wait ();
	options.thr_work_count[ni]++;
	buffer++;
	if (--resources == 0)
	  notfull.signal ();
	mutex.release ();
      }

  /* NOTREACHED */
  return 0;
}

extern "C" ACE_Service_Object *cond_signal_test (void);

ACE_Service_Object *cond_signal_test (void)
{
  return new Cond_Signal_Test;
}

// ACE_Service_Object_Type cst (&cond_signal_test, "Condition_Signal_Test");
#endif /* ACE_HAS_THREADS */
