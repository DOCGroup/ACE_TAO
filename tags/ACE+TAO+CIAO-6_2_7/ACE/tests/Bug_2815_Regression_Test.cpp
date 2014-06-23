/**
 * @file Bug_2815_Regression_Test.cpp
 *
 * $Id$
 *
 * Verify that the notification queue can be used with large numbers
 * of event handlers.
 *
 * Normally the ACE_Reactor uses a pipe to implement the notify()
 * methods.  ACE can be compiled with
 * ACE_HAS_REACTOR_NOTIFICATION_QUEUE, with this configuration flag
 * the Reactor uses a user-space queue to contain the notifications.
 * A single message is sent through the pipe to indicate "pipe not
 * empty."
 *
 * In this configuration, if an event handler is removed
 * from the Reactor the user-space queue has to be searched for
 * pending notifications and the notifications must be removed.
 *
 * The original implementation used a naive algorithm to search and
 * remove the handlers, which resulted in very high overhead when
 * removing handlers while having a very long notification queue.
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Select_Reactor.h"

class One_Shot_Handler;

/**
 * @class Driver
 *
 * @brief Main driver for the test, generates notification events and
 * verifies they are received correctly.
 *
 */
class Driver
{
public:
  Driver(ACE_Reactor * reactor,
         int max_notifications,
         char const *test_name);

  /// Run the test
  void run (void);

  /// One of the sub-handlers has received a notification
  void notification_received ();

  /// One of the sub-handlers has decided to skip several notifications
  void notifications_skipped (int skip_count);

  /**
   * @brief Return the reactor configured for this test
   */
  ACE_Reactor * reactor ();

private:
  /**
   * @brief Implement a single iteration.
   *
   * Each iteration of the test consists of sending multiple
   * notifications simultaneously.
   */
  void send_notifications (void);

  /**
   * @brief Return true if the test is finished.
   */
  bool done (void) const;

  /**
   * @brief Return true if there are more iterations to run.
   */
  bool more_iterations () const;

  /**
   * @brief Return true if the current iteration is completed.
   */
  bool current_iteration_done () const;

  /**
   * @brief Run one iteration of the test, each iteration doubles
   * the number of events.
   */
  int run_one_iteration (void);

  /**
   * @brief Initialize a bunch of One_Shot_Handlers
   */
  void initialize_handlers(
    int nhandlers, One_Shot_Handler ** handlers);

  /**
   * @brief Dispatch events to the One_Shot_Handlers
   */
  void notify_handlers(
    int nhandlers, One_Shot_Handler ** handlers);

  /**
   * @brief Helpful for debugging
   *
   * The number of notifications received, skipped and sent are
   * subject to simple invariants.  During debugging any violation of
   * those invariants indicates a problem in the application or the
   * Reactor.
   */
  void check_notification_invariants();

  /// A good place to set break points.
  void invariant_failed();

private:
  /**
   * @brief The reactor used in this test
   */
  ACE_Reactor * reactor_;

  /**
   * @brief The maximum number of notifications in any single
   * iteration.
   */
  int max_notifications_;

  /**
   * @brief The name of the test
   */
  char const * test_name_;

  /**
   * @brief Number of notifications received
   */
  int notifications_sent_;

  /**
   * @brief Number of notifications sent
   */
  int notifications_recv_;

  /**
   * @brief Number of notifications skipped because
   * the handler was removed
   */
  int notifications_skipped_;

  /**
   * @brief Number of notifications sent on each iteration
   */
  int notifications_curr_;
};

/**
 * @class One_Shot_Handler
 *
 * @brief A handler that removes itself from the reactor
 * after its first notification.
 *
 * To demonstrate the problems with the first implementation of
 * the notification queue we generate multiple event handlers.
 * Then we generate multiple notifications for each, but the handlers
 * remove themselves from the reactor when the first notification
 * is delivered.  This causes a lot of activity in the notification
 * queue.
 *
 */
class One_Shot_Handler : public ACE_Event_Handler
{
public:
  One_Shot_Handler(
    Driver * master_handler,
    char const * test_name,
    int id);

  /// Increase the number of expected notifications
  void notification_queued();

  /// Receive the notifications, but remove itself from the reactor on
  /// on the first one.
  virtual int handle_exception(ACE_HANDLE);

private:
  /// The driver for this test, communicate results to it
  Driver * master_handler_;

  /// The number of expected notifications
  int expected_notifications_;

  /// Identify the test and handler for debugging and better error output
  char const * test_name_;
  int id_;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2815_Regression_Test"));

#if !defined(ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Notification queue disabled. ")
              ACE_TEXT ("This test depends on purge_pending_notifications. ")
              ACE_TEXT ("Thus, the test is disabled in this case\n")));

#else
  int max_notifications = 512 * 1024;

  {
    ACE_Reactor select_reactor (
        new ACE_Select_Reactor,
        1);

    Driver handler(&select_reactor,
                   max_notifications,
                   "Select_Reactor");

    handler.run ();
  }

  {
    ACE_Reactor tp_reactor (new ACE_TP_Reactor,
                            1);
    Driver handler(&tp_reactor,
                   max_notifications,
                   "TP_Reactor");
    handler.run();
  }

#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;

  return 0;
}

