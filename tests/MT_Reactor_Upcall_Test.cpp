// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MT_Reactor_Upcall_Test.cpp
//
// = DESCRIPTION
//    This is a test that shows how to handle upcalls from the
//    TP_Reactor and the WFMO_Reactor when the event loop is being run
//    by multiple threads.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Pipe.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, MT_Reactor_Upcall_Test, "$Id$")

int number_of_event_loop_threads = 3;
int number_of_messages = 10;
int sleep_time_in_msec = 100;
int lock_upcall = 1;
static const char *message =
"Hello there!";

class Guard
{
public:
  Guard (ACE_SYNCH_MUTEX &lock)
    : lock_ (lock)
  {
    if (lock_upcall)
      lock.acquire ();
  }

  ~Guard (void)
  {
    if (lock_upcall)
      this->lock_.release ();
  }

  ACE_SYNCH_MUTEX &lock_;
};

struct Message
{
  enum Type
  {
    DATA,
    SHUTDOWN
  };

  Type type_;
  size_t size_;
  char data_[BUFSIZ];
};

class Handler : public ACE_Event_Handler
{
public:
  Handler (ACE_Reactor &reactor);
  int handle_input (ACE_HANDLE fd);

  ACE_Pipe pipe_;
  int number_of_messages_read_;
  ACE_SYNCH_MUTEX lock_;
  int shutdown_;
};

Handler::Handler (ACE_Reactor &reactor)
  : ACE_Event_Handler (&reactor),
    number_of_messages_read_ (0),
    shutdown_ (0)
{
  // Create the pipe.
  int result
    = this->pipe_.open ();
  ACE_ASSERT (result == 0);

  // Register for input events.
  result =
    this->reactor ()->register_handler (this->pipe_.read_handle (),
                                        this,
                                        ACE_Event_Handler::READ_MASK);
  ACE_ASSERT (result == 0);
}

int
Handler::handle_input (ACE_HANDLE fd)
{
  Guard monitor (this->lock_);

  // If we have been shutdown, return.
  if (this->shutdown_)
    return 0;

  // Read fixed part of message.
  Message message;
  size_t fixed_size_of_message =
    sizeof Message::Type + sizeof size_t;

  ssize_t result =
    ACE::recv_n (fd,
                 &message,
                 fixed_size_of_message);
  ACE_ASSERT (result == ssize_t (fixed_size_of_message));

  // On shutdown message, stop the event loop.
  if (message.type_ == Message::SHUTDOWN)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Shutdown message\n"));

      this->shutdown_ = 1;

      this->reactor ()->end_reactor_event_loop ();

      // Remove self from Reactor.
      return -1;
    }

  // Else it is a data message: read the data.
  result =
    ACE::recv_n (fd,
                 &message.data_,
                 message.size_);
  ACE_ASSERT (result == ssize_t (message.size_));

  message.data_[result] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Starting to handle message %d: %s\n",
              this->number_of_messages_read_ + 1,
              message.data_));

  // Process message (sleep).
  ACE_OS::sleep (ACE_Time_Value (0,
                                 sleep_time_in_msec * 1000));

  // Keep count.
  this->number_of_messages_read_++;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Completed handling message %d\n",
              this->number_of_messages_read_));

  return 0;
}

class Event_Loop_Task : public ACE_Task_Base
{
public:
  Event_Loop_Task (ACE_Reactor &reactor);
  int svc (void);

private:
  ACE_Reactor &reactor_;
};

Event_Loop_Task::Event_Loop_Task (ACE_Reactor &reactor)
  : reactor_ (reactor)
{
}

int
Event_Loop_Task::svc (void)
{
  return this->reactor_.run_reactor_event_loop ();
}

void
test_reactor_upcall (ACE_Reactor &reactor)
{
  Handler handler (reactor);
  Event_Loop_Task event_loop_task (reactor);

  // Start up the event loop threads.
  int result =
    event_loop_task.activate (THR_NEW_LWP | THR_JOINABLE,
                              number_of_event_loop_threads);
  ACE_ASSERT (result == 0);

  // Data message.
  Message data_message;
  data_message.type_ =
    Message::DATA;
  data_message.size_ =
    ACE_OS::strlen (message);
  ACE_OS::strcpy (data_message.data_,
                  message);

  size_t size_of_data_message =
    sizeof Message::Type + sizeof size_t + data_message.size_;

  for (int i = 0;
       i < number_of_messages;
       ++i)
    {
      // This should trigger a call to <handle_input>.
      result =
        ACE::send_n (handler.pipe_.write_handle (),
                     &data_message,
                     size_of_data_message);
      ACE_ASSERT (result == ssize_t (size_of_data_message));
    }

  // We are done: send shutdown message.
  Message shutdown_message;
  shutdown_message.type_ =
    Message::SHUTDOWN;
  shutdown_message.size_ = 0;

  size_t size_of_shutdown_message =
    sizeof Message::Type + sizeof size_t;

  // This should trigger a call to <handle_input>.
  result =
    ACE::send_n (handler.pipe_.write_handle (),
                 &shutdown_message,
                 size_of_shutdown_message);
  ACE_ASSERT (result == ssize_t (size_of_shutdown_message));

  // Wait for the event loop tasks to exit.
  event_loop_task.wait ();
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("t:m:s:l:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 't':
        number_of_event_loop_threads =
          ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'm':
        number_of_messages =
          ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's':
        sleep_time_in_msec =
          ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'l':
        lock_upcall =
          ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-m <number of messages> (defaults to %d)\n"
                           "\t-t <number of event loop threads> (defaults to %d)\n"
                           "\t-s <sleep time in msec> (defaults to %d)\n"
                           "\t-l <lock upcall> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           number_of_messages,
                           number_of_event_loop_threads,
                           sleep_time_in_msec,
                           lock_upcall),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reactor_Upcall_Test"));

#if defined (ACE_HAS_THREADS)

  // ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  int result =
    parse_args (argc, argv);

  if (result != 0)
    return result;

  ACE_DEBUG ((LM_DEBUG,
              "\nTesting TP Reactor\n\n"));

  ACE_TP_Reactor tp_reactor_impl;
  ACE_Reactor tp_reactor (&tp_reactor_impl);

  test_reactor_upcall (tp_reactor);

#if defined (ACE_WIN32)

  ACE_DEBUG ((LM_DEBUG,
              "\nTesting WFMO Reactor\n\n"));

  ACE_WFMO_Reactor wfmo_reactor_impl;
  ACE_Reactor wfmo_reactor (&wfmo_reactor_impl);

  test_reactor_upcall (wfmo_reactor);

#endif /* ACE_WIN32 */

#else /* ACE_HAS_THREADS */
  ACE_UNUSED_ARG(argc);
  ACE_UNUSED_ARG(argv);

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
