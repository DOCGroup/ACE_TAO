// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Reactors_Test.cpp
//
// = DESCRIPTION
//      This is a test that performs a torture test of multiple
//      <ACE_Reactors> and <ACE_Tasks> in the same process.
//
// = AUTHOR
//    Prashant Jain, Detlef Becker, and Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Synch.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

static const int MAX_TASKS = 20;

class Test_Task : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //    Exercise the tasks.
{
public:
  // = Initialization and termination methods.
  Test_Task (void);
  ~Test_Task (void);

  // = Task hooks.
  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);
  virtual int svc (void);

  // = Event Handler hooks.
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE fd, 
			    ACE_Reactor_Mask close_mask);

private:
  int handled_;
  // Number of iterations handled.

  static int task_count_;
  // Number of tasks running.
};

// Static data member initialization.
int Test_Task::task_count_ = 0;

static ACE_Atomic_Op<ACE_Thread_Mutex, int> done_count = MAX_TASKS * 2;

static ACE_Recursive_Thread_Mutex recursive_lock;

Test_Task::Test_Task (void)
  : handled_ (0)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, recursive_lock);

  Test_Task::task_count_++;

  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) TT+ Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));
}

Test_Task::~Test_Task (void)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, recursive_lock);

  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) TT- Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));

  ACE_ASSERT (Test_Task::task_count_ == 0);
}

int 
Test_Task::open (void *args)
{
  this->reactor ((ACE_Reactor *) args);
  return this->activate (THR_NEW_LWP);
}

int 
Test_Task::close (u_long)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, recursive_lock, -1);

  Test_Task::task_count_--;
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) close Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));

  if (Test_Task::task_count_ < 0)
    abort ();
  return 0;
}

int 
Test_Task::svc (void)
{
  ACE_NEW_THREAD;

  ACE_DEBUG ((LM_DEBUG, "(%t) svc\n"));

  for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_OS::thr_yield ();

      // Only wait up to 10 milliseconds to notify the Reactor.
      ACE_Time_Value timeout (0, 10 * 1000);

      if (this->reactor ()->notify (this,
				    ACE_Event_Handler::READ_MASK,
				    &timeout) == -1)
	{
	  if (errno == ETIME)
	    ACE_DEBUG ((LM_DEBUG, "(%t) %p\n", "notify() timed out"));
	  else
	    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);
	}
    }

  return 0;
}

int 
Test_Task::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return 0;
}

int 
Test_Task::handle_input (ACE_HANDLE)
{
  this->handled_++;

  if (this->handled_ == ACE_MAX_ITERATIONS)
    {
      done_count--;
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) handle_input, handled_ = %d, done_count = %d\n",
		  this->handled_, (int) done_count));
    }

  ACE_OS::thr_yield ();
  return -1;
}

static void *
worker (void *args)
{
  ACE_NEW_THREAD;

  ACE_Reactor *reactor = (ACE_Reactor *) args;

  // Make this thread the owner of the Reactor's event loop.
  reactor->owner (ACE_Thread::self ());

  // Use a timeout to inform the Reactor when to shutdown.
  ACE_Time_Value timeout (4);

  for (;;)
    {
      switch (reactor->handle_events (timeout))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "reactor"), 0);
	  /* NOTREACHED */
	case 0:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) Reactor shutdown\n"), 0);
	  /* NOTREACHED */
	}
    }

  ACE_NOTREACHED (return 0);
}

#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Reactors_Test");

#if defined (ACE_HAS_THREADS)
  // We need this to make sure the Reactor Singleton gets deleted!
  ACE_Service_Config daemon; 
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  ACE_Reactor *reactor;

  ACE_NEW_RETURN (reactor, ACE_Reactor, -1);
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  Test_Task tt1[MAX_TASKS];
  Test_Task tt2[MAX_TASKS];

  // Activate all of the Tasks.

  for (int i = 0; i < MAX_TASKS; i++)
    {
      tt1[i].open (ACE_Reactor::instance ());
      tt2[i].open (reactor);
    }

  // Spawn two threads each running a different reactor.

  if (ACE_Thread_Manager::instance ()->spawn 
      (ACE_THR_FUNC (worker),
       (void *) ACE_Reactor::instance (),
       THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  else if (ACE_Thread_Manager::instance ()->spawn 
      (ACE_THR_FUNC (worker), (void *) reactor, 
       THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  ACE_Thread_Manager::instance ()->wait ();

  reactor->close ();
  delete reactor;
  // Note that the destructor of ACE_Service_Config daemon will close
  // down the ACE_Reactor::instance().
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
