// $Id$

#include "Util_Thread.h"

ACE_RCSID(MT_Cubit, Util_Thread, "$Id$")

Util_Thread::Util_Thread (Task_State *ts,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_MT (ACE_Task<ACE_NULL_SYNCH> (thr_mgr)),
    done_ (0),
    number_of_computations_ (0),
    ts_ (ts)
{
}

int
Util_Thread::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  // thr_getprio () on the current thread should never fail.
  ACE_OS::thr_getprio (thr_handle, prio);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Utilization Thread created with priority %d, "
              "waiting for threads to finish binding\n", prio));

  // this barrier synchronizes the utilization thread with
  // the client threads
  // i.e., the Util_thread should wait until all the
  // clients have finished binding, and only then
  // start measuring the utilization.
  this->ts_->barrier_->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) )))))))) "
              "utilization test STARTED at %D\n"));

  this->ts_->utilization_task_started_ = 1;

  this->ts_->timer_.start ();

  this->run_computations ();

  this->ts_->timer_.stop ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) (((((((( " 
	      "utilization test ENDED at %D\n"));

  return 0;
}

u_long
Util_Thread::get_number_of_computations (void)
{
  return this->number_of_computations_;
}

// computation performed by the Utilization thread.  We need this in a
// separate function to get it's execution time.
//inline
void
Util_Thread::computation (void)
{
  // This is the number that the Util_Thread uses to check for
  // primality.
  const u_long CUBIT_PRIME_NUMBER = 509UL;

  // See if this number is prime.  2 and CUBIT_PRIME_NUMBER / 2 are
  // the recommended values for min_factor and max_factor, as
  // explained in ACE.h (is_prime).
  ACE::is_prime (CUBIT_PRIME_NUMBER,
                 2UL,
                 CUBIT_PRIME_NUMBER / 2);
}

// Perform repeated prime factor computations on an arbitrary number.
// And you thought your life was boring... :-)
int
Util_Thread::run_computations (void)
{
  while (this->done_ == 0)
    {
      // bound the number of computations, since we can potentially
      // block the machine if this thread never leaves the loop. 
      if (this->number_of_computations_ > (ts_->loop_count_ * 1000)) // magic number
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "\t(%t) utilization test breaking loop so machine won't block.\n"));
	  break;
	}
      this->computation ();
      this->number_of_computations_ ++;
      //            ACE_OS::thr_yield (); // Shouldn't need this.  And I'm not sure
                            // if it really helps.

      if (ts_->utilization_task_started_ == 0)
	break;
    }

  return 0;
}
