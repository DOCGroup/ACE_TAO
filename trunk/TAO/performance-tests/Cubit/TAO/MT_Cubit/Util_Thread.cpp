// $Id$

#include "Util_Thread.h"

Util_Thread::Util_Thread (Task_State *ts,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_MT (ACE_Task<ACE_MT_SYNCH> (thr_mgr)),
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

  // this barrier synchronizes the utilization thread with 
  // the client threads
  // i.e., the Util_thread should wait until all the 
  // clients have finished binding, and only then
  // start measuring the utilization. 
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
  while (this->done_ == 0)
    {
      ACE::is_prime (CUBIT_ARBIT_NUMBER,
                     2,
                     CUBIT_ARBIT_NUMBER / 2);
      this->number_of_computations_ ++;
    }

  return 0;
}
