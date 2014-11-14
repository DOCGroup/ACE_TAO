/**
 * @file  Reactor_Remove_Resume_Test.cpp
 *
 * $Id$
 *
 * This test verifies that ACE reactors only remove or resume the event
 * handler used during an upcall, not another with same handle value.
 * There is are least one case where the event handler can change
 * during an upcall.  The event handler could be removed by another
 * thread, and a new one is registered for a handle of the same value
 * (e.g. closed and then reopened) during an upcall.  "Misbehaved"
 * event handlers could also cause this problem by closing and
 * deregistering the event handler during an upcall.
 *
 * @author  Ossama Othman
 */


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Pipe.h"
#include "ace/Auto_Ptr.h"

#include <algorithm>
#include <functional>

int overall_result = 0;

// ------------------------------------------------------------

class Bogus_Handler : public ACE_Event_Handler
{
public:

  Bogus_Handler (ACE_Reactor * reactor,
                 ACE_HANDLE read_handle,
                 bool & okay_to_close);

protected:

  virtual ~Bogus_Handler (void);

  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  virtual int resume_handler (void);

private:

  ACE_HANDLE const read_handle_;

  // If the reactor closes the event handler before it gets the okay,
  // we will issue an error.
  bool & okay_to_close_;

};

Bogus_Handler::Bogus_Handler (ACE_Reactor * reactor,
                              ACE_HANDLE read_handle,
                              bool & okay_to_close)
  : ACE_Event_Handler (reactor)
  , read_handle_ (read_handle)
  , okay_to_close_ (okay_to_close)
{
  this->reference_counting_policy ().value (
     ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

Bogus_Handler::~Bogus_Handler (void)
{
}

ACE_HANDLE
Bogus_Handler::get_handle (void) const
{
  return this->read_handle_;
}

int
Bogus_Handler::handle_input (ACE_HANDLE)
{
  // This event handler should have been suspended, meaning it should
  // not have received any events.
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Bogus_Handler received an ")
                     ACE_TEXT ("unexpected event.\n")),
                    -1);
}

int
Bogus_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  if (this->okay_to_close_)
    return 0;

  overall_result = -1;

  // This event handler is being closed by the reactor unexpectedly.
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Bogus_Handler unexpectedly closed\n")),
                    -1);
}

int
Bogus_Handler::resume_handler (void)
{
  // We don't want the reactor to resume this event handler.
  return ACE_APPLICATION_RESUMES_HANDLER;
}

// ------------------------------------------------------------

class Bad_Handler : public ACE_Event_Handler
{
public:

  Bad_Handler (ACE_Reactor * reactor,
               ACE_HANDLE read_handle,
               bool & okay_to_close,
               bool suspension_test);

  ACE_HANDLE write_handle (void) const;

protected:

  virtual ~Bad_Handler (void);

private:

  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  int handle_input_result (void) const;

private:

  ACE_HANDLE const read_handle_;

  bool handle_close_called_;

  // Passed on to the Bogus_Handler constructor.  Not used by the
  // Bad_Handler, otherwise.
  bool & okay_to_close_;

  // Are we running the event handler suspension or removal test?
  bool suspension_test_;

};

Bad_Handler::Bad_Handler (ACE_Reactor * reactor,
                          ACE_HANDLE read_handle,
                          bool & okay_to_close,
                          bool suspension_test)
  : ACE_Event_Handler (reactor)
  , read_handle_ (read_handle)
  , handle_close_called_ (false)
  , okay_to_close_ (okay_to_close)
  , suspension_test_ (suspension_test)
{
  this->reference_counting_policy ().value (
     ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

}

Bad_Handler::~Bad_Handler (void)
{
}

ACE_HANDLE
Bad_Handler::get_handle (void) const
{
  return this->read_handle_;
}

int
Bad_Handler::handle_input (ACE_HANDLE handle)
{
  // Remove ourselves from the reactor, and trigger a different one
  // with the same handle to be registered.
  if (this->reactor ()->remove_handler (handle,
                                        ACE_Event_Handler::READ_MASK) != 0)
    {
      overall_result = -1;

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to remove Bad_Handler ")
                         ACE_TEXT ("from reactor\n")),
                        1);
    }

  ACE_Event_Handler_var bogus_handler (
    new Bogus_Handler (this->reactor (),
                       handle,
                       this->okay_to_close_));

  // Register and suspend a new handler.
  if (this->reactor ()->register_handler (handle,
                                          bogus_handler.handler (),
                                          ACE_Event_Handler::READ_MASK) != 0
      || this->reactor ()->suspend_handler (handle) != 0)
    {
      overall_result = -1;

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to register or suspend ")
                         ACE_TEXT ("Bogus_Handler\n")),
                        1);
    }

  // Make sure a read event exists for dispatching during the next
  // event loop iteration.
  if (this->reactor ()->schedule_wakeup (handle,
                                         ACE_Event_Handler::READ_MASK) == -1)
    {
      overall_result = -1;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Wakeup scheduling failed\n")));
    }

  return this->handle_input_result ();
}

int
Bad_Handler::handle_close (ACE_HANDLE,
                           ACE_Reactor_Mask)
{
  if (this->handle_close_called_)
    {
      overall_result = -1;

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Bad_Handler::handle_close() called ")
                         ACE_TEXT ("more than once\n")),
                        -1);
    }

  this->handle_close_called_ = true;

  return 0;
}

