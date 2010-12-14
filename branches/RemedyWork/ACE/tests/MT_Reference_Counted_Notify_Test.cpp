// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MT_Reference_Counted_Notify_Test.cpp
//
// = DESCRIPTION
//    This test is used to check reference counting of the event
//    handlers when it interacts with the reactor notification
//    mechanism.
//
// = AUTHOR
//    Irfan Pyarali <irfan@oomworks.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"



#if defined (ACE_HAS_THREADS)

static int test_select_reactor = 1;
static int test_tp_reactor = 1;
static int test_wfmo_reactor = 1;
static int test_dev_poll_reactor = 1;
static int test_empty_notify = 1;
static int test_simple_notify = 1;
static int test_reference_counted_notify = 1;
static int iterations = 5;
static int debug = 1;

class Reference_Counted_Event_Handler : public ACE_Event_Handler
{
public:

  Reference_Counted_Event_Handler (void);

  ~Reference_Counted_Event_Handler (void);

  int handle_input (ACE_HANDLE);

  ACE_Event_Handler::Reference_Count add_reference (void);

  ACE_Event_Handler::Reference_Count remove_reference (void);

};

Reference_Counted_Event_Handler::Reference_Counted_Event_Handler (void)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  if (debug)
    ACE_DEBUG
      ((LM_DEBUG,
        ACE_TEXT ("Reference count in Reference_Counted_Event_Handler() ")
        ACE_TEXT ("is %d\n"),
        this->reference_count_.value ()));
}

Reference_Counted_Event_Handler::~Reference_Counted_Event_Handler (void)
{
  if (debug)
    ACE_DEBUG
      ((LM_DEBUG,
        ACE_TEXT ("Reference count in ~Reference_Counted_Event_Handler() ")
        ACE_TEXT ("is %d\n"),
        this->reference_count_.value ()));

  if (0 != this->reference_count_.value ())
    ACE_ERROR
      ((LM_ERROR,
        ACE_TEXT ("Reference count in ~Reference_Counted_Event_Handler() ")
        ACE_TEXT ("should be 0 but is %d\n"),
        this->reference_count_.value ()));
}

int
Reference_Counted_Event_Handler::handle_input (ACE_HANDLE)
{
  if (debug)
    ACE_DEBUG
      ((LM_DEBUG,
        ACE_TEXT ("Reference count in Reference_Counted_Event_Handler::")
        ACE_TEXT ("handle_input() is %d\n"),
        this->reference_count_.value ()));

  if (2 != this->reference_count_.value ())
    ACE_ERROR
      ((LM_ERROR,
        ACE_TEXT ("Reference count in Reference_Counted_Event_Handler::")
        ACE_TEXT ("handle_input() should be 2 but is %d\n"),
        this->reference_count_.value ()));

  return 0;
}

ACE_Event_Handler::Reference_Count
Reference_Counted_Event_Handler::add_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::add_reference ();

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Reference count after add_reference() is %d\n"),
                this->reference_count_.value ()));

  return reference_count;
}

ACE_Event_Handler::Reference_Count
Reference_Counted_Event_Handler::remove_reference (void)
{
  ACE_Event_Handler::Reference_Count reference_count =
    this->ACE_Event_Handler::remove_reference ();

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Reference count after remove_reference() is %d\n"),
                reference_count));

  return reference_count;
}

class Simple_Event_Handler : public ACE_Event_Handler
{
public:

  Simple_Event_Handler (int notifies);

  ~Simple_Event_Handler (void);

  int handle_input (ACE_HANDLE);

  int notifies_;
};

Simple_Event_Handler::Simple_Event_Handler (int notifies)
  : notifies_ (notifies)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Event_Handler()\n")));
}

Simple_Event_Handler::~Simple_Event_Handler (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~Simple_Event_Handler()\n")));
}

int
Simple_Event_Handler::handle_input (ACE_HANDLE)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Simple_Event_Handler::handle_input()\n")));

  this->notifies_--;

  if (this->notifies_ == 0)
    delete this;

  return 0;
}

class Event_Loop_Thread : public ACE_Task_Base
{
public:

  Event_Loop_Thread (ACE_Thread_Manager &thread_manager,
                     ACE_Reactor &reactor,
                     int extra_iterations_needed);

  int svc (void);

  ACE_Reactor &reactor_;

  int extra_iterations_needed_;
};

Event_Loop_Thread::Event_Loop_Thread (ACE_Thread_Manager &thread_manager,
                                      ACE_Reactor &reactor,
                                      int extra_iterations_needed)
  : ACE_Task_Base (&thread_manager),
    reactor_ (reactor),
    extra_iterations_needed_ (extra_iterations_needed)
{
}

