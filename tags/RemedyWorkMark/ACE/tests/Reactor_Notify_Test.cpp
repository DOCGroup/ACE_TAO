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
//      This is a test that illustrates how the <ACE_Reactor>'s
//      <notify> method works under various <max_notify_iterations>
//      settings.  It also tests that the <disable_notify_pipe> option
//      works correctly.  Moreover, if the $ACE_ROOT/ace/config.h file
//      has the ACE_HAS_REACTOR_NOTIFICATION_QUEUE option enabled this
//      test will also exercise this feature.
//
// = AUTHOR
//      Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Synch_Traits.h"
#include "ace/Task.h"
#include "ace/Pipe.h"
#include "ace/Auto_Ptr.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Thread_Semaphore.h"



#if defined (ACE_HAS_THREADS)

static const time_t LONG_TIMEOUT = 10;
static const time_t SHORT_TIMEOUT = 2;

// A class to run a quiet event loop in one thread, and a plain notify()
// from the main thread to make sure a simple notify will wake up a quiet
// reactor.
class Quiet_Notify_Tester : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  Quiet_Notify_Tester (void) : result_ (0) {}
  ~Quiet_Notify_Tester (void) { this->wait (); }

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Start the reactor event thread.

  // Run the reactor event loop.
  virtual int svc (void);

  // Return the test result, 0 ok, -1 fail
  int result (void) const { return this->result_; }

private:
  ACE_Reactor r_;
  int result_;
};

int
Quiet_Notify_Tester::open (void *)
{
  this->reactor (&this->r_);
  return this->activate ();
}

int
Quiet_Notify_Tester::svc (void)
{
  // Count on the main thread doing a notify in less than LONG_TIMEOUT
  // seconds. If we don't get it, report a failure.
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Starting quiet event loop\n")));
  this->r_.owner (ACE_Thread::self ());
  ACE_Time_Value tmo (LONG_TIMEOUT);
  int status = this->r_.handle_events (&tmo);
  time_t remain = tmo.sec ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) event loop status %d, %: secs remain\n"),
              status, remain));
  if (remain == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Notify did not wake quiet event loop\n")));
      this->result_ = -1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Notify woke quiet event loop\n")));
    }
  return 0;
}

static int
run_quiet_notify_test (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Starting quiet notify test\n"));
  Quiet_Notify_Tester t;
  if (t.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Quiet notify activate")),
                      -1);
  // Now sleep a bit, then do a simple, reactor wake-up
  ACE_OS::sleep (SHORT_TIMEOUT);
  t.reactor ()->notify ();
  t.wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Quiet notify test done\n"));
  return t.result ();
}


class Supplier_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Supplier_Task (int disable_notify_pipe,
                 const ACE_Time_Value &tv);
  // Constructor.

  ~Supplier_Task (void);
  // Destructor.

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Make this an Active Object.

  virtual int close (u_long);
  // Close down the supplier.
  //FUZZ: enable check_for_lack_ACE_OS

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
  : waiter_ ((unsigned int) 0), // Make semaphore "locked" by default.
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
  ACE_TEST_ASSERT (result != -1);

  // Register the pipe's write handle with the <Reactor> for writing.
  // This should mean that it's always "active."
  if (long_timeout_ == 0)
    {
      result = ACE_Reactor::instance ()->register_handler
        (this->pipe_.write_handle (),
         this,
         ACE_Event_Handler::WRITE_MASK);
      ACE_TEST_ASSERT (result != -1);
    }

  // Make this an Active Object.
  result = this->activate (THR_BOUND | THR_DETACHED);
  ACE_TEST_ASSERT (result != -1);
  return 0;
}

