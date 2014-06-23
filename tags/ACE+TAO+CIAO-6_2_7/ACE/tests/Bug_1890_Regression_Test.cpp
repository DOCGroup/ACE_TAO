/**
 * @file Bug_1890_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 1890
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1890
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 * Based on a test provided by "Vadim" (no further details available)
 */

#include "test_config.h"

#include "ace/Pipe.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Auto_Ptr.h"

int const nhandlers = 3;

/**
 * This class is used to create real I/O in the test.  To keep the I/O under
 * control and keep the test to a single process we use ACE_Pipe.  This class
 * is known to work with the Reactor, in fact, that is its main function.
 *
 * Handler counts how many calls to handle_input() has the reactor performed.
 * When bug 1890 is triggered the Reactor continues to call the timers, but it
 * stops calling select() and the handle_input() functions.
 */
class Handler : public ACE_Event_Handler
{
public:
    Handler();

    //FUZZ: disable check_for_lack_ACE_OS
    /// Initialize the pipe and register with the reactor
    int open(ACE_Reactor * reactor);
    //FUZZ: enable check_for_lack_ACE_OS

    /// Return the current count
    size_t handle_input_count() const;

    /// Write some data
    void send_dummy_data();

    /// Reactor callback
    virtual ACE_HANDLE get_handle() const;
    virtual int handle_input(ACE_HANDLE);

private:
    size_t handle_input_count_;

    ACE_Pipe the_pipe_;

    ACE_HANDLE handles_[2];
};

/**
 * This is the main driver for the test.  This timer is called by the reactor
 * in a repeating interval.  On the first @c initial_iterations the Timer
 * writes data through all of its handlers.  On iteration @c initial_iteration
 * it triggers bug 1890 by removing all the handlers from the reactor, and
 * then re-adding one handler.
 *
 */
class Timer : public ACE_Event_Handler
{
public:
    Timer();

    //FUZZ: disable check_for_lack_ACE_OS
    int open(ACE_Reactor * reactor);
    void close();
    //FUZZ: enable check_for_lack_ACE_OS

    bool check_expected_results() const;

    virtual int handle_timeout(ACE_Time_Value const &, void const*);

private:
    void send_data_through_handlers();
    void remove_some_handlers();

    Handler & special_handler();
    Handler const & special_handler() const;

private:
    Handler handler_[nhandlers];
    int iteration_;

    size_t recorded_count_;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_1890_Regression_Test"));

  bool success = true;

  // Bug 1890 is all about ACE_Select_Reactor, so run it on that reactor
  // regardless of platform.
  ACE_Select_Reactor *impl_ptr = 0;
  ACE_NEW_RETURN (impl_ptr, ACE_Select_Reactor, -1);
  auto_ptr<ACE_Select_Reactor> auto_impl (impl_ptr);

  ACE_Reactor reactor (impl_ptr);

  // Create the timer, this is the main driver for the test
  Timer * timer = new Timer;

  // Initialize the timer and register with the reactor
  if (-1 == timer->open (&reactor))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Cannot initialize timer")),
                        -1);
    }

  reactor.run_reactor_event_loop ();

  // Verify that the results are what we expect
  if (!(success = timer->check_expected_results ()))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Test failed\n")));

  // Cleanup
  timer->close ();
  delete timer;

  ACE_END_TEST;

  return success ? 0 : -1;
}

Handler::Handler()
    : handle_input_count_(0)
    , the_pipe_()
{
}

int
Handler::open (ACE_Reactor * r)
{
  if (-1 == the_pipe_.open (handles_))
    {
      return -1;
    }
  if (-1 == r->register_handler (this, ACE_Event_Handler::READ_MASK))
    {
      return -1;
    }
  return 0;
}

size_t
Handler::handle_input_count() const
{
  return handle_input_count_;
}

void
Handler::send_dummy_data()
{
  char buf[] = "dummy";
  (void) the_pipe_.send (buf, sizeof (buf));
}

ACE_HANDLE
Handler::get_handle() const
{
  return the_pipe_.read_handle ();
}

int
Handler::handle_input(ACE_HANDLE)
{
  ++handle_input_count_;
  // ACE_DEBUG((LM_DEBUG, "Handler::handle_input called for %d\n", h));
  return 0;
}

int const initial_iterations = 5;
int const total_iterations = 10;

int const special_handler_index = nhandlers - 1;

Timer::Timer()
    : iteration_(0)
    , recorded_count_(0)
{
}

int
Timer::open (ACE_Reactor * r)
{
  this->reactor (r);

  // Initialize both handles and register them with the reactor for reading.
  for (int i = 0; i != nhandlers; ++i)
    {
      if (-1 == handler_[i].open (r))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Could not open dummy handler %d %p\n"),
                             i,
                             ACE_TEXT ("")),
                            -1);
        }
    }

  ACE_Time_Value const interval(0, ACE_ONE_SECOND_IN_USECS / 10);
  ACE_Time_Value const startup (0, ACE_ONE_SECOND_IN_USECS / 20);

  if (-1 == r->schedule_timer (this, 0, startup, interval))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Could not schedule timer")),
                        -1);
    }

  return 0;
}

void
Timer::close()
{
  for (int i = 0; i != nhandlers; ++i)
    {
      this->reactor ()->remove_handler (&handler_[i],
                                        ACE_Event_Handler::ALL_EVENTS_MASK |
                                        ACE_Event_Handler::DONT_CALL);
    }
  this->reactor ()->cancel_timer (this);
}

bool
Timer::check_expected_results() const
{
  if (this->recorded_count_ < this->special_handler ().handle_input_count ())
    {
      return true;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("recorded_count %B, special_handler count %B\n"),
              this->recorded_count_,
              this->special_handler ().handle_input_count ()));
  return false;
}

int
Timer::handle_timeout(ACE_Time_Value const &, void const *)
{
  if (iteration_ == 0)
    {
      send_data_through_handlers();
    }

  ++iteration_;
  if (iteration_ < initial_iterations)
    {
      return 0;
    }

  if (iteration_ == initial_iterations)
    {
      remove_some_handlers();
      recorded_count_  = special_handler().handle_input_count();
      return 0;
    }

  if (iteration_ < total_iterations)
    {
      return 0;
    }

  reactor()->end_reactor_event_loop();

  return 0;
}

void
Timer::send_data_through_handlers()
{
  for (int i = 0; i != nhandlers; ++i)
    {
      handler_[i].send_dummy_data();
    }
}

void
Timer::remove_some_handlers()
{
  // The reactor may not get around to callbacks on deletion until the test
  // is over.
  ACE_Reactor_Mask mask =
    ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
  for (int i = 0; i != nhandlers; ++i)
    {
      if (-1 == reactor()->remove_handler(&handler_[i], mask))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Cannot remove handler %d in %p\n"),
                      i,
                      ACE_TEXT ("timeout")));
        }
    }

  if (-1 == reactor()->register_handler(&special_handler(),
                                        ACE_Event_Handler::ALL_EVENTS_MASK))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot add back special handler in %p\n"),
                  ACE_TEXT ("timeout")));
    }
}

Handler & Timer::special_handler()
{
  return handler_[special_handler_index];
}

Handler const & Timer::special_handler() const
{
  return handler_[special_handler_index];
}
