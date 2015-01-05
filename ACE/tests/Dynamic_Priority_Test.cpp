// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Dynamic_Priority_Test.cpp (based on Priority_Buffer_Test.cpp)
//
// = DESCRIPTION
//    This is a test to verify and illustrate the static and dynamic
//    priority mechanisms of the <ACE_Message_Queue> class and the
//    <ACE_Dynamic_Message_Queue> class.  As in the
//    <Priority_Buffer_Test>, a producer generates messages and
//    enqueues them, and a consumer dequeues them and checks their
//    ordering.
//
//    In these tests, every effort is made to ensure that there is
//    plenty of time for the messages to be enqueued and dequeued,
//    with messages that *should* meet their deadlines actually
//    meeting them, while messages that should miss their deadlines
//    are delayed so that they actually miss them.  It is, however,
//    remotely possible that this test could yield a false negative:
//    the dynamic queues could work correctly but due to timing
//    variations the test could indicate failure.
//
//    Three message queues are obtained from the message queue
//    factory, one static, two dynamic (one deadline based, and one
//    laxity based) and the same supplier behavior is used each time:
//    the messages are preallocated and their static information
//    valued, the current time is obtained and deadlines are set, with
//    half of the messages given late deadlines, and the other half of
//    the messages given reachable deadlines.  The producer then
//    immediately enqueues all messages.
//
//    Two separate tests are run, one which verifies messages are
//    correctly ordered my the given queues, and one which generates
//    performance numbers for the various queues under increasing
//    numbers of messages.  In the first test, the consumer is passed
//    the filled queue and a string with the expected order in which
//    the messages should dequeue.  In the second test, measurements
//    are made as non-intrusive as possible, with no ordering checks.
//
// = AUTHOR
//    Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h" /* Include first to enable ACE_ASSERT. */
#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#if defined (VXWORKS)
# include "ace/OS_NS_string.h"
#endif /* VXWORKS */



#if defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
enum Test_Type {BEST, WORST, RANDOM};

class ArgStruct
{
  // = TITLE
  //   Structure used to pass arguments to test functions.
public:

  ACE_Message_Queue<ACE_SYNCH> *queue_;
  // message queue to test

  const char *order_string_;
  // string of characters to indicate message order

  ACE_Message_Block **array_;
  // array of message blocks to use

  u_int expected_count_;
  // expected message count
};

// Order in which messages are sent.
static const char send_order [] = "abcdefghijklmnop";

// Order in which messages are received with "FIFO prioritization"
// (i.e., none)
// Unused:  static const char FIFO_receipt_order [] = "abcdefghijklmnop";

// Order in which messages are received with static prioritization.
static const char static_receipt_order [] = "ponmlkjihgfedcba";

// Order in which messages are received with deadline prioritization.
static const char deadline_receipt_order [] = "hgfedcbaponmlkji";

// Order in which messages are received with laxity prioritization.
static const char laxity_receipt_order [] = "hfgedbcapnomljki";

// Fast and slow execution time values (sec, usec), kept very small to
// allow comparison of deadline, laxity, and static strategies across
// a very wide range of message counts.
static const ACE_Time_Value fast_execution (0, 1);
static const ACE_Time_Value slow_execution (0, 2);

// Make the queue be capable of being *very* large.
static const long max_queue = LONG_MAX;

#if defined (VXWORKS)
// VxWorks Message Queue parameters.
const int vx_max_queue = INT_MAX;
const int vx_msg_size = 32;
#endif /* defined (VXWORKS) */

// Loading parameters (number of messages to push through queues) for
// performance tests.
static int MIN_LOAD = 20;
static int MAX_LOAD = 1000;
static int LOAD_STEP = 20;

// Time offsets for a minute in the past (for the best case test) and
// two seconds in the future (for the worst case and randomized
// tests).
static const ACE_Time_Value far_past_offset (-60, 0);
static const ACE_Time_Value near_future_offset (2, 0);
static const ACE_Time_Value offset_step (0, 5);

