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

  // Sumedh, please add comments here.
  this->ts_->barrier_->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Threads have bound, "
              "utilization test started\n"));

  this->run_computations ();
  return 0;
}

double
Util_Thread::get_number_of_computations (void)
{
  return this->number_of_computations_;
}

// Perform repeated prime factor computations on an arbitrary number
// and you thought your life was boring. :-)

int
Util_Thread::run_computations (void)
{
  // Sumedh, isn't there a "ACE::is_prime()" method that we can reuse
  // here?  If this doesn't work right, can you please add a new
  // method in class ACE so that we can leverage existing effort and
  // prepare for the future?

  while (this->done_ == 0)
    {
      u_long original = CUBIT_ARBIT_NUMBER;
      u_long n = original;
      u_long test_done = 1;
      u_long sqrt_n = (u_long) ceil (sqrt (n));
      u_long i;

      for (i = 2; i <= sqrt_n; i++)
        while ((n % i) == 0) 
          {
            n /= i;
            test_done *= i;
          }

      ACE_ASSERT (test_done * n == original);

      this->number_of_computations_ ++;
    }
  return 0;
}
