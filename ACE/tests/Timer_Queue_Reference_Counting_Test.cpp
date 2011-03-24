
//=============================================================================
/**
 *  @file    Timer_Queue_Reference_Counting_Test.cpp
 *
 *  $Id$
 *
 *  This test is used to check reference counting of the Event
 *  Handler when it interacts with Timer Queues.
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_Wheel.h"
#include "ace/Reactor.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_unistd.h"



static int debug = 0;
static const char *one_second_timeout = "one second timeout";
static const char *two_second_timeout = "two second timeout";

namespace
{
  inline void WAIT_FOR_NEXT_EVENT (ACE_Timer_Queue &timer_queue)
  {
    ACE_Time_Value const earliest_time = timer_queue.earliest_time ();
    ACE_Time_Value const time_of_day =   timer_queue.gettimeofday ();
    if (earliest_time > time_of_day)
      {
        ACE_OS::sleep (earliest_time - time_of_day);
      }
  }
}

class Reference_Counted_Event_Handler : public ACE_Event_Handler
{
public:

  Reference_Counted_Event_Handler (int expected_number_of_handle_close_calls);

  ~Reference_Counted_Event_Handler (void);

  int handle_timeout (const ACE_Time_Value &,
                      const void *);

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask masks);

  int expected_number_of_handle_close_calls_;
  int number_of_handle_close_calls_;
};

Reference_Counted_Event_Handler::Reference_Counted_Event_Handler (int expected_number_of_handle_close_calls)
  : expected_number_of_handle_close_calls_ (expected_number_of_handle_close_calls),
    number_of_handle_close_calls_ (0)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Reference count in Reference_Counted_Event_Handler() is %d\n",
                this->reference_count_.value ()));
}

Reference_Counted_Event_Handler::~Reference_Counted_Event_Handler (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Reference count in ~Reference_Counted_Event_Handler() is %d\n",
                this->reference_count_.value ()));

  if (this->expected_number_of_handle_close_calls_ != -1)
    ACE_TEST_ASSERT (this->number_of_handle_close_calls_ ==
                this->expected_number_of_handle_close_calls_);
}

int
Reference_Counted_Event_Handler::handle_timeout (const ACE_Time_Value &,
                                                 const void *arg)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Reference count in Reference_Counted_Event_Handler::handle_timeout() for arg = %C is %d\n",
                (const char *) arg,
                this->reference_count_.value ()));

  return 0;
}

int
Reference_Counted_Event_Handler::handle_close (ACE_HANDLE handle,
                                               ACE_Reactor_Mask masks)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Reference_Counted_Event_Handler::handle_close() called with handle = %d and masks = %d. "
                "Reference count is %d\n",
                handle,
                masks,
                this->reference_count_.value ()));

  ++this->number_of_handle_close_calls_;

  return 0;
}

void
cancellation (ACE_Timer_Queue &timer_queue,
              int repeat_timer,
              int cancel_handler,
              int second_timer,
              int dont_call_handle_close)
{
  int result = 0;

  int expected_number_of_handle_close_calls = -1;

  if (!dont_call_handle_close)
    {
      if (cancel_handler)
        expected_number_of_handle_close_calls = 1;
      else if (second_timer)
        expected_number_of_handle_close_calls = 2;
      else
        expected_number_of_handle_close_calls = 1;
    }

  Reference_Counted_Event_Handler *handler =
    new Reference_Counted_Event_Handler (expected_number_of_handle_close_calls);

  ACE_Event_Handler_var safe_handler (handler);

  long first_timer_id = -1;
  long second_timer_id = -1;

  if (repeat_timer)
    {
      first_timer_id =
        timer_queue.schedule (handler,
                              one_second_timeout,
                              ACE_Time_Value (1) + timer_queue.gettimeofday (),
                              ACE_Time_Value (1));
      ACE_TEST_ASSERT (first_timer_id != -1);
    }
  else
    {
      first_timer_id =
        timer_queue.schedule (handler,
                              one_second_timeout,
                              ACE_Time_Value (1) + timer_queue.gettimeofday ());
      ACE_TEST_ASSERT (first_timer_id != -1);
    }

  if (second_timer)
    {
      second_timer_id =
        timer_queue.schedule (handler,
                              two_second_timeout,
                              ACE_Time_Value (2) + timer_queue.gettimeofday (),
                              ACE_Time_Value (2));
      ACE_TEST_ASSERT (second_timer_id != -1);
    }

  if (cancel_handler)
    {
      result =
        timer_queue.cancel (handler,
                            dont_call_handle_close);

      if (second_timer)
        ACE_TEST_ASSERT (result == 2);
      else
        ACE_TEST_ASSERT (result == 1);
    }
  else
    {
      result =
        timer_queue.cancel (first_timer_id,
                            0,
                            dont_call_handle_close);
      ACE_TEST_ASSERT (result == 1);

      if (second_timer)
        {
          result =
            timer_queue.cancel (second_timer_id,
                                0,
                                dont_call_handle_close);
          ACE_TEST_ASSERT (result == 1);
        }
    }
}

template <class TIMER_QUEUE>
class cancellation_test
{
public:
  cancellation_test (const char *);
};

template <class TIMER_QUEUE>
cancellation_test<TIMER_QUEUE>::cancellation_test (const char *timer_queue_type)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nCancellation test for %C\n\n",
              timer_queue_type));

  int configs[][4] = {
    { 0, 0, 0, 0, },
    { 0, 0, 0, 1, },
    { 0, 0, 1, 0, },
    { 0, 0, 1, 1, },
    { 0, 1, 0, 0, },
    { 0, 1, 0, 1, },
    { 0, 1, 1, 0, },
    { 0, 1, 1, 1, },
    { 1, 0, 0, 0, },
    { 1, 0, 0, 1, },
    { 1, 0, 1, 0, },
    { 1, 0, 1, 1, },
    { 1, 1, 0, 0, },
    { 1, 1, 0, 1, },
    { 1, 1, 1, 0, },
    { 1, 1, 1, 1, },
  };

  for (int i = 0;
       i < (int) (sizeof configs / (sizeof (int) * 5));
       i++)
    {
      TIMER_QUEUE timer_queue;

      cancellation (timer_queue,
                    configs[i][0],
                    configs[i][1],
                    configs[i][2],
                    configs[i][3]);
    }
}

typedef int (*Expire_Function) (ACE_Timer_Queue &timer_queue);

int
invoke_expire (ACE_Timer_Queue &timer_queue)
{
  return timer_queue.expire ();
}

int
invoke_one_upcall (ACE_Timer_Queue &timer_queue)
{
  // Get the current time
  ACE_Time_Value current_time (timer_queue.gettimeofday () +
                               timer_queue.timer_skew ());

  // Look for a node in the timer queue whose timer <= the present
  // time.
  ACE_Timer_Node_Dispatch_Info dispatch_info;

  if (timer_queue.dispatch_info (current_time,
                                 dispatch_info))
    {
      const void *upcall_act = 0;

      // Preinvoke.
      timer_queue.preinvoke (dispatch_info,
                             current_time,
                             upcall_act);

      // Call the functor
      timer_queue.upcall (dispatch_info,
                          current_time);

      // Postinvoke
      timer_queue.postinvoke (dispatch_info,
                              current_time,
                              upcall_act);

      // We have dispatched a timer
      return 1;
    }

  return 0;
}

void
expire (ACE_Timer_Queue &timer_queue,
        Expire_Function expire_function)
{
  int events = 0;
  int result = 0;

  Reference_Counted_Event_Handler *handler =
    new Reference_Counted_Event_Handler (1);

  ACE_Event_Handler_var safe_handler (handler);

  long timer_id =
    timer_queue.schedule (handler,
                          one_second_timeout,
                          ACE_Time_Value (1) + timer_queue.gettimeofday (),
                          ACE_Time_Value (1));
  ACE_TEST_ASSERT (timer_id != -1);

  result =
    timer_queue.schedule (handler,
                          two_second_timeout,
                          ACE_Time_Value (2) + timer_queue.gettimeofday ());
  ACE_TEST_ASSERT (result != -1);

  events += 4;

  for (int i = 0; i < events;)
    {
      WAIT_FOR_NEXT_EVENT (timer_queue);

      result =
        expire_function (timer_queue);

      ACE_TEST_ASSERT (result >= 0);

      i += result;
    }

  timer_queue.cancel (timer_id, 0, 0);
}

template<class TIMER_QUEUE>
class expire_test
{
public:
  expire_test (const char *);
};

template <class TIMER_QUEUE>
expire_test<TIMER_QUEUE>::expire_test (const char *timer_queue_type)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nExpire test for %C\n\n",
              timer_queue_type));

  TIMER_QUEUE timer_queue;

  expire (timer_queue,
          invoke_expire);
}

template<class TIMER_QUEUE>
class upcall_test
{
public:
  upcall_test (const char *);
};

template <class TIMER_QUEUE>
upcall_test<TIMER_QUEUE>::upcall_test (const char *timer_queue_type)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nOne upcall test for %C\n\n",
              timer_queue_type));

  TIMER_QUEUE timer_queue;

  expire (timer_queue,
          invoke_one_upcall);
}

class Simple_Event_Handler : public ACE_Event_Handler
{
public:

  Simple_Event_Handler (void);

  ~Simple_Event_Handler (void);

  int handle_timeout (const ACE_Time_Value &,
                      const void *);

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask);
};

Simple_Event_Handler::Simple_Event_Handler (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Simple_Event_Handler()\n"));
}

Simple_Event_Handler::~Simple_Event_Handler (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "~Simple_Event_Handler()\n"));
}

int
Simple_Event_Handler::handle_timeout (const ACE_Time_Value &,
                                      const void *arg)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Simple_Event_Handler::handle_timeout() for arg = %C\n",
                (const char *) arg));
  return 0;
}

int
Simple_Event_Handler::handle_close (ACE_HANDLE handle,
                                    ACE_Reactor_Mask masks)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "Simple_Event_Handler::handle_close() called with handle = %d and masks = %d.\n",
                handle,
                masks));

  delete this;

  return 0;
}

void
simple (ACE_Timer_Queue &timer_queue)
{
  int events = 0;
  int result = 0;
  long timer_id = -1;

  {
    Simple_Event_Handler *handler =
      new Simple_Event_Handler;

    timer_id =
      timer_queue.schedule (handler,
                            one_second_timeout,
                            ACE_Time_Value (1) + timer_queue.gettimeofday (),
                            ACE_Time_Value (1));
    ACE_TEST_ASSERT (timer_id != -1);

    result =
      timer_queue.cancel (timer_id,
                          0,
                          0);
    ACE_TEST_ASSERT (result == 1);
  }

  {
    Simple_Event_Handler *handler =
      new Simple_Event_Handler;

    timer_id =
      timer_queue.schedule (handler,
                            one_second_timeout,
                            ACE_Time_Value (1) + timer_queue.gettimeofday (),
                            ACE_Time_Value (1));
    ACE_TEST_ASSERT (timer_id != -1);

    events += 3;
  }

  for (int i = 0; i < events;)
    {
      WAIT_FOR_NEXT_EVENT (timer_queue);

      result =
        timer_queue.expire ();

      ACE_TEST_ASSERT (result >= 0);

      i += result;
    }

  timer_queue.cancel (timer_id, 0, 0);
}

template <class TIMER_QUEUE>
class simple_test
{
public:
  simple_test (const char *);
};

template <class TIMER_QUEUE>
simple_test<TIMER_QUEUE>::simple_test (const char *timer_queue_type)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nSimple test for %C\n\n",
              timer_queue_type));

  TIMER_QUEUE timer_queue;

  simple (timer_queue);
}

static int heap = 1;
static int list = 1;
static int hash = 1;
static int wheel = 1;
static int hashheap = 1;
static int test_cancellation = 1;
static int test_expire = 1;
static int test_one_upcall = 1;
static int test_simple = 1;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:d:e:l:m:n:o:z:"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
      switch (cc)
        {
        case 'a':
          heap = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          list = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'c':
          hash = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          wheel = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'e':
          hashheap = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          test_cancellation = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'm':
          test_expire = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'n':
          test_one_upcall = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'o':
          test_simple = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'z':
          debug = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a heap]  (defaults to %d)\n")
                      ACE_TEXT ("\t[-b list]  (defaults to %d)\n")
                      ACE_TEXT ("\t[-c hash]  (defaults to %d)\n")
                      ACE_TEXT ("\t[-d wheel] (defaults to %d)\n")
                      ACE_TEXT ("\t[-e hashheap] (defaults to %d)\n")
                      ACE_TEXT ("\t[-l test_cancellation] (defaults to %d)\n")
                      ACE_TEXT ("\t[-m test_expire] (defaults to %d)\n")
                      ACE_TEXT ("\t[-n test_one_upcall] (defaults to %d)\n")
                      ACE_TEXT ("\t[-o test_simple] (defaults to %d)\n")
                      ACE_TEXT ("\t[-z debug] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      heap,
                      list,
                      hash,
                      wheel,
                      hashheap,
                      test_cancellation,
                      test_expire,
                      test_one_upcall,
                      test_simple,
                      debug));
          return -1;
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Timer_Queue_Reference_Counting_Test"));

  // Validate options.
  int result =
    parse_args (argc, argv);
  if (result != 0)
    return result;

  if (test_cancellation)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nCancellation test...\n\n"));

      if (heap)  { cancellation_test<ACE_Timer_Heap>  test ("ACE_Timer_Heap");  ACE_UNUSED_ARG (test); }
      if (list)  { cancellation_test<ACE_Timer_List>  test ("ACE_Timer_List");  ACE_UNUSED_ARG (test); }
      if (hash)  { cancellation_test<ACE_Timer_Hash>  test ("ACE_Timer_Hash");  ACE_UNUSED_ARG (test); }
      if (wheel) { cancellation_test<ACE_Timer_Wheel> test ("ACE_Timer_Wheel"); ACE_UNUSED_ARG (test); }
      if (hashheap) { cancellation_test<ACE_Timer_Hash_Heap> test ("ACE_Timer_Hash_Heap"); ACE_UNUSED_ARG (test); }
    }

  if (test_expire)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nExpire test...\n\n"));

      if (heap)  { expire_test<ACE_Timer_Heap>  test ("ACE_Timer_Heap");  ACE_UNUSED_ARG (test); }
      if (list)  { expire_test<ACE_Timer_List>  test ("ACE_Timer_List");  ACE_UNUSED_ARG (test); }
      if (hash)  { expire_test<ACE_Timer_Hash>  test ("ACE_Timer_Hash");  ACE_UNUSED_ARG (test); }
      if (wheel) { expire_test<ACE_Timer_Wheel> test ("ACE_Timer_Wheel"); ACE_UNUSED_ARG (test); }
      if (hashheap) { expire_test<ACE_Timer_Hash_Heap> test ("ACE_Timer_Hash_Heap"); ACE_UNUSED_ARG (test); }
    }

  if (test_one_upcall)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nOne upcall at a time test...\n\n"));

      if (heap)  { upcall_test<ACE_Timer_Heap>  test ("ACE_Timer_Heap");  ACE_UNUSED_ARG (test); }
      if (list)  { upcall_test<ACE_Timer_List>  test ("ACE_Timer_List");  ACE_UNUSED_ARG (test); }
      if (hash)  { upcall_test<ACE_Timer_Hash>  test ("ACE_Timer_Hash");  ACE_UNUSED_ARG (test); }
      if (wheel) { upcall_test<ACE_Timer_Wheel> test ("ACE_Timer_Wheel"); ACE_UNUSED_ARG (test); }
      if (hashheap) { upcall_test<ACE_Timer_Hash_Heap> test ("ACE_Timer_Hash_Heap"); ACE_UNUSED_ARG (test); }
    }

  if (test_simple)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nSimple test...\n\n"));

      if (heap)  { simple_test<ACE_Timer_Heap>  test ("ACE_Timer_Heap");  ACE_UNUSED_ARG (test); }
      if (list)  { simple_test<ACE_Timer_List>  test ("ACE_Timer_List");  ACE_UNUSED_ARG (test); }
      if (hash)  { simple_test<ACE_Timer_Hash>  test ("ACE_Timer_Hash");  ACE_UNUSED_ARG (test); }
      if (wheel) { simple_test<ACE_Timer_Wheel> test ("ACE_Timer_Wheel"); ACE_UNUSED_ARG (test); }
      if (hashheap) { simple_test<ACE_Timer_Hash_Heap> test ("ACE_Timer_Hash_Heap"); ACE_UNUSED_ARG (test); }
    }

  ACE_END_TEST;

  return 0;
}