// The order consumer dequeues a message from the passed
// Message_Queue, and checks its data character against the passed
// string of characters which has the expected ordering.  Suppliers
// and consumers do not allocate or deallocate messages, to avoid
// timing delays and timing jitter in the test: the main function is
// responsible for all initialization allocation and cleanup before,
// between, and after (but not during) the transfer of messages from a
// supplier to the corresponding consumer.

static void *
order_consumer (void *args)
{
  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_SYNCH> *msg_queue =
    static_cast<ArgStruct *> (args)->queue_;
  const char *receipt_order =
    static_cast<ArgStruct *> (args)->order_string_;
  u_int expected_count =
    static_cast<ArgStruct *> (args)->expected_count_;

  ACE_ASSERT (receipt_order != 0);
  ACE_ASSERT (msg_queue != 0);

  u_int local_count = 0;

  // Keep looping, reading a message out of the queue, until we reach
  // the end of the receipt order string, which signals us to quit.

  for (const char *expected = receipt_order;
       *expected != '\0';
       ++expected)
  {
    ACE_Message_Block *mb = 0;

    int result = msg_queue->dequeue_head (mb);

    if (result == -1)
      break;

    local_count++;

    ACE_ASSERT (*expected == *mb->rd_ptr ());
  }

  ACE_ASSERT (local_count == ACE_OS::strlen (receipt_order));
  ACE_ASSERT (local_count == expected_count);
  return 0;
}

// The order producer runs through the passed send string, setting the
// read pointer of the current message to the current character
// position in the string, and then queueing the message in the
// message list, where it is removed by the order consumer.

static void *
order_producer (void *args)
{
  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_SYNCH> *msg_queue =
    static_cast<ArgStruct *> (args)->queue_;
  const char *send_order =
    static_cast<ArgStruct *> (args)->order_string_;
  ACE_Message_Block **block_array =
    static_cast<ArgStruct *> (args)->array_;
  int expected_count =
    static_cast<ArgStruct *> (args)->expected_count_;

  ACE_ASSERT (send_order != 0);
  ACE_ASSERT (block_array != 0);

  // Iterate through the send order string and the message block
  // array, setting the current message block's read pointer to the
  // current position in the send order string.
  int local_count = 0;
  const char *c;

  for (local_count = 0, c = send_order; *c != '\0'; ++local_count, ++c)
    {
      // point to the current message block
      ACE_Message_Block *mb = block_array [local_count];
      ACE_ASSERT (mb != 0);

      // Set the current send character in the current message block
      // at its read pointer position, and adjust the write pointer.
      *mb->wr_ptr () = *c;
      mb->wr_ptr (1);


      // Enqueue the message block in priority order.
      if (msg_queue->enqueue_prio (mb) == -1)
        break;
    }

  ACE_ASSERT (local_count == expected_count);

  return 0;
}

