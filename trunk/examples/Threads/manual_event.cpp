// The test shows the use of an ACE_Manual_Event to create a
// $Id$

// Pseudo_Barrier. Multiple threads are created which do the
// following:
//
//	1. work 
//	2. synch with other threads
//	3. more work
// 
// ACE_Manual_Event is use to synch with other
// threads. ACE_Manual_Event::signal() is used for broadcasting.

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)
static ACE_Atomic_Op <ACE_Thread_Mutex, int> amount_of_work = (u_long) 0;

class Pseudo_Barrier
  // = TITLE
  //    A barrier class using ACE manual-reset events.
  //
  // = DESCRIPTION
  //    This is *not* a real barrier. 
  //    Pseudo_Barrier is more like a ``one shot'' barrier.
  //    All waiters after the Nth waiter are allowed to go. 
  //    The barrier does not reset after the Nth waiter.
  //    For an example of a real barrier, please see class ACE_Barrier.
{
public:
  Pseudo_Barrier (u_long count);

  int wait (void);

private: 
  ACE_Atomic_Op <ACE_Thread_Mutex, int> counter_; 
  ACE_Manual_Event event_;
};

Pseudo_Barrier::Pseudo_Barrier (u_long count)
  : counter_ (count)
{
}

int 
Pseudo_Barrier::wait (void)
{
  if (--this->counter_ == 0)
    return this->event_.signal ();
  else 
    return this->event_.wait ();
}

static void *
worker (void *arg)
{
  Pseudo_Barrier &barrier = *(Pseudo_Barrier *) arg;
  
  // work
  ACE_DEBUG ((LM_DEBUG, "(%t) working (%d secs)\n", ++::amount_of_work));
  ACE_OS::sleep (::amount_of_work);

  // synch with everybody else
  ACE_DEBUG ((LM_DEBUG, "(%t) waiting to synch with others \n"));
  barrier.wait ();

  // more work
  ACE_DEBUG ((LM_DEBUG, "(%t) more work (%d secs)\n", ++::amount_of_work));
  ACE_OS::sleep (amount_of_work);

  ACE_DEBUG ((LM_DEBUG, "(%t) dying \n"));

  return 0;
}

int 
main (int argc, char **argv)
{
  int n_threads = argc == 2 ? atoi (argv[1]) : 5;

  ACE_Thread_Manager &tm = *ACE_Thread_Manager::instance ();
  
  // synch object shared by all threads
  Pseudo_Barrier barrier (n_threads);

  // create workers
  if (tm.spawn_n (n_threads, (ACE_THR_FUNC) worker, &barrier) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "thread creates for worker failed"), -1);

  // wait for all workers to exit
  if (tm.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "thread wait failed"), -1);
  else 
    ACE_DEBUG ((LM_ERROR, "graceful exit\n"));

  return 0;
}
#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
