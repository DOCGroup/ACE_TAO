// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Priority_Task_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate the priority mechanism of
//      ACE Tasks.  
//
// = AUTHOR
//    Doug Schmidt
// 
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "ace/Thread_Priority.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

class Priority_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Priority_Task (void);

  int close (u_long = 0);
  int open (void *);
  int svc (void);

private:
  int priority_;
};

Priority_Task::Priority_Task (void)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Service_Config::thr_mgr ()),
    priority_ (0)
{
}

int
Priority_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) leaving Task with priority %d\n", this->priority_));
  return 0;
}

int
Priority_Task::open (void *arg)
{
  ACE_Thread_Priority priority(ACE_Thread_Priority::ACE_NORMAL_PRIORITY_CLASS,
			       ACE_Thread_Priority::Thread_Priority(*(int *) arg));
  this->priority_ = priority.os_default_thread_priority ();

  // Become an active object.
  ACE_ASSERT (this->activate (THR_NEW_LWP, 1, 0,
			      priority.os_default_thread_priority ()) != -1);
  return 0;
}

int
Priority_Task::svc (void)
{
  // Set up logging for this thread.
  ACE_NEW_THREAD;

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) prio = %d, priority_ = %d\n", 
	      prio, this->priority_));
  ACE_ASSERT (this->priority_ == prio);
  return 0;
}

#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Priority_Task_Test");

#if defined (ACE_HAS_THREADS)

  Priority_Task tasks[ACE_MAX_ITERATIONS];

  int i;

  // Spawn off ACE_MAX_ITERATIONS of tasks, passing each one their
  // iteration number as their priority.

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    tasks[i].open ((void *) &i);

  // Wait for all tasks to exit.
  ACE_Service_Config::thr_mgr ()->wait ();

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    tasks[i].close ();
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
