
//=============================================================================
/**
 *  @file    Reference_Counted_Event_Handler_Test.cpp
 *
 *  $Id$
 *
 *  This test is used to check reference counting of the Event
 *  Handler when it interacts with the Reactor.
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"

static const char message[] = "abcdefghijklmnopqrstuvwxyz";
static const int message_size = 26;
static int test_select_reactor = 1;
static int test_tp_reactor = 1;
static int test_wfmo_reactor = 1;
static int test_dev_poll_reactor = 1;
static int test_io = 1;
static int test_timers = 1;
static int test_find = 1;
static int test_simple_event_handler = 1;
static int test_reference_counted_event_handler_1 = 1;
static int test_reference_counted_event_handler_2 = 1;
static int test_closed_in_upcall_event_handler = 1;
static int debug = 1;
static const char *one_second_timeout = "one second timeout";
static const char *two_second_timeout = "two second timeout";

class Reference_Counted_Event_Handler : public ACE_Event_Handler
{
public:

  Reference_Counted_Event_Handler (int &events);

  ~Reference_Counted_Event_Handler (void);

  int handle_input (ACE_HANDLE);

  int handle_output (ACE_HANDLE);

  int handle_timeout (const ACE_Time_Value &,
                      const void *);

  int handle_signal (int, siginfo_t *, ucontext_t *);

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask);

  ACE_Event_Handler::Reference_Count add_reference (void);

  ACE_Event_Handler::Reference_Count remove_reference (void);

  ACE_Pipe pipe_;

  int &events_;

};

Reference_Counted_Event_Handler::Reference_Counted_Event_Handler (int &events)
  : events_ (events)
{
  if (this->pipe_.open () != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("ctor: pipe open")));

  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  ACE_DEBUG ((LM_DEBUG,
              "Reference count in Reference_Counted_Event_Handler() is %d\n",
              this->reference_count_.value ()));
}

Reference_Counted_Event_Handler::~Reference_Counted_Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference count in ~Reference_Counted_Event_Handler() is %d\n",
              this->reference_count_.value ()));

  this->pipe_.close ();
}

int
Reference_Counted_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference count in Reference_Counted_Event_Handler::handle_input() is %d\n",
              this->reference_count_.value ()));

  --this->events_;

  char buf[message_size + 1];

  ssize_t result =
    ACE::recv_n (this->pipe_.read_handle (),
                 buf,
                 sizeof buf - 1);

  if (result != message_size)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("recv_n expected %d bytes; got %b\n"),
                message_size,
                result));

  buf[message_size] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              "Message received: %C\n",
              buf));

  return 0;
}

int
Reference_Counted_Event_Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference count in Reference_Counted_Event_Handler::handle_output() is %d\n",
              this->reference_count_.value ()));

  --this->events_;

  ssize_t result =
    ACE::send_n (this->pipe_.write_handle (),
                 message,
                 message_size);

  if (result != message_size)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("send_n sent %b bytes; should be %d\n"),
                result,
                message_size));

  // No longer interested in output.
  return -1;
}

int
Reference_Counted_Event_Handler::handle_timeout (const ACE_Time_Value &,
                                                 const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference count in Reference_Counted_Event_Handler::handle_timeout() for arg = %C is %d\n",
              (const char *) arg,
              this->reference_count_.value ()));

  --this->events_;

  return 0;
}

int
Reference_Counted_Event_Handler::handle_signal (int,
                                                siginfo_t *,
                                                ucontext_t *)
{
  return 0;
}

int
Reference_Counted_Event_Handler::handle_close (ACE_HANDLE handle,
                                               ACE_Reactor_Mask masks)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference_Counted_Event_Handler::handle_close() called with handle = %d and masks = %d. "
              "Reference count is %d\n",
              handle,
              masks,
              this->reference_count_.value ()));

  return 0;
}

ACE_Event_Handler::Reference_Count
Reference_Counted_Event_Handler::add_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::add_reference ();

  ACE_DEBUG ((LM_DEBUG,
              "Reference count after add_reference() is %d\n",
              this->reference_count_.value ()));

  return reference_count;
}

ACE_Event_Handler::Reference_Count
Reference_Counted_Event_Handler::remove_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::remove_reference ();

  ACE_DEBUG ((LM_DEBUG,
              "Reference count after remove_reference() is %d\n",
              reference_count));

  return reference_count;
}

void
reference_counted_event_handler_test_1 (ACE_Reactor *reactor)
{
  int events = 0;
  int result = 0;

  Reference_Counted_Event_Handler *handler =
    new Reference_Counted_Event_Handler (events);

  ACE_Event_Handler_var safe_handler (handler);

  if (test_io)
    {
      if (-1 == reactor->register_handler (handler->pipe_.read_handle (),
                                           handler,
                                           ACE_Event_Handler::READ_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe read")));
      else
        ++events;

      if (-1 == reactor->register_handler (handler->pipe_.write_handle (),
                                           handler,
                                           ACE_Event_Handler::WRITE_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe write")));
      else
        events++;
    }

  if (test_timers)
    {
      ACE_Time_Value const one_second (1);
      long timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        if ((result = reactor->cancel_timer (timer_id, 0, 0)) != 1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("cancel_timer returned %d; should be 1\n"),
                      result));

      timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events += 2;  // Wait for the scheduled and one repeating

      ACE_Time_Value const two_second (2);
      timer_id =
        reactor->schedule_timer (handler,
                                 two_second_timeout,
                                 two_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events++;
    }

  while (events > 0)
    {
      result =
        reactor->handle_events ();
    }
}

void
reference_counted_event_handler_test_2 (ACE_Reactor *reactor)
{
  int events = 0;
  int result = 0;
  ACE_Time_Value const one_second (1);

  if (test_find)
    {
      Reference_Counted_Event_Handler *handler =
        new Reference_Counted_Event_Handler (events);

      ACE_Event_Handler_var safe_handler (handler);

      result =
        reactor->register_handler (handler->pipe_.read_handle (),
                                   handler,
                                   ACE_Event_Handler::READ_MASK);
      if (result != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe handler read")));
      else
      {
        ACE_Event_Handler *result_handler = 0;

        result =
          reactor->handler (handler->pipe_.read_handle (),
                            ACE_Event_Handler::READ_MASK,
                            &result_handler);
        ACE_Event_Handler_var safe_result_handler (result_handler);

        if (result != 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Looking up pipe read handler")));
        else
          if (result_handler != handler)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Mismatch: result_handler %@ should be %@\n"),
                        result_handler, handler));
      }

      {
        ACE_Event_Handler *result_handler = 0;

        result =
          reactor->handler (handler->pipe_.read_handle (),
                            ACE_Event_Handler::WRITE_MASK,
                            &result_handler);
        ACE_Event_Handler_var safe_result_handler (result_handler);

        if (result == 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Pipe write handler succeeded but shouldn't")));
      }

      {
        ACE_Event_Handler_var result_handler =
          reactor->find_handler (handler->pipe_.read_handle ());

        if (result_handler.handler () != handler)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Mismatch 2: result_handler %@ should be %@\n"),
                      result_handler.handler (), handler));
      }
    }

  if (test_io)
    {
      Reference_Counted_Event_Handler *handler =
        new Reference_Counted_Event_Handler (events);

      ACE_Event_Handler_var safe_handler (handler);

      if (-1 == reactor->register_handler (handler->pipe_.read_handle (),
                                           handler,
                                           ACE_Event_Handler::READ_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe read")));
      else
        ++events;

      if (-1 == reactor->register_handler (handler->pipe_.write_handle (),
                                           handler,
                                           ACE_Event_Handler::WRITE_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe write")));
      else
        events++;
    }

  if (test_timers)
    {
      Reference_Counted_Event_Handler *handler =
        new Reference_Counted_Event_Handler (events);

      ACE_Event_Handler_var safe_handler (handler);

      long timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        if ((result = reactor->cancel_timer (timer_id, 0, 0)) != 1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("cancel_timer returned %d; should be 1\n"),
                      result));
    }

  if (test_timers)
    {
      Reference_Counted_Event_Handler *handler =
        new Reference_Counted_Event_Handler (events);

      ACE_Event_Handler_var safe_handler (handler);

      long timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events += 2;  // Wait for the scheduled and one repeating

      ACE_Time_Value const two_second (2);
      timer_id =
        reactor->schedule_timer (handler,
                                 two_second_timeout,
                                 two_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events++;
    }

  while (events > 0)
    {
      result =
        reactor->handle_events ();
    }
}

void
reference_count_1 (ACE_Reactor_Impl *impl)
{
  ACE_Reactor reactor (impl, 1);

  ACE_DEBUG ((LM_DEBUG,
              "\nTesting Reference Counted Event Handler Test 1....\n\n"));

  reference_counted_event_handler_test_1 (&reactor);
}

void
reference_count_2 (ACE_Reactor_Impl *impl)
{
  ACE_Reactor reactor (impl, 1);

  ACE_DEBUG ((LM_DEBUG,
              "\nTesting Reference Counted Event Handler Test 2....\n\n"));

  reference_counted_event_handler_test_2 (&reactor);
}

class Simple_Event_Handler : public ACE_Event_Handler
{
public:

  Simple_Event_Handler (int &events,
                        int close_count);

  ~Simple_Event_Handler (void);

  int handle_input (ACE_HANDLE);

  int handle_output (ACE_HANDLE);

  int handle_timeout (const ACE_Time_Value &,
                      const void *);

  int handle_signal (int, siginfo_t *, ucontext_t *);

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask);

  ACE_Pipe pipe_;

  int &events_;

  int close_count_;

};

Simple_Event_Handler::Simple_Event_Handler (int &events,
                                            int close_count)
  : events_ (events),
    close_count_ (close_count)
{
  if (-1 == this->pipe_.open ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Simple_Event_Handler pipe open")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Event_Handler()\n")));
}

Simple_Event_Handler::~Simple_Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~Simple_Event_Handler()\n")));

  this->pipe_.close ();
}

int
Simple_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Event_Handler::handle_input()\n")));

  --this->events_;

  char buf[message_size + 1];

  ssize_t result =
    ACE::recv_n (this->pipe_.read_handle (),
                 buf,
                 sizeof buf - 1);
  if (result != message_size)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("line %l recv_n got %b; should be %d\n"),
                result, message_size));
  buf[message_size] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message received: %C\n"),
              buf));

  return 0;
}

int
Simple_Event_Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Event_Handler::handle_output()\n")));

  --this->events_;

  ssize_t result =
    ACE::send_n (this->pipe_.write_handle (),
                 message,
                 message_size);
  if (result != message_size)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("line %l send_n sent %b; should be %d\n"),
                result, message_size));

  // No longer interested in output.
  return -1;
}

int
Simple_Event_Handler::handle_timeout (const ACE_Time_Value &,
                                      const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Simple_Event_Handler::handle_timeout() for arg = %C\n"),
              (const char *) arg));

  --this->events_;

  return 0;
}

int
Simple_Event_Handler::handle_signal (int,
                                     siginfo_t *,
                                     ucontext_t *)
{
  return 0;
}

int
Simple_Event_Handler::handle_close (ACE_HANDLE handle,
                                    ACE_Reactor_Mask masks)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Simple_Event_Handler::handle_close() called with ")
              ACE_TEXT ("handle = %d and masks = %d with close count = %d.\n"),
              handle,
              masks,
              --this->close_count_));

  if (this->close_count_ == 0)
    delete this;

  return 0;
}

void
simple_event_handler (ACE_Reactor *reactor)
{
  int events = 0;
  int result = 0;
  ACE_Time_Value const one_second (1);

  if (test_find)
    {
      Simple_Event_Handler handler (events,
                                    0);

      result =
        reactor->register_handler (handler.pipe_.read_handle (),
                                   &handler,
                                   ACE_Event_Handler::READ_MASK);
      if (result != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe handler read")));
      else
      {
        ACE_Event_Handler *result_handler = 0;

        result =
          reactor->handler (handler.pipe_.read_handle (),
                            ACE_Event_Handler::READ_MASK,
                            &result_handler);
        ACE_Event_Handler_var safe_result_handler (result_handler);
        if (result != 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Looking up pipe read handler")));
        else
          if (result_handler != &handler)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Mismatch: result_handler %@ should be %@\n"),
                        result_handler, &handler));
      }

      {
        ACE_Event_Handler *result_handler = 0;

        result =
          reactor->handler (handler.pipe_.read_handle (),
                            ACE_Event_Handler::WRITE_MASK,
                            &result_handler);
        ACE_Event_Handler_var safe_result_handler (result_handler);
        if (result != -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("line %l handler() suceeded but shouldn't\n")));
      }

      {
        ACE_Event_Handler_var result_handler =
          reactor->find_handler (handler.pipe_.read_handle ());
        if (result_handler.handler () != &handler)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Mismatch: line %l: result_handler.handler %@ ")
                      ACE_TEXT ("should be %@\n"),
                      result_handler.handler (), &handler));
      }

      result =
        reactor->remove_handler (handler.pipe_.read_handle (),
                                 ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL);
      if (result != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l: %p\n"),
                    ACE_TEXT ("remove_handler")));
    }

  if (test_io)
    {
      Simple_Event_Handler *handler =
        new Simple_Event_Handler (events,
                                  2);

      if (-1 == reactor->register_handler (handler->pipe_.read_handle (),
                                           handler,
                                           ACE_Event_Handler::READ_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe read")));
      else
        ++events;

      if (-1 == reactor->register_handler (handler->pipe_.write_handle (),
                                           handler,
                                           ACE_Event_Handler::WRITE_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register pipe write")));
      else
        events++;
    }

  if (test_timers)
    {
      Simple_Event_Handler *handler =
        new Simple_Event_Handler (events,
                                  1);

      long timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        if ((result = reactor->cancel_timer (timer_id, 0, 0)) != 1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("cancel_timer returned %d; should be 1\n"),
                      result));
    }

  if (test_timers)
    {
      Simple_Event_Handler *handler =
        new Simple_Event_Handler (events,
                                  1);

      long timer_id =
        reactor->schedule_timer (handler,
                                 one_second_timeout,
                                 one_second,
                                 one_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events += 2;  // Wait for the scheduled and one repeating

      ACE_Time_Value const two_second (2);
      timer_id =
        reactor->schedule_timer (handler,
                                 two_second_timeout,
                                 two_second);
      if (timer_id == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("schedule_timer")));
      else
        events++;
    }

  while (events > 0)
    {
      result =
        reactor->handle_events ();
    }
}

void
simple (ACE_Reactor_Impl *impl)
{
  ACE_Reactor reactor (impl, 1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nTesting Simple Event Handler....\n\n")));

  simple_event_handler (&reactor);
}

class Closed_In_Upcall_Event_Handler : public ACE_Event_Handler
{
public:

  Closed_In_Upcall_Event_Handler (int &events);

  ~Closed_In_Upcall_Event_Handler (void);

  int handle_input (ACE_HANDLE);

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask);

  ACE_Event_Handler::Reference_Count add_reference (void);

  ACE_Event_Handler::Reference_Count remove_reference (void);

  ACE_Pipe pipe_;

  int &events_;

};

Closed_In_Upcall_Event_Handler::Closed_In_Upcall_Event_Handler (int &events)
  : events_ (events)
{
  if (-1 == this->pipe_.open ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Closed_In_Upcall_Event_Handler pipe open")));

  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Closed_In_Upcall_Event_Handler()\n")));
}

Closed_In_Upcall_Event_Handler::~Closed_In_Upcall_Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~Closed_In_Upcall_Event_Handler()\n")));

  this->pipe_.close ();
}

int
Closed_In_Upcall_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Closed_In_Upcall_Event_Handler::handle_input()\n")));

  this->events_--;

  if (0 != this->reactor ()->remove_handler (this->pipe_.read_handle (),
                                             ACE_Event_Handler::ALL_EVENTS_MASK))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("line %l %p\n"),
                ACE_TEXT ("remove_handler")));

  char buf[message_size + 1];

  ssize_t recv_result =
    ACE::recv_n (this->pipe_.read_handle (),
                 buf,
                 sizeof buf - 1);

  if (recv_result != message_size)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("line %l recv_n got %b; should be %d\n"),
                recv_result, message_size));

  buf[message_size] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message received: %C\n"),
              buf));

  return 0;
}

int
Closed_In_Upcall_Event_Handler::handle_close (ACE_HANDLE handle,
                                              ACE_Reactor_Mask masks)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Closed_In_Upcall_Event_Handler::handle_close() ")
              ACE_TEXT ("called with handle = %d and masks = %d. ")
              ACE_TEXT ("Reference count is %d\n"),
              handle,
              masks,
              this->reference_count_.value ()));

  return 0;
}

ACE_Event_Handler::Reference_Count
Closed_In_Upcall_Event_Handler::add_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::add_reference ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference count after add_reference() is %d\n"),
              this->reference_count_.value ()));

  return reference_count;
}

ACE_Event_Handler::Reference_Count
Closed_In_Upcall_Event_Handler::remove_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::remove_reference ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference count after remove_reference() is %d\n"),
              reference_count));

  return reference_count;
}

void
closed_in_upcall_event_handler (ACE_Reactor *reactor)
{
  int events = 0;
  int handle_events_result = 0;

  if (test_io)
    {
      Closed_In_Upcall_Event_Handler *handler = 0;
      ACE_NEW (handler, Closed_In_Upcall_Event_Handler (events));

      ACE_Event_Handler_var safe_handler (handler);

      ssize_t send_n_result =
        ACE::send_n (handler->pipe_.write_handle (),
                     message,
                     message_size);

      if (send_n_result != message_size)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l send_n sent %b; should be %d\n"),
                    send_n_result, message_size));

      if (-1 == reactor->register_handler (handler->pipe_.read_handle (),
                                           handler,
                                           ACE_Event_Handler::READ_MASK))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("line %l %p\n"),
                    ACE_TEXT ("register_handler")));
      else
        events += 1;
    }

  while (events > 0)
    {
      handle_events_result =
        reactor->handle_events ();
    }
}

void
closed_in_upcall (ACE_Reactor_Impl *impl)
{
  ACE_Reactor reactor (impl, 1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nTesting Closed in Upcall Event Handler....\n\n")));

  closed_in_upcall_event_handler (&reactor);
}

template <class REACTOR_IMPLEMENTATION>
class test
{
public:
  test (void);
};

template <class REACTOR_IMPLEMENTATION>
test<REACTOR_IMPLEMENTATION>::test (void)
{
  if (test_simple_event_handler)
    simple (new REACTOR_IMPLEMENTATION);

  if (test_reference_counted_event_handler_1)
    reference_count_1 (new REACTOR_IMPLEMENTATION);

  if (test_reference_counted_event_handler_2)
    reference_count_2 (new REACTOR_IMPLEMENTATION);

  if (test_closed_in_upcall_event_handler)
    closed_in_upcall (new REACTOR_IMPLEMENTATION);
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:d:f:g:h:i:k:l:m:z:"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
      switch (cc)
        {
        case 'a':
          test_select_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          test_tp_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'c':
          test_wfmo_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          test_dev_poll_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'f':
          test_simple_event_handler = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'g':
          test_reference_counted_event_handler_1 = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'h':
          test_reference_counted_event_handler_2 = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'i':
          test_closed_in_upcall_event_handler = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'k':
          test_io = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          test_timers = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'm':
          test_find = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'z':
          debug = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case '?':
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a test Select Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-b test TP Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-c test WFMO Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-d test Dev Poll Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-f test simple event handler] (defaults to %d)\n")
                      ACE_TEXT ("\t[-g test reference counted event handler (first test)] (defaults to %d)\n")
                      ACE_TEXT ("\t[-h test reference counted event handler (second test)] (defaults to %d)\n")
                      ACE_TEXT ("\t[-i test closed in upcall event handler] (defaults to %d)\n")
                      ACE_TEXT ("\t[-k test io] (defaults to %d)\n")
                      ACE_TEXT ("\t[-l test timers] (defaults to %d)\n")
                      ACE_TEXT ("\t[-m test find] (defaults to %d)\n")
                      ACE_TEXT ("\t[-z debug] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      test_select_reactor,
                      test_tp_reactor,
                      test_wfmo_reactor,
                      test_dev_poll_reactor,
                      test_simple_event_handler,
                      test_reference_counted_event_handler_1,
                      test_reference_counted_event_handler_2,
                      test_closed_in_upcall_event_handler,
                      test_io,
                      test_timers,
                      test_find,
                      debug));
          return -1;
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Reference_Counted_Event_Handler_Test"));

  // Validate options.
  int result =
    parse_args (argc, argv);
  if (result != 0)
    return result;

  if (test_select_reactor)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\nTesting Select Reactor....\n\n")));

      test<ACE_Select_Reactor> test;
      ACE_UNUSED_ARG (test);
    }

  if (test_tp_reactor)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\nTesting TP Reactor....\n\n")));

      test<ACE_TP_Reactor> test;
      ACE_UNUSED_ARG (test);
    }

#if defined (ACE_WIN32) && \
    (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))

  if (test_wfmo_reactor)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\nTesting WFMO Reactor....\n\n")));

      test<ACE_WFMO_Reactor> test;
      ACE_UNUSED_ARG (test);
    }

#endif /* ACE_WIN32 && ACE_HAS_WINSOCK2 */

#if defined (ACE_HAS_DEV_POLL) || defined (ACE_HAS_EVENT_POLL)

  if (test_dev_poll_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\nTesting ACE_Dev_Poll_Reactor....\n\n")));

      test<ACE_Dev_Poll_Reactor> test;
      ACE_UNUSED_ARG (test);
    }

#endif /* ACE_HAS_DEV_POLL || ACE_HAS_EVENT_POLL */

  ACE_END_TEST;

  return result;
}

