// $Id$

// ============================================================================
//
// = LIBRARY
//      tests
// 
// = FILENAME
//      Reactors_Test.cpp
//
// = DESCRIPTION
//      This is a test that illustrates how the Reactor's notify()
//      method works under various settings of max_notify_iterations().
//
// = AUTHOR
//      Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Pipe.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

class Supplier_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Supplier_Task (void);
  // Constructor.

  ~Supplier_Task (void);
  // Destructor.

  virtual int open (void * = 0);
  // Make this an Active Object.

  virtual int close (u_long);
  // Close down the supplier.

  virtual int svc (void);
  // Generates events and sends them to the <Reactor>'s <notify>
  // method.

  virtual int handle_exception (ACE_HANDLE);
  // Releases the <waiter_> semaphore when called by the <Reactor>'s
  // notify handler.

  virtual int handle_output (ACE_HANDLE);
  // Called every time through the main Reactor event loop to
  // illustrate the difference between "limited" and "unlimited"
  // notification.

private:
  ACE_Thread_Semaphore waiter_;
  // Used to hand-shake between the <Supplier_Task> and the
  // <Reactor>'s notify mechanism.

  ACE_Pipe pipe_;
  // We use this pipe just so we can get a handle that is always
  // "active."
};

Supplier_Task::Supplier_Task (void)
  : waiter_ (0) // Make semaphore "locked" by default.
{
}

int
Supplier_Task::open (void *)
{
  if (this->pipe_.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "open failed"), -1);
  else if (this->activate (THR_BOUND))
    // Make this an Active Object.
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "activate failed"), -1);
  else if (ACE_Reactor::instance()->register_handler 
      (this->pipe_.write_handle (), this, ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler failed"), -1);
  else
    return 0;
}

int
Supplier_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Supplier_Task::close\n"));

  if (ACE_Reactor::instance()->remove_handler
      (this->pipe_.write_handle (), ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "remove_handler failed"));
  return 0;
}

Supplier_Task::~Supplier_Task (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) ~Supplier_Task\n"));
}

int 
Supplier_Task::svc (void)
{
  size_t i;

  ACE_DEBUG ((LM_DEBUG, "(%t) **** starting unlimited notifications test\n"));

  // Allow an unlimited number of iterations per
  // <ACE_Reactor::notify>.
  ACE_Reactor::instance()->max_notify_iterations (-1);

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) notifying reactor\n"));
      // Notify the Reactor.
      if (ACE_Reactor::instance()->notify (this) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);

      // Wait for our <handle_exception> method to release the
      // semaphore.
      else if (this->waiter_.acquire () == -1) 
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "acquire"), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) **** starting limited notifications test\n"));

  // Only allow 1 iteration per <ACE_Reactor::notify>
  ACE_Reactor::instance()->max_notify_iterations (1);

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) notifying reactor\n"));
      // Notify the Reactor.
      if (ACE_Reactor::instance()->notify (this) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);

      // Wait for our <handle_exception> method to release the
      // semaphore.
      else if (this->waiter_.acquire () == -1) 
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "acquire"), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) **** exiting thread test\n"));
  return 0;
}

int 
Supplier_Task::handle_exception (ACE_HANDLE handle)
{
  ACE_ASSERT (handle == ACE_INVALID_HANDLE);
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_exception\n"));

  this->waiter_.release ();
  return 0;
}

int 
Supplier_Task::handle_output (ACE_HANDLE handle)
{
  ACE_ASSERT (handle == this->pipe_.write_handle ());
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_output\n"));
  return 0;
}

#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Reactor_Notify_Test");

#if defined (ACE_HAS_THREADS)
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  Supplier_Task task;
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  if (task.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) open failed\n"), -1);
  else
    {
      ACE_Time_Value timeout (2);

      int shutdown = 0;
      for (int iteration = 1; !shutdown; iteration++)
	{
	  // Use a timeout to inform the Reactor when to shutdown.
	  switch (ACE_Reactor::instance()->handle_events (timeout))
	    {
	    case -1:
	      ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "reactor"), -1);
	      /* NOTREACHED */
	    case 0:
              shutdown = 1;
              break;
	    default:
	      // ACE_DEBUG ((LM_DEBUG, "(%t) done dispatching %d\n", iteration));
	      ;
	    }
	}
    }
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
