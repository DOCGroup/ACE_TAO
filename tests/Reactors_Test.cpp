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
//      ACE_Reactors and ACE_Tasks in the same process.
//
// = AUTHOR
//    Prashant Jain and Detlef Becker
// 
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

static const int NUM_INVOCATIONS = 10;
static const int MAX_TASKS = 20;

class Test_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Test_Task (void);
  ~Test_Task (void);

  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);
  virtual int svc (void);

  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE fd, 
			    ACE_Reactor_Mask close_mask);

private:
  ACE_Reactor *r_;
  int handled_;

  static int task_count_;
};

int Test_Task::task_count_ = 0;

static ACE_Atomic_Op<ACE_Thread_Mutex, u_long> done_count = MAX_TASKS * 2;

static ACE_Recursive_Thread_Mutex reclock_;

Test_Task::Test_Task (void)
  : handled_ (0)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, reclock_);

  Test_Task::task_count_++;
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) TT+ Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));
}

Test_Task::~Test_Task (void)
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, reclock_);

  ACE_ASSERT (Test_Task::task_count_ == 0);
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) TT- Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));
}

int 
Test_Task::open (void *args)
{
  r_ = (ACE_Reactor *) args;
  return this->activate (THR_NEW_LWP);
}

int 
Test_Task::close (u_long)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, reclock_, -1);

  Test_Task::task_count_--;
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) close Test_Task::task_count_ = %d\n", 
	      Test_Task::task_count_));
  return 0;
}

int 
Test_Task::put (ACE_Message_Block *, ACE_Time_Value *)
{
  return 0;
}

int 
Test_Task::svc (void)
{
  ACE_NEW_THREAD;

  for (int i = 0; i < NUM_INVOCATIONS; i++)
    {
      ACE_OS::thr_yield ();

      if (r_->notify (this, ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);
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

  if (this->handled_ == NUM_INVOCATIONS)
    {
      done_count--;
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) handle_input, handled_ = %d, done_count = %d\n",
		  this->handled_, (u_long) done_count));
    }

  ACE_OS::thr_yield ();
  return -1;
}

static void *
worker (void *args)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;

  ACE_Reactor *reactor = (ACE_Reactor *) args;

  reactor->owner (ACE_Thread::self ());

  ACE_Time_Value timeout (4);

  for (;;)
    {
      switch (reactor->handle_events (timeout))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "reactor"), 0);
	  /* NOTREACHED */
	case 0:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) timeout\n"), 0);
	  /* NOTREACHED */
	}
    }

  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, u_long>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_HAS_THREADS */

int 
main (int, char *argv[])
{
  ACE_START_TEST ("Reactors_Test.cpp");

#if defined (ACE_HAS_THREADS)
  ACE_Reactor *react1 = ACE_Service_Config::reactor ();
  ACE_Reactor *react2;

  ACE_NEW_RETURN (react2, ACE_Reactor, -1);

  Test_Task tt1[MAX_TASKS];
  Test_Task tt2[MAX_TASKS];

  for (int i = 0; i < MAX_TASKS; i++)
    {
      tt1[i].open (react1);
      tt2[i].open (react2);
    }

  if (ACE_Service_Config::thr_mgr ()->spawn 
      (ACE_THR_FUNC (worker), (void *) react1, THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  else if (ACE_Service_Config::thr_mgr ()->spawn 
      (ACE_THR_FUNC (worker), (void *) react2, THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  ACE_Service_Config::thr_mgr ()->wait ();
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