static int
run_order_test (ACE_Message_Queue<ACE_SYNCH>* msg_queue,
                const char *send_order,
                const char *receipt_order)
{
  u_int i;
  u_int array_size = ACE_OS::strlen (send_order);

  ACE_ASSERT (msg_queue != 0);
  ACE_ASSERT (send_order != 0);
  ACE_ASSERT (receipt_order != 0);
  ACE_ASSERT (ACE_OS::strlen (send_order) == ACE_OS::strlen (receipt_order));

  ArgStruct supplier_args, consumer_args;

  supplier_args.queue_ = msg_queue;
  supplier_args.order_string_ = send_order;
  supplier_args.expected_count_ = ACE_OS::strlen (send_order);

  // Allocate message blocks, fill in pointer array, set static
  // information.
  ACE_NEW_RETURN (supplier_args.array_,
                  ACE_Message_Block *[array_size],
                  -1);

  for (i = 0; i < array_size; ++i)
    {
      // Construct a message new block off the heap, to hold a single
      // character.
      ACE_NEW_RETURN (supplier_args.array_[i],
                      ACE_Message_Block (1),
                      -1);

      // Assign static (minimal) message priority in ascending order.
      supplier_args.array_[i]->msg_priority (i);

      // Assign every other message short or long execution time.
      supplier_args.array_[i]->msg_execution_time (((i % 2)
                                                    ? slow_execution
                                                    : fast_execution));
    }

  consumer_args.queue_ = msg_queue;
  consumer_args.order_string_ = receipt_order;
  consumer_args.expected_count_ = ACE_OS::strlen (receipt_order);
  consumer_args.array_ = 0;

  // Construct pending and late absolute deadline times.

  ACE_Time_Value current_time (0, 0);
  ACE_Time_Value future_deadline (1, 0);
  ACE_Time_Value near_deadline (0, 500000);
  ACE_Time_Value recent_deadline (0, -1);
  ACE_Time_Value past_deadline (0, -500000);

  current_time = ACE_OS::gettimeofday ();

  future_deadline += current_time;
  near_deadline += current_time;
  recent_deadline += current_time;
  past_deadline += current_time;

  // Set absolute time of deadline associated with the message.

  for (i = 0; i < array_size; ++i)
    {
      switch ((4 * i) / array_size)
        {
        case 0:
          supplier_args.array_[i]->msg_deadline_time (future_deadline);
          break;
        case 1:
          supplier_args.array_[i]->msg_deadline_time (near_deadline);
          break;
        case 2:
          supplier_args.array_[i]->msg_deadline_time (recent_deadline);
          break;
        case 3:
          supplier_args.array_[i]->msg_deadline_time (past_deadline);
          break;
          // should never reach here, but its better to make sure
        default:
          ACE_ASSERT ((4 * i) / array_size < 4);
          break;
        }
    }

  // run the order test producer
  order_producer (&supplier_args);

  // run the order test consumer
  order_consumer (&consumer_args);

  // free all the allocated message blocks
  for (i = 0; i < array_size; ++i)
  {
    delete supplier_args.array_[i];
  }

  // free the allocated pointer array
  delete [] supplier_args.array_;

  return 0;
}

// The performance consumer starts a timer, dequeues all messages from
// the passed Message_Queue, stops the timer, and reports the number
// of dequeued messages, the elapsed time, and the average time per
// message.

static void *
performance_consumer (void * args)
{
  ACE_High_Res_Timer timer;

  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_SYNCH> *msg_queue =
    static_cast<ArgStruct *> (args)->queue_;
  u_int expected_count =
    static_cast<ArgStruct *> (args)->expected_count_;

  ACE_ASSERT (msg_queue != 0);

  u_int local_count = 0;
  ACE_Message_Block *mb = 0;

  // reset, then start timer
  timer.reset ();
  timer.start ();

  // Keep looping, reading a message out of the queue, until the
  // expected number of messages have been dequeued.
  for (local_count = 0; local_count < expected_count; ++local_count)
    if (msg_queue->dequeue_head (mb) == -1)
      break;

  // Stop timer, obtain and report its elapsed time.x
  timer.stop ();
  ACE_Time_Value tv;
  timer.elapsed_time (tv);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%6u, %6u, %f"),
              local_count,
              tv.msec (),
              (ACE_timer_t) tv.msec () / local_count));

  ACE_ASSERT (local_count == expected_count);
  return 0;
}

// The performance producer starts a timer, enqueues the passed
// messages setting the read pointer of each message to the first
// character position in the passed string, stops the timer, and
// reports the number of enqueued messages, the elapsed time, and the
// average time per message.

