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
//      This is a test that illustrates how the <ACE_Reactor>'s <notify>
//      method works under various <max_notify_iterations> settings.
//      It also tests that the <disable_notify_pipe> option works
//      correctly.
//
// = AUTHOR
//      Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Pipe.h"
#include "ace/Select_Reactor.h"

ACE_RCSID(tests, Reactor_Notify_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

class Supplier_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Supplier_Task (int disable_notify_pipe);
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
  // Called every time through the main <ACE_Reactor> event loop to
  // illustrate the difference between "limited" and "unlimited"
  // notification.

private:
  int perform_notifications (int notifications);
  // Perform the notifications.

  ACE_Thread_Semaphore waiter_;
  // Used to hand-shake between the <Supplier_Task> and the
  // <Reactor>'s notify mechanism.

  ACE_Pipe pipe_;
  // We use this pipe just so we can get a handle that is always
  // "active."

  int disable_notify_pipe_;
  // Keeps track of whether the notification pipe in the <ACE_Reactor>
  // has been diabled or not.
};

Supplier_Task::Supplier_Task (int disable_notify_pipe)
  : waiter_ (0), // Make semaphore "locked" by default.
    disable_notify_pipe_ (disable_notify_pipe)
{
}

int
Supplier_Task::open (void *)
{
  // Create the pipe.
  if (this->pipe_.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) %p\n"),
                       ASYS_TEXT ("open failed")),
                      -1);
  // Register the pipe's write handle with the <Reactor> for writing.
  // This should mean that it's always "active."
  else if (ACE_Reactor::instance ()->register_handler
      (this->pipe_.write_handle (),
       this,
       ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) %p\n"),
                       ASYS_TEXT ("register_handler failed")),
                      -1);
  // Make this an Active Object.
  else if (this->activate (THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) %p\n"),
                       ASYS_TEXT ("activate failed")),
                      -1);
  else
    return 0;
}

int
Supplier_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) Supplier_Task::close\n")));

  if (ACE_Reactor::instance ()->remove_handler
      (this->pipe_.write_handle (),
       ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%t) %p\n"),
                ASYS_TEXT ("remove_handler failed")));
  return 0;
}

Supplier_Task::~Supplier_Task (void)
{
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) ~Supplier_Task\n")));
  this->pipe_.close ();
}

int
Supplier_Task::perform_notifications (int notifications)
{
  ACE_Reactor::instance ()->max_notify_iterations (notifications);

  for (size_t i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) notifying reactor\n")));
      // Notify the Reactor, which will call <handle_exception>.
      if (ACE_Reactor::instance ()->notify (this) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%t) %p\n"),
                           ASYS_TEXT ("notify")),
                          -1);

      // Wait for our <handle_exception> method to release the
      // semaphore.
      else if (this->disable_notify_pipe_ == 0
               && this->waiter_.acquire () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%t) %p\n"),
                           ASYS_TEXT ("acquire")),
                          -1);
    }
  return 0;
}

int
Supplier_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) **** starting unlimited notifications test\n")));

  // Allow an unlimited number of iterations per
  // <ACE_Reactor::notify>.
  if (this->perform_notifications (-1) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) **** starting limited notifications test\n")));

  // Only allow 1 iteration per <ACE_Reactor::notify>

  if (this->perform_notifications (1) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) **** exiting thread test\n")));
  return 0;
}

int
Supplier_Task::handle_exception (ACE_HANDLE handle)
{
  ACE_ASSERT (handle == ACE_INVALID_HANDLE);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) handle_exception\n")));

  this->waiter_.release ();
  return 0;
}

int
Supplier_Task::handle_output (ACE_HANDLE handle)
{
  ACE_ASSERT (handle == this->pipe_.write_handle ());
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) handle_output\n")));

  // This function is called by the main thread, believe it or not :-)
  // That's because the pipe's write handle is always active.  So,
  // give the <Supplier_Task> a chance to run.
  ACE_OS::thr_yield ();

  return 0;
}

static void
run_test (int disable_notify_pipe)
{
  // Create special reactors with the appropriate flags enabled.

  ACE_Select_Reactor *reactor_impl;
  if (disable_notify_pipe)
    ACE_NEW (reactor_impl,
             ACE_Select_Reactor (0, 0, 1));
  else
    ACE_NEW (reactor_impl,
             ACE_Select_Reactor);

  ACE_Reactor *reactor;
  ACE_NEW (reactor,
           ACE_Reactor (reactor_impl));

  // Set the Singleton Reactor.
  ACE_Reactor::instance (reactor);

  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);
  ACE_ASSERT (ACE_Reactor::instance () == reactor);

  Supplier_Task task (disable_notify_pipe);
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  if (task.open () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%t) open failed\n")));
  else
    {
      int shutdown = 0;

      // Run the event loop that handles the <handle_output> and
      // <handle_exception> notifications.
      for (int iteration = 1; shutdown == 0; iteration++)
        {
          ACE_Time_Value timeout (2);

          // Use a timeout to inform the Reactor when to shutdown.
          switch (ACE_Reactor::instance ()->handle_events (timeout))
            {
            case -1:
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("(%t) %p\n"),
                          ASYS_TEXT ("reactor")));
              shutdown = 1;
              break;
              /* NOTREACHED */
            case 0:
              shutdown = 1;
              break;
              /* NOTREACHED */
            default:
              break;
              /* NOTREACHED */
            }
        }
    }

  delete reactor_impl;
  delete reactor;
}

#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Notify_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running tests with notify pipe enabled\n")));
  run_test (0);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running tests with notify pipe disabled\n")));
  run_test (1);
#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