Driver::Driver (
    ACE_Reactor * reactor,
    int max_notifications,
    char const * test_name)
  : reactor_(reactor)
  , max_notifications_(max_notifications)
  , test_name_(test_name)
  , notifications_sent_(0)
  , notifications_recv_(0)
  , notifications_skipped_(0)
  , notifications_curr_(1)
{
}

void
Driver::run (void)
{
  while(more_iterations())
  {
    if(run_one_iteration() == -1)
      {
        return;
      }

    notifications_curr_ *= 2;
  }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Test %C passed sent=%d, recv=%d, skip=%d\n"),
              test_name_,
              notifications_sent_,
              notifications_recv_,
              notifications_skipped_));
}

void
Driver::notification_received ()
{
  ++notifications_recv_;
  check_notification_invariants();
}

void
Driver::notifications_skipped (int skip_count)
{
  notifications_skipped_ += skip_count;
  check_notification_invariants();
}

ACE_Reactor *
Driver::reactor()
{
  return reactor_;
}

void
Driver::send_notifications (void)
{
  int const nhandlers = 16;
  One_Shot_Handler * handlers[nhandlers];
  initialize_handlers(nhandlers, handlers);

  for (int i = 0; i != notifications_curr_; ++i)
  {
    notify_handlers(nhandlers, handlers);
  }
}

bool
Driver::done (void) const
{
  return !more_iterations() && current_iteration_done();
}

bool
Driver::more_iterations() const
{
  return notifications_curr_ < max_notifications_;
}

bool
Driver::current_iteration_done() const
{
  return notifications_sent_ == (notifications_recv_ + notifications_skipped_);
}

int
Driver::run_one_iteration (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Running iteration with %d events for %C test\n"),
              notifications_curr_,
              test_name_));

  send_notifications ();

  // Run for 30 seconds or until the test is done.

  ACE_Time_Value const timeout(30,0);

  while (!current_iteration_done())
  {
    ACE_Time_Value start = ACE_OS::gettimeofday();
    ACE_Time_Value interval(1,0);
    reactor()->run_reactor_event_loop(interval);
    ACE_Time_Value end = ACE_OS::gettimeofday();

    if (end - start >= timeout)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Test %C failed due to timeout ")
                    ACE_TEXT (" sent=%d,recv=%d,skip=%d\n"),
                    test_name_,
                    notifications_sent_,
                    notifications_recv_,
                    notifications_skipped_));
        return -1;
      }
  }

  return 0;
}

void
Driver::initialize_handlers(
  int nhandlers, One_Shot_Handler ** handlers)
{
  for (int j = 0; j != nhandlers; ++j)
  {
    handlers[j] = new One_Shot_Handler(this, test_name_, j);
  }
}

void
Driver::notify_handlers(
  int nhandlers, One_Shot_Handler ** handlers)
{
  for(int i = 0; i != nhandlers; ++i)
  {
    if(reactor()->notify (handlers[i]) == -1)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT ("Cannot send notifications in %C test (%d/%d)\n"),
                   test_name_, i, notifications_curr_));
        return;
      }
    handlers[i]->notification_queued();

    ++notifications_sent_;
  }
}

void Driver::
check_notification_invariants()
{
  if (notifications_sent_ < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("The number of notifications sent (%d)")
                  ACE_TEXT(" should be positive\n"),
                  notifications_sent_));
      invariant_failed();
    }

  if (notifications_recv_ < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("The number of notifications received (%d)")
                  ACE_TEXT(" should be positive\n"),
                  notifications_recv_));
      invariant_failed();
    }

  if (notifications_skipped_ < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("The number of notifications skipped (%d)")
                  ACE_TEXT(" should be positive\n"),
                  notifications_skipped_));
      invariant_failed();
    }

  if (notifications_sent_ < notifications_recv_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Too many notifications received (%d)")
                  ACE_TEXT(" vs sent (%d)\n"),
                  notifications_recv_, notifications_sent_));
      invariant_failed();
    }

  if (notifications_sent_ < notifications_skipped_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Too many notifications skipped (%d)")
                  ACE_TEXT(" vs sent (%d)\n"),
                  notifications_skipped_, notifications_sent_));
      invariant_failed();
    }

  if (notifications_skipped_ + notifications_recv_ > notifications_sent_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Too many notifications skipped (%d)")
                  ACE_TEXT(" and received (%d) vs sent (%d)\n"),
                  notifications_skipped_, notifications_recv_,
                  notifications_sent_));
      invariant_failed();
    }
}

void Driver::
invariant_failed()
{
  // Just a good place to set a breakpoint
}

// ============================================

One_Shot_Handler::One_Shot_Handler(
  Driver * master_handler,
  char const * test_name, int id)
  : ACE_Event_Handler(master_handler->reactor())
  , master_handler_(master_handler)
  , expected_notifications_(0)
  , test_name_(test_name)
  , id_(id)
{
}

void One_Shot_Handler::
notification_queued()
{
  ++expected_notifications_;
}

int One_Shot_Handler::
handle_exception(ACE_HANDLE)
{
  --expected_notifications_;
  master_handler_->notification_received();

  int r = reactor()->purge_pending_notifications(this);
  if (r >= 0)
    {
      master_handler_->notifications_skipped(r);
      delete this;
      return 0;
    }

  ACE_ERROR((LM_ERROR,
             ACE_TEXT ("Cannot remove handler %d in %C test\n"),
             id_, test_name_));

  delete this;
  return 0;
}