static void *
performance_producer (void *args)
{
  ACE_High_Res_Timer timer;

  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_SYNCH> *msg_queue =
    static_cast<ArgStruct *> (args)->queue_;
  ACE_Message_Block **block_array =
    static_cast<ArgStruct *> (args)->array_;
  int expected_count =
    static_cast<ArgStruct *> (args)->expected_count_;

  ACE_ASSERT (send_order != 0);
  ACE_ASSERT (block_array != 0);

  // reset, then start timer
  timer.reset ();
  timer.start ();

  // Iterate through the message block array, setting the character
  // under the current message block's read pointer to null before
  // enqueueing the message block.

  int local_count = 0;
  for (local_count = 0; local_count < expected_count; ++local_count)
    {
      // Point to the current message block.
      ACE_Message_Block *mb = block_array [local_count];
      ACE_ASSERT (mb != 0);

      // Set a character in the current message block at its
      // read pointer position, and adjust the write pointer.
      mb->reset();
      *mb->wr_ptr () = 'a';
      mb->wr_ptr (1);

      // Enqueue the message block in priority order.
      if (msg_queue->enqueue_prio (mb) == -1)
        break;
    }

  // Stop timer, obtain and report its elapsed time.
  timer.stop ();
  ACE_Time_Value tv;
  timer.elapsed_time (tv);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%6u, %6u, %f, "),
              local_count,
              tv.msec (),
              (ACE_timer_t) tv.msec () / local_count));

  ACE_ASSERT (local_count == expected_count);
  return 0;
}

