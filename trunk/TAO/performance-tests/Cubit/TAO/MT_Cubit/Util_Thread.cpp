// $Id$

#include "Util_Thread.h"

Util_Thread::Util_Thread (Task_State *ts,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    done_ (0),
    number_of_computations_ (0),
    ts_ (ts)
{
}

int
Util_Thread::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Utilization Thread created, "
              "waiting for threads to finish binding\n"));
  ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Threads have bound, "
              "utilization test started\n"));
  run_computations ();

  return 0;
}

double
Util_Thread::get_number_of_computations ()
{
  return number_of_computations_;
}

// perform repeated prime factor computations on an arbitrary number
// and you thought your life was boring. :-)
int
Util_Thread::run_computations ()
{

  while (done_ == 0)
    {
      unsigned long original = CUBIT_ARBIT_NUMBER;
      unsigned long n = original;
      unsigned long test_done = 1;
      unsigned long sqrt_n = (unsigned long) ceil (sqrt (n));
      unsigned long i;

      for (i = 2; i <= sqrt_n; i++)
        while ( (n % i) == 0) {
          n /= i;
          test_done *= i;
        }
      assert (test_done * n == original);
      number_of_computations_ ++;
    }
  return 0;
}