int
Supplier_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Supplier_Task::close\n")));

  int result;

  if (long_timeout_ == 0)
    {
      result = ACE_Reactor::instance ()->remove_handler
        (this->pipe_.write_handle (),
         ACE_Event_Handler::WRITE_MASK);
      ACE_TEST_ASSERT (result != -1);
    }
  else
    {
      // Wait to be told to shutdown by the main thread.

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) waiting to be shutdown by main thread\n")));
      result = this->waiter_.acquire ();
      ACE_TEST_ASSERT (result != -1);
    }
  return 0;
}

Supplier_Task::~Supplier_Task (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) ~Supplier_Task\n")));
  this->pipe_.close ();
}

int
Supplier_Task::perform_notifications (int notifications)
{
  ACE_Reactor::instance ()->max_notify_iterations (notifications);

  size_t iterations = ACE_MAX_ITERATIONS;

  if (this->long_timeout_)
    {
      iterations *= (iterations * iterations * 2);
#if defined (ACE_VXWORKS)
      // scale down otherwise the test won't finish in time
      iterations /= 4;
#endif
    }

  for (size_t i = 0; i < iterations; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) notifying reactor on iteration %d\n"),
                  i));

      int result;

      // Notify the Reactor, which will call <handle_exception>.
      result = ACE_Reactor::instance ()->notify (this);
      if (result == -1)
        {
          if (errno == ETIME)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("notify")));
          else
            ACE_TEST_ASSERT (result != -1);
        }

      // Wait for our <handle_exception> method to release the
      // semaphore.
      if (this->long_timeout_ == 0
          && this->disable_notify_pipe_ == 0)
        {
          result = this->waiter_.acquire ();
          ACE_TEST_ASSERT (result != -1);
        }
    }

  return 0;
}

int
Supplier_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) **** starting unlimited notifications test\n")));

  // Allow an unlimited number of iterations per
  // <ACE_Reactor::notify>.
  this->perform_notifications (-1);

  if (this->long_timeout_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) **** starting limited notifications test\n")));

      // Only allow 1 iteration per <ACE_Reactor::notify>
      this->perform_notifications (1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) **** exiting thread test\n")));
    }
  return 0;
}

int
Supplier_Task::handle_exception (ACE_HANDLE handle)
{
  ACE_TEST_ASSERT (handle == ACE_INVALID_HANDLE);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) handle_exception\n")));

  this->waiter_.release ();
  return 0;
}

int
Supplier_Task::handle_output (ACE_HANDLE handle)
{
  ACE_TEST_ASSERT (handle == this->pipe_.write_handle ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) handle_output\n")));

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
                  ACE_Reactor (reactor_impl, 1),    // Delete implementation
                  -1);

  // Make sure this stuff gets cleaned up when this function exits.
  auto_ptr<ACE_Reactor> r (reactor);

  // Set the Singleton Reactor.
  ACE_Reactor *orig_reactor = ACE_Reactor::instance (reactor);
  ACE_TEST_ASSERT (ACE_LOG_MSG->op_status () != -1);
  ACE_TEST_ASSERT (ACE_Reactor::instance () == reactor);

  Supplier_Task task (disable_notify_pipe,
                      tv);
  ACE_TEST_ASSERT (ACE_LOG_MSG->op_status () != -1);

  int result;

  result = task.open ();
  ACE_TEST_ASSERT (result != -1);

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
                  ACE_TEXT ("(%t) starting handle_events() on iteration %d")
                  ACE_TEXT (" with time-out = %d seconds\n"),
                  iteration,
                  timeout.sec ()));

      // Use a timeout to inform the Reactor when to shutdown.
      switch (ACE_Reactor::instance ()->handle_events (timeout))
        {
        case -1:
          if (! disable_notify_pipe)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("reactor")));
          shutdown = 1;
          break;
          /* NOTREACHED */
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) handle_events timed out\n")));
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
                  ACE_TEXT ("(%t) releasing supplier task thread\n")));
      task.release ();
    }
  ACE_Reactor::instance (orig_reactor);
  return 0;
}

#endif /* ACE_HAS_THREADS */