static int
run_performance_test (u_int min_load,
                      u_int max_load,
                      u_int load_step,
                      Test_Type test_type)
{
  ArgStruct supplier_args, consumer_args;   // supplier and consumer argument strings
  u_int load = 0;                           // message load
  ACE_Time_Value *time_offsets;             // pointer to array of time offsets
  ACE_Time_Value current_time;              // current time value
  u_int shuffle_index;                      // used to shuffle arrays
  int random_int = 0;                       // also used to shuffle arrays
  ACE_Message_Block *temp_block;            // temporary message block pointer
  ACE_Time_Value temp_time;                 // temporary time value

  // Build a static queue, a deadline based dynamic queue, and a
  // laxity based dynamic queue.

  ACE_Message_Queue<ACE_SYNCH> *static_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_static_message_queue (max_queue);
  ACE_ASSERT (static_queue != 0);

  ACE_Message_Queue<ACE_SYNCH> *deadline_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_deadline_message_queue (max_queue);
  ACE_ASSERT (deadline_queue != 0);

  ACE_Message_Queue<ACE_SYNCH> *laxity_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_laxity_message_queue (max_queue);

  ACE_ASSERT (laxity_queue != 0);

  // Zero out unused struct members.
  supplier_args.order_string_ = 0;
  consumer_args.order_string_ = 0;
  consumer_args.array_ = 0;

  // Print column headings for the specific test type.
  switch (test_type)
    {
    case BEST:
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\n\nenqueued, best static time, best static avg, ")
                  ACE_TEXT ("dequeued, best static time, best static avg, ")
                  ACE_TEXT ("enqueued, best deadline time, best deadline avg, ")
                  ACE_TEXT ("dequeued, best deadline time, best deadline avg, ")
                  ACE_TEXT ("enqueued, best laxity time, best laxity avg, ")
                  ACE_TEXT ("dequeued, best laxity time, best laxity avg\n")));
      break;
    case WORST:
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\n\nenqueued, worst static time, worst static avg, ")
                  ACE_TEXT ("dequeued, worst static time, worst static avg, ")
                  ACE_TEXT ("enqueued, worst deadline time, worst deadline avg, ")
                  ACE_TEXT ("dequeued, worst deadline time, worst deadline avg, ")
                  ACE_TEXT ("enqueued, worst laxity time, worst laxity avg, ")
                  ACE_TEXT ("dequeued, worst laxity time, worst laxity avg\n")));

      break;
    case RANDOM:
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\n\nenqueued, random static time, random static avg, ")
                  ACE_TEXT ("dequeued, random static time, random static avg, ")
                  ACE_TEXT ("enqueued, random deadline time, random deadline avg, ")
                  ACE_TEXT ("dequeued, random deadline time, random deadline avg, ")
                  ACE_TEXT ("enqueued, random laxity time, random laxity avg, ")
                  ACE_TEXT ("dequeued, random laxity time, random laxity avg\n")));
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("unknown test type %d"),
                         test_type),
                        -1);
    }

  // Iterate through the message loads, and at each load do an
  // identical test on all queues.
  for (load = min_load; load <= max_load; load += load_step)
    {
      u_int i;

      supplier_args.expected_count_ = load;
      consumer_args.expected_count_ = load;

      // Allocate message blocks, fill in pointer array, set static
      // information.
      ACE_NEW_RETURN (supplier_args.array_,
                      ACE_Message_Block *[load],
                      -1);

      // Allocate array of timing offsets.
      ACE_NEW_RETURN (time_offsets,
                      ACE_Time_Value [load],
                      -1);

      // Fill in information for all types of tests.
      for (i = 0; i < load; ++i)
        {
          // Construct a message new block off the heap, to hold a
          // single character.
          ACE_NEW_RETURN (supplier_args.array_[i],
                          ACE_Message_Block (1),
                          -1);

          // Assign every other message short or long execution time.
          supplier_args.array_[i]->msg_execution_time (((i % 2)
                                                        ? slow_execution
                                                        : fast_execution));
        }

      // Fill in information for the specific type of test.
      switch (test_type)
        {
        case BEST:
          // Fill in best case information.
          time_offsets [0] = far_past_offset;
          supplier_args.array_[0]->msg_priority (load);

          for (i = 1; i < load; ++i)
            {
              // Assign static (minimal) message priority in
              // descending order.
              supplier_args.array_[i]->msg_priority (load - i);

              // Assign time to deadline in descending order.
              time_offsets [i] = time_offsets [i - 1] + offset_step;
            }

          break;
        case WORST:
          // Fill in worst case information.
          time_offsets [0] = near_future_offset;
          supplier_args.array_[0]->msg_priority (0);

          for (i = 1; i < load; ++i)
            {
              // Assign static (minimal) message priority in ascending
              // order.
              supplier_args.array_[i]->msg_priority (i);

              // Assign time to deadline in descending order (puts
              // dynamic priority in ascending order).
              time_offsets [i] = time_offsets [i - 1] - offset_step;
            }
          break;
        case RANDOM:
          // Fill in worst case information.
          time_offsets [0] = near_future_offset;
          supplier_args.array_[0]->msg_priority (0);

          for (i = 1; i < load; ++i)
            {
              // Assign static (minimal) message priority in ascending
              // order.
              supplier_args.array_[i]->msg_priority (i);

              // Assign time to deadline in descending order (puts
              // dynamic priority in ascending order).
              time_offsets [i] = time_offsets [i - 1] - offset_step;
            }

          // Then shuffle the arrays in tandem.
          for (i = 0; i < load; ++i)
            {
              // Choose a (pseudo) random integer (evenly distributed
              // over [0, load-1]).
              if (RAND_MAX >= load)
                {
                  // Discard integers in the tail of the random range
                  // that do not distribute evenly modulo the number
                  // of messages.
                  do
                    random_int = ACE_OS::rand ();
                  while (random_int >= (int)(RAND_MAX - (RAND_MAX % load)));
                }
              else if (RAND_MAX < load - 1)
                // This should only happen for a *very* large messages
                // relative to the system's representation size.
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Insufficient range of random numbers")),
                                  -1);
              shuffle_index = random_int % load;

              // Swap the message at the current index with the one at
              // the shuffle index.
              temp_block = supplier_args.array_[i];
              supplier_args.array_[i] = supplier_args.array_[shuffle_index];
              supplier_args.array_[shuffle_index] = temp_block;

              // Swap the time at the current index with the one at
              // the shuffle index.
              temp_time = time_offsets [i];
              time_offsets [i] = time_offsets [shuffle_index];
              time_offsets [shuffle_index] = temp_time;
            }
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("unknown test type %d"),
                             test_type),
                            -1);
        }

      // Set absolute time of deadline associated with each message.
      current_time = ACE_OS::gettimeofday ();

      for (i = 0; i < load; ++i)
        supplier_args.array_[i]->msg_deadline_time (time_offsets [i] + current_time);

      // Run the performance test producer and consumer on the static
      // queue.
      supplier_args.queue_ = static_queue;
      performance_producer (&supplier_args);
      consumer_args.queue_ = static_queue;
      performance_consumer (&consumer_args);

      // Add a comma delimiter for most recent outputs.
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT (", ")));

      // Run the performance test producer and consumer on the
      // deadline queue.
      supplier_args.queue_ = deadline_queue;
      performance_producer (&supplier_args);
      consumer_args.queue_ = deadline_queue;
      performance_consumer (&consumer_args);

      // Add a comma delimiter for most recent outputs.
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT (", ")));

      // Run the performance test producer and consumer on the laxity
      // queue.
      supplier_args.queue_ = laxity_queue;
      performance_producer (&supplier_args);
      consumer_args.queue_ = laxity_queue;
      performance_consumer (&consumer_args);

      // Move to the next line of output.
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\n")));

      delete [] time_offsets;

      // Free all the allocated message blocks.
      for (i = 0; i < load; ++i)
        delete supplier_args.array_[i];

      // Free the allocated pointer array.
      delete [] supplier_args.array_;

    }

  // Free resources and leave.
  delete static_queue;
  delete deadline_queue;
  delete laxity_queue;
  return 0;
}
#endif /* ACE_HAS_TIMED_MESSAGE_BLOCKS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dynamic_Priority_Test"));

