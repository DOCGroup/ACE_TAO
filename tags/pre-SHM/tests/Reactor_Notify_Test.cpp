// $Id$

// ============================================================================
//
// = LIBRARY
//      tests
//
// = FILENAME
//      Reactor_Notify_Test.cpp
//
// = DESCRIPTION
//      This is a test that illustrates how the <ACE_Reactor>'s <notify>
//      method works under various <max_notify_iterations> settings.
//      It also tests that the <disable_notify_pipe> option works
//      correctly.  Moreover, if the $ACE_ROOT/ace/config.h file has
//      the ACE_HAS_REACTOR_NOTIFICATION_QUEUE option enabled this
//      test will also exercise this feature.
//
// = AUTHOR
//      Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Pipe.h"
#include "ace/Auto_Ptr.h"
#include "ace/Select_Reactor.h"

ACE_RCSID(tests, Reactor_Notify_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

static const int LONG_TIMEOUT = 10;
static const int SHORT_TIMEOUT = 2;

class Supplier_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Supplier_Task (int disable_notify_pipe,
                 const ACE_Time_Value &tv);
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

  void release (void);
  // Release the <waiter_>.

private:
  int perform_notifications (int notifications);
  // Perform the notifications.

  ACE_Thread_Semaphore waiter_;
  // Used to hand-shake between the <Supplier_Task> and the
  // <Reactor>'s notify mechanism.

  ACE_Pipe pipe_;
  // We use this pipe just to get a handle that is always "active,"
  // i.e., the <ACE_Reactor> will always dispatch its <handle_output>
  // method.

  int disable_notify_pipe_;
  // Keeps track of whether the notification pipe in the <ACE_Reactor>
  // has been diabled or not.

  int long_timeout_;
  // Keeps track of whether we're running with a <LONG_TIMEOUT>, which
  // is used for the ACE_HAS_REACTOR_NOTIFICATION_QUEUE portion of
  // this test.
};

void
Supplier_Task::release (void)
{
  this->waiter_.release ();
}

Supplier_Task::Supplier_Task (int disable_notify_pipe,
                              const ACE_Time_Value &tv)
  : waiter_ (0), // Make semaphore "locked" by default.
    disable_notify_pipe_ (disable_notify_pipe),
    long_timeout_ (tv.sec () == LONG_TIMEOUT)
{
}

int
Supplier_Task::open (void *)
{
  // Create the pipe.
  int result;

  result = this->pipe_.open ();
  ACE_ASSERT (result != -1);

  // Register the pipe's write handle with the <Reactor> for writing.
  // This should mean that it's always "active."
  if (long_timeout_ == 0)
    {
      result = ACE_Reactor::instance ()->register_handler
        (this->pipe_.write_handle (),
         this,
         ACE_Event_Handler::WRITE_MASK);
      ACE_ASSERT (result != -1);
    }

  // Make this an Active Object.
  result = this->activate (THR_BOUND | THR_DETACHED);
  ACE_ASSERT (result != -1);
  return 0;
}

int
Supplier_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) Supplier_Task::close\n")));

  int result;

  if (long_timeout_ == 0)
    {
      result = ACE_Reactor::instance ()->remove_handler
        (this->pipe_.write_handle (),
         ACE_Event_Handler::WRITE_MASK);
      ACE_ASSERT (result != -1);
    }
  else
    {
      // Wait to be told to shutdown by the main thread.

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) waiting to be shutdown by main thread\n")));
      result = this->waiter_.acquire ();
      ACE_ASSERT (result != -1);
    }
  return 0;
}

Supplier_Task::~Supplier_Task (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) ~Supplier_Task\n")));
  this->pipe_.close ();
}