int
Event_Loop_Thread::svc (void)
{
  int counter = 0;

  // Simply run the event loop.
  this->reactor_.owner (ACE_Thread::self ());

  while (1)
    {
      counter++;

      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Event Loop iteration %d....\n"),
                    counter));

      this->reactor_.handle_events ();

      if (counter ==
          iterations + this->extra_iterations_needed_)
        break;
    }

  return 0;
}

void
notify (ACE_Reactor &reactor,
        ACE_Event_Handler *event_handler,
        int extra_iterations_needed)
{
  ACE_Thread_Manager thread_manager;

  // Create a thread to run the event loop.
  Event_Loop_Thread event_loop_thread (thread_manager,
                                       reactor,
                                       extra_iterations_needed);

  int result =
    event_loop_thread.activate ();
  ACE_TEST_ASSERT (result == 0);

  for (int i = 0;
       i < iterations;
       ++i)
    {
      ACE_OS::sleep (ACE_Time_Value (0, 500 * 1000));

      result = reactor.notify (event_handler,
                               ACE_Event_Handler::READ_MASK);

      ACE_TEST_ASSERT (result == 0);
    }

  thread_manager.wait ();
}

template <class REACTOR_IMPLEMENTATION>
class test
{
public:
  test (int extra_iterations_needed);
};

template <class REACTOR_IMPLEMENTATION>
test<REACTOR_IMPLEMENTATION>::test (int extra_iterations_needed)
{
  if (test_empty_notify)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\nTesting empty notifies...\n\n")));

      REACTOR_IMPLEMENTATION impl;
      ACE_Reactor reactor (&impl, 0);

      notify (reactor,
              0,
              extra_iterations_needed);
    }


  if (test_simple_notify)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\nTesting simple notifies...\n\n")));

      REACTOR_IMPLEMENTATION impl;
      ACE_Reactor reactor (&impl, 0);

      Simple_Event_Handler *simple_event_handler =
        new Simple_Event_Handler (iterations);

      notify (reactor,
              simple_event_handler,
              extra_iterations_needed);
    }

  if (test_reference_counted_notify)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\nTesting reference counted notifies...\n\n")));

      REACTOR_IMPLEMENTATION impl;
      ACE_Reactor reactor (&impl, 0);

      Reference_Counted_Event_Handler *reference_counted_event_handler =
        new Reference_Counted_Event_Handler;

      ACE_Event_Handler_var safe_event_handler (reference_counted_event_handler);

      notify (reactor,
              reference_counted_event_handler,
              extra_iterations_needed);
    }
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:d:e:f:g:z:"));

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
        case 'e':
          test_empty_notify = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'f':
          test_simple_notify = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'g':
          test_reference_counted_notify = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'z':
          debug = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a test Select Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-b test TP Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-c test WFMO Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-d test Dev Poll Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-e test empty notify] (defaults to %d)\n")
                      ACE_TEXT ("\t[-f test simple notify] (defaults to %d)\n")
                      ACE_TEXT ("\t[-g test reference counted notify] (defaults to %d)\n")
                      ACE_TEXT ("\t[-z debug] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      test_select_reactor,
                      test_tp_reactor,
                      test_wfmo_reactor,
                      test_dev_poll_reactor,
                      test_empty_notify,
                      test_simple_notify,
                      test_reference_counted_notify,
                      debug));
          return -1;
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reference_Counted_Notify_Test"));

  // Validate options.
  int result =
    parse_args (argc, argv);
  if (result != 0)
    return result;

  int extra_iterations_needed = 1;
  int extra_iterations_not_needed = 0;

  if (test_select_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting Select Reactor....\n\n"));

      test<ACE_Select_Reactor> test (extra_iterations_not_needed);
      ACE_UNUSED_ARG (test);
    }

  if (test_tp_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting TP Reactor....\n\n"));

      test<ACE_TP_Reactor> test (extra_iterations_not_needed);
      ACE_UNUSED_ARG (test);
    }

#if defined (ACE_HAS_EVENT_POLL)

  if (test_dev_poll_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting Dev Poll Reactor....\n\n"));

      test<ACE_Dev_Poll_Reactor> test (extra_iterations_not_needed);
      ACE_UNUSED_ARG (test);
    }

#endif

#if defined (ACE_WIN32)

  if (test_wfmo_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting WFMO Reactor....\n\n"));

      test<ACE_WFMO_Reactor> test (extra_iterations_needed);
      ACE_UNUSED_ARG (test);
    }

#else /* ACE_WIN32 */

  ACE_UNUSED_ARG (extra_iterations_needed);

#endif /* ACE_WIN32 */

  ACE_END_TEST;

  return 0;
}

#else /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reference_Counted_Notify_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_HAS_THREADS */
