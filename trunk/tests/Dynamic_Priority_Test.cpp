// $Id$
//
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
//    priority mechanisms of the ACE_Message_Queue class and the 
//    ACE_Dynamic_Message_Queue class. As in the Priority_Buffer_Test, 
//    a producer generates messages and enqueues them, and a consumer 
//    dequeues them and checks their ordering.  
//
//    In these tests, every effort is made to ensure that there is plenty
//    of time for the messages to be enqueued and dequeued, with messages
//    that *should* meet their deadlines actually meeting them,
//    while messages that should miss their deadlines are delayed
//    so that they actually miss them.  It is, however, remotely
//    possible that this test could yield a false negative:
//    the dynamic queues could work correctly but due to timing 
//    variations the test could indicate failure.
//
//    Three message queues are obtained from the message queue factory,
//    one static, two dynamic (one deadline based, and one laxity based)
//    and the same supplier behavior is used each time: the messages
//    are preallocated and their static information valued, the current    
//    time is obtained and deadlines are set, with half of the messages
//    given late deadlines, and the other half of the messages given
//    reachable deadlines.  The producer then immediately enqueues all
//    messages.
//
//    In each test, the consumer is passed the filled queue and a string with
//    the expected order in which the messages should dequeue.
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// structure used to pass arguments to test functions

struct ArgStruct 
{
  ACE_Message_Queue<ACE_MT_SYNCH> *queue_;
  const char *order_string_;
  ACE_Message_Block **array_;
};

// order in which messages are sent
static const char send_order [] = "abcdefghijklmnop";

// order in which messages are received with static prioritization
static const char static_receipt_order [] = "ponmlkjihgfedcba"; 

// order in which messages are received with deadline prioritization
static const char deadline_receipt_order [] = "hgfedcbaponmlkji";

// order in which messages are received with laxity prioritization
static const char laxity_receipt_order [] = "hfgedbcapnomljki";

// fast and slow execution time values (sec, usec)
static const ACE_Time_Value fast_execution (0, 10);
static const ACE_Time_Value slow_execution (0, 100);

// Make the queue be capable of being *very* large.
static const long max_queue = LONG_MAX;

// The consumer dequeues a message from the passed Message_Queue,
// and checks its data character against the passed string of characters
// which has the expected ordering.   The supplier and consumer do not
// allocate or deallocate messages, to avoid timing delays and timing
// jitter in the test: the main function is responsible for all
// initialization allocation and cleanup before, between, and after
// (but not during) the transfer of messages from the supplier to the
// consumer.

static void *
consumer (void * args)
{
  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue = ((ArgStruct *) args)->queue_;
  const char *receipt_order = ((ArgStruct *) args)->order_string_;

  ACE_ASSERT (receipt_order != 0);
  ACE_ASSERT (msg_queue != 0);

  u_int local_count = 0;

  // Keep looping, reading a message out of the queue, until we
  // reach the end of the receipt order string, which signals us to quit.
  for (const char *expected = receipt_order; *expected != '\0'; ++expected)
  {
    ACE_Message_Block *mb = 0;

    int result = msg_queue->dequeue_head (mb);

    if (result == -1)
      break;

    local_count++;

	ACE_ASSERT (*expected == *mb->rd_ptr ());
  }

  ACE_ASSERT (local_count == ACE_OS::strlen (receipt_order));
  return 0;
}

// The producer runs through the passed send string,  setting the read 
// pointer of the current message to the current character position in
// the string, and then queueing the message in the message list, where
// it is removed by the consumer thread.

static void *
producer (void *args)
{
  ACE_ASSERT (args != 0);

  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue = ((ArgStruct *) args)->queue_;
  const char *send_order = ((ArgStruct *) args)->order_string_;
  ACE_Message_Block **block_array = ((ArgStruct *) args)->array_;

  ACE_ASSERT (send_order != 0);
  ACE_ASSERT (block_array != 0);

  // iterate through the send order string and the message block array,
  // setting the current message block's read pointer to the current
  // position in the send order string. 
  int local_count;
  const char *c;
  for (local_count = 0, c = send_order; *c != '\0'; ++local_count, ++c)
    {
      // point to the current message block
      ACE_Message_Block *mb = block_array [local_count];
      ACE_ASSERT (mb != 0);

      // Set the current send character in the current message block
      // at its read pointer position, and adjust the write pointer
      *mb->rd_ptr () = *c;
      mb->wr_ptr (1);

      // Enqueue the message block in priority order.
      if (msg_queue->enqueue_prio (mb) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"), ASYS_TEXT ("put_next")), 0);
    }

  return 0;
}


int  run_test (ACE_Message_Queue<ACE_MT_SYNCH>* msg_queue, const char *send_order, const char *receipt_order)
{
  u_int i = 0;
  u_int array_size = ACE_OS::strlen (send_order);

  ACE_ASSERT (msg_queue != 0);
  ACE_ASSERT (send_order != 0);
  ACE_ASSERT (receipt_order != 0);
  ACE_ASSERT (ACE_OS::strlen (send_order) == ACE_OS::strlen (receipt_order));

  ArgStruct supplier_args, consumer_args;

  supplier_args.queue_ = msg_queue;
  supplier_args.order_string_ = send_order;

  // allocate message blocks, fill in pointer array, set static information
  ACE_NEW_RETURN (supplier_args.array_, ACE_Message_Block * [array_size], -1);
  for (i = 0; i < array_size; ++i)
  {
    // construct a message new block off the heap, to hold a single character
    ACE_NEW_RETURN (supplier_args.array_[i], ACE_Message_Block (1), -1);

    // assign static (minimal) message priority in ascending order
    supplier_args.array_[i]->msg_priority (i);

    // assign every other message short or long execution time
    supplier_args.array_[i]->msg_execution_time (((i % 2) ? slow_execution : fast_execution));
  }

  consumer_args.queue_ = msg_queue;
  consumer_args.order_string_ = receipt_order;
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
    switch ((4*i)/array_size)
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
        ACE_ASSERT ((4*i)/array_size < 4);  
        break;
	}
  }

  // run the producer
  producer (&supplier_args);

  // run the consumer
  consumer (&consumer_args);

  // free all the allocated message blocks 
  for (i = 0; i < array_size; ++i)
  {
    delete supplier_args.array_[i];
  }

  // free the allocated pointer array
  delete [] supplier_args.array_;

  return 0;
}


int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Dynamic_Priority_Test"));
 
  ACE_Message_Queue<ACE_MT_SYNCH> *test_queue = 0;

  // test factory, static message queue
  test_queue = ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_static_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_test (test_queue, send_order, static_receipt_order);
  delete test_queue;

  // test factory, dynamic message queue (deadline strategy, no cleanup)
  test_queue =  ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_deadline_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_test (test_queue, send_order, deadline_receipt_order);
  delete test_queue;

  // test factory, dynamic message queue (laxity strategy, no cleanup)
  test_queue =  ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_laxity_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  run_test (test_queue, send_order, laxity_receipt_order);
  delete test_queue;

  // test factory (deadline strategy, with cleanup)
  test_queue =  ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_deadline_cleanup_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  delete test_queue;

  // test factory (laxity strategy, with cleanup)
  test_queue =  ACE_Message_Queue_Factory<ACE_MT_SYNCH>::create_laxity_cleanup_message_queue (max_queue);
  ACE_ASSERT (test_queue != 0);
  delete test_queue;

  ACE_END_TEST;
  return 0;
}