class Purged_Notify : public ACE_Event_Handler
{
  // = TITLE
  //   <run_notify_purge_test> tests the reactor's
  //   purge_pending_notifications function. It does 2 notifications,
  //   and explicitly cancels one, and deletes the other's event
  //   handler, which should cause it to be cancelled as well.

  virtual int handle_exception (ACE_HANDLE = ACE_INVALID_HANDLE)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Got a notify that should have been purged!\n")),
                      0);
  }
};

static int
run_notify_purge_test (void)
{
  int status;
  ACE_Reactor *r = ACE_Reactor::instance ();
  {
    Purged_Notify n1;
    Purged_Notify *n2;

    ACE_NEW_RETURN (n2, Purged_Notify, -1);
    auto_ptr<Purged_Notify> ap (n2);

    // First test:
    // Notify EXCEPT, and purge ALL
    r->notify (&n1); // the mask is EXCEPT_MASK

    status = r->purge_pending_notifications (&n1);
    if (status == -1 && errno == ENOTSUP)
      return 0;         // Select Reactor w/o ACE_HAS_REACTOR_NOTIFICATION_QUEUE
    if (status != 1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Purged %d notifies; expected 1\n"),
                  status));
    // Second test:
    // Notify READ twice, and WRITE once, and purge READ and WRITE - should purge 3 times.
    r->notify (&n1, ACE_Event_Handler::READ_MASK);
    r->notify (&n1, ACE_Event_Handler::READ_MASK);
    r->notify (&n1, ACE_Event_Handler::WRITE_MASK);
    status = r->purge_pending_notifications
      (&n1, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);
    if (status != 3)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Purged %d notifies; expected 3\n"),
                  status));
    // Third test:
    // Notify READ on 2 handlers, and purge READ|WRITE on all handlers. Should purge 2
    r->notify (&n1, ACE_Event_Handler::READ_MASK);
    r->notify (n2, ACE_Event_Handler::READ_MASK);
    status = r->purge_pending_notifications
      (0, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);
    if (status != 2)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Purged %d notifies; expected 2\n"),
                  status));
    // Forth test:
    // Notify EXCEPT and WRITE, purge READ. Should not purge
    r->notify (&n1); // the mask is EXCEPT_MASK
    r->notify (&n1, ACE_Event_Handler::WRITE_MASK);
    status = r->purge_pending_notifications
      (&n1, ACE_Event_Handler::READ_MASK);
    if (status != 0)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Purged %d notifies; expected 0\n"),
                  status));
    // Fifth test:
    r->notify (n2);

    // The destructor of the event handler no longer removes the
    // notifications.  It is the application's responsability to do
    // so.
    r->purge_pending_notifications(n2,
                                   ACE_Event_Handler::ALL_EVENTS_MASK);
    r->purge_pending_notifications(&n1,
                                   ACE_Event_Handler::ALL_EVENTS_MASK);
  }

  ACE_Time_Value t (1);
  status = r->handle_events (t);  // Should be nothing to do, and time out
  return status < 0 ? 1 : 0;     // Return 0 for all ok, else error
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Notify_Test"));

  int test_result = 0;       // Innocent until proven guilty

  if (0 == run_notify_purge_test ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("purge_pending_notifications test OK\n")));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("purge_pending_notifications test FAIL\n")));
      test_result = 1;
    }

#if defined (ACE_HAS_THREADS)
  if (0 != run_quiet_notify_test ())
    test_result = 1;

  ACE_Time_Value timeout (SHORT_TIMEOUT);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) running tests with notify pipe enabled")
              ACE_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (0, timeout);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) running tests with notify pipe disabled")
              ACE_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (1, timeout);

  timeout.set (LONG_TIMEOUT, 0);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) running tests with reactor notification ")
              ACE_TEXT ("pipe enabled\n")
              ACE_TEXT (" and time-out = %d seconds\n"),
              timeout.sec ()));
  run_test (0, timeout);

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return test_result;
}