#if defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
  {
    if (ACE_OS::last_error () == EPERM)
      ACE_DEBUG ((LM_MAX,
                  ACE_TEXT ("user is not superuser, ")
                  ACE_TEXT ("so remain in time-sharing class\n")));
    else if (ACE_OS::last_error () == ENOTSUP)
      ACE_DEBUG ((LM_MAX,
                  ACE_TEXT ("process scope scheduling is not available, ")
                  ACE_TEXT ("so remain in time-sharing class\n")));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%n: ACE_OS::sched_params failed\n%a")),
                        -1);
  }

  // Test factory, static message queue.
  ACE_Message_Queue<ACE_SYNCH> *test_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_static_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_order_test (test_queue,
                  send_order,
                  static_receipt_order);
  delete test_queue;

  // Test factory, dynamic message queue (deadline strategy).
  test_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_deadline_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_order_test (test_queue,
                  send_order,
                  deadline_receipt_order);
  delete test_queue;

  // Test factory, dynamic message queue (laxity strategy).
  test_queue =
    ACE_Message_Queue_Factory<ACE_SYNCH>::create_laxity_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_order_test (test_queue,
                  send_order,
                  laxity_receipt_order);
  delete test_queue;

#if defined (VXWORKS)
  // test factory for VxWorks message queue.
  ACE_Message_Queue_Vx *test_queue_vx =
    ACE_Message_Queue_Factory<ACE_NULL_SYNCH>::create_Vx_message_queue (vx_max_queue,
                                                                        vx_msg_size);
  ACE_ASSERT (test_queue_vx != 0);
  // (TBD - does message receipt order test make any sense for Vx Queue ?
  //  If so, uncomment order test, or if not remove order test, below)
  // @@ % levine 22 Jul 1998 % It'd be nice to run the test, but:
  //                           ACE_Message_Queue_Vx isa
  //                           ACE_Message_Queue<ACE_NULL_SYNCH>, not an
  //                           ACE_Message_Queue<ACE_MT_SYNCH>, so we're
  //                           not type-compatible.

  // run_order_test (test_queue, send_order, static_receipt_order);
  delete test_queue_vx;
#endif /* VXWORKS */

  // For each of an increasing number of message loads, run the same
  // performance test (best case, worst case, and randomized, over
  // each kind of queue).
  run_performance_test (MIN_LOAD,
                        MAX_LOAD,
                        LOAD_STEP,
                        BEST);

  run_performance_test (MIN_LOAD,
                        MAX_LOAD,
                        LOAD_STEP,
                        WORST);

  run_performance_test (MIN_LOAD,
                        MAX_LOAD,
                        LOAD_STEP,
                        RANDOM);
#else
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("ACE is not compiled with ACE_HAS_TIMED_MESSAGE_BLOCKS enabled\n")));
#endif /* ACE_HAS_TIMED_MESSAGE_BLOCKS */

  ACE_END_TEST;
  return 0;
}