int
Supplier_Task::perform_notifications (int notifications)
{
  ACE_Reactor::instance ()->max_notify_iterations (notifications);

  size_t iterations = ACE_MAX_ITERATIONS;

  if (this->long_timeout_)
    iterations *= (iterations * iterations * 2);

  for (size_t i = 0; i < iterations; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) notifying reactor on iteration %d\n"),
                  i));

      int result;

      // Notify the Reactor, which will call <handle_exception>.
      result = ACE_Reactor::instance ()->notify (this);
      if (result == -1)
        {
          if (errno == ETIME)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("(%t) %p\n"),
                        ASYS_TEXT ("notify")));
          else
            ACE_ASSERT (result != -1);
        }

      // Wait for our <handle_exception> method to release the
      // semaphore.
      if (this->long_timeout_ == 0
          && this->disable_notify_pipe_ == 0)
        {
          result = this->waiter_.acquire ();
          ACE_ASSERT (result != -1);
        }
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
  this->perform_notifications (-1);

  if (this->long_timeout_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) **** starting limited notifications test\n")));

      // Only allow 1 iteration per <ACE_Reactor::notify>
      this->perform_notifications (1);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) **** exiting thread test\n")));
    }
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
  // That's because the pipe's write handle is always active.  Thus,
  // we can give the <Supplier_Task> a chance to run in its own
  // thread.
  ACE_OS::thr_yield ();

  return 0;
}

static int
run_test (int disable_notify_pipe,
          const ACE_Time_Value &tv)
{
  // Create special reactors with the appropriate flags enabled.

  ACE_Select_Reactor *reactor_impl = 0;
  if (disable_notify_pipe)
    ACE_NEW_RETURN (reactor_impl,
                    ACE_Select_Reactor (0, 0, 1),
                    -1);
  else
    ACE_NEW_RETURN (reactor_impl,
                    ACE_Select_Reactor,
                    -1);

  ACE_Reactor *reactor;
  ACE_NEW_RETURN (reactor,
                  ACE_Reactor (reactor_impl),
                  -1);

  // Make sure this stuff gets cleaned up when this function exits.
  auto_ptr<ACE_Reactor> r (reactor);
  auto_ptr<ACE_Select_Reactor> ri (reactor_impl);

  // Set the Singleton Reactor.
  ACE_Reactor::instance (reactor);
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);
  ACE_ASSERT (ACE_Reactor::instance () == reactor);

  Supplier_Task task (disable_notify_pipe,
                      tv);
  ACE_ASSERT (ACE_LOG_MSG->op_status () != -1);

  int result;

  result = task.open ();
  ACE_ASSERT (result != -1);

  if (tv.sec () == LONG_TIMEOUT)
    // Sleep for a while so that the <ACE_Reactor>'s notification
    // buffers will fill up!
    ACE_OS::sleep (tv);

  int shutdown = 0;

  // Run the event loop that handles the <handle_output> and
  // <handle_exception> notifications.
  for (int iteration = 1;
       shutdown == 0;
       iteration++)
    {
      ACE_Time_Value timeout (tv);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) starting handle_events() on iteration %d")
                  ASYS_TEXT (" with time-out = %d seconds\n"),
                  iteration,
                  timeout.sec ()));

      // Use a timeout to inform the Reactor when to shutdown.
      switch (ACE_Reactor::instance ()->handle_events (timeout))
        {
        case -1:
          if (! disable_notify_pipe)
            ACE_ERROR ((LM_ERROR,
                        ASYS_TEXT ("(%t) %p\n"),
                        ASYS_TEXT ("reactor")));
          shutdown = 1;
          break;
          /* NOTREACHED */
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%t) handle_events timed out\n")));
          shutdown = 1;
          break;
          /* NOTREACHED */
        default:
          break;
          /* NOTREACHED */
        }
    }

  if (tv.sec () == LONG_TIMEOUT)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) releasing supplier task thread\n")));
      task.release ();
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Notify_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_Time_Value timeout (SHORT_TIMEOUT);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running tests with notify pipe enabled")
              ASYS_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (0, timeout);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running tests with notify pipe disabled")
              ASYS_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (1, timeout);

  timeout.set (LONG_TIMEOUT, 0);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running tests with reactor notification pipe enabled\n")
              ASYS_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (0, timeout);

#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<ACE_Reactor>;
template class auto_ptr<ACE_Select_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Select_Reactor>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <ACE_Reactor>
#pragma instantiate auto_ptr <ACE_Select_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Select_Reactor>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