int
Bad_Handler::handle_input_result (void) const
{
  return
    (this->suspension_test_

     // Attempt to force reactor to resume the handle after the
     // upcall completes.
     ? 0

     // Attempt to force the new event handler to be removed from the
     // reactor.
     : -1);
}

// ------------------------------------------------------------
int
register_handler (ACE_Reactor * reactor,
                  ACE_HANDLE read_handle,
                  bool & okay_to_close,
                  bool suspension_test)
{
  ACE_Event_Handler_var bad_handler (new Bad_Handler (reactor,
                                                      read_handle,
                                                      okay_to_close,
                                                      suspension_test));

  // Register for read events.
  if (reactor->register_handler (bad_handler.handler (),
                                 ACE_Event_Handler::READ_MASK) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to register Bad_Handler with ")
                         ACE_TEXT ("reactor\n")),
                        -1);
    }

  return 0;
}

// ------------------------------------------------------------

int
send_data (ACE_HANDLE write_handle)
{
  char const foo[] = "foo";
  size_t const len = sizeof (foo); // We want the number of bytes, not
                                   // the number of characters.

  ACE_Time_Value const timeout (2);

  // Trigger a read event on the pipe handle.  This shouldn't timeout
  // since the pipe should be able to hold such a small amount of
  // data.
  size_t bytes_transferred = 0;
  ssize_t const result =
    ACE::send_n (write_handle,
                 foo,
                 len,
                 &timeout, // timeout
                 &bytes_transferred);

  if (result == -1
      || bytes_transferred != len)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Unable to send data")),
                        -1);
    }

  return 0;
}

// ------------------------------------------------------------

int
handle_events (ACE_Reactor & reactor,
               bool & okay_to_close)
{
  ACE_Time_Value timeout (2);

  // Only one event handler should have been dispatched.
  if (reactor.handle_events (&timeout) != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Initial event dispatch failed\n")));
    }
  else
    {
      okay_to_close = true;

      // Run the event loop again in an attempt to make the reactor
      // dispatch the newly registered event handler.  No events
      // should be dispatched.
      timeout.sec (2);
      int const result = reactor.handle_events (&timeout);


      if (result > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unexpectedly dispatched an event\n")));
        }
      else if (result < 0)
        {
          overall_result = -1;

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Event loop failed unexpectedly\n")));
        }
      else
        return 0;
    }

  return -1;
}

// ------------------------------------------------------------

typedef auto_ptr<ACE_Reactor_Impl> (*reactor_factory_type) (void);

auto_ptr<ACE_Reactor_Impl>
tp_reactor_factory (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Creating ACE_TP_Reactor.\n")));

  return auto_ptr<ACE_Reactor_Impl> (new ACE_TP_Reactor);
}

// ------------------------------------------------------------
/**
 * @struct Caller
 *
 * @brief Reactor test execution functor.
 *
 * Reactor test execution functor.
 */
struct Run_Test : public std::unary_function<reactor_factory_type, void>
{
  /// Function call operator overload.
  void operator() (reactor_factory_type factory)
  {
    bool const suspension_test[] =
      {
        true,  // Run suspension test.
        false  // Run removal test.
      };

    bool const * const begin = suspension_test;
    bool const * const end =
      suspension_test
      + sizeof (suspension_test) / sizeof (suspension_test[0]);

    for (bool const * i = begin; i != end; ++i)
      {
        bool const suspension_test = *i;

        if (suspension_test)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("** Running suspension test **\n")));
          }
        else
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("** Running removal test **\n")));
          }

        auto_ptr<ACE_Reactor_Impl> the_factory (factory ());
        ACE_Reactor reactor (the_factory.get ());

        // In this test, it's only okay to close the Bogus_Handler
        // when the reactor is destroyed.
        bool okay_to_close = false;

        ACE_Pipe the_pipe;
        if (the_pipe.open () != 0
            || register_handler (&reactor,
                                 the_pipe.read_handle (),
                                 okay_to_close,
                                 suspension_test) != 0
            || send_data (the_pipe.write_handle ()) != 0
            || handle_events (reactor, okay_to_close) != 0)
          {
            overall_result = -1;
          }

        okay_to_close = true;

        ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n"))); // For log aesthetics.
      }
  }
};

// ------------------------------------------------------------

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Remove_Resume_Test"));

  static reactor_factory_type const factories[] =
    {
      tp_reactor_factory
    };

  static size_t const factory_count = sizeof (factories) / sizeof (factories[0]);

  std::for_each (factories, factories + factory_count, Run_Test ());

  if (overall_result != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Test failed.\n")));
  else
    ACE_ERROR ((LM_INFO,
                ACE_TEXT ("Test passed.\n")));

#if defined (ACE_HAS_CPP11)
  ACE_Event_Handler_var nullvar;
  if (!nullvar)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EH_var explicit operator bool ok\n")));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("EH_var explicit operator bool FAILED\n")));
    ++overall_result;
  }
  if (nullvar == nullptr)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EH_var explicit operator== nullptr ok\n")));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("EH_var explicit operator== nullptr FAILED\n")));
    ++overall_result;
  }
  if (!(nullvar != nullptr))
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EH_var explicit operator!= nullptr ok\n")));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("EH_var explicit operator!= nullptr FAILED\n")));
    ++overall_result;
  }
#endif

  ACE_END_TEST;

  return overall_result;
}
