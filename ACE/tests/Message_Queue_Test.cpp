// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Message_Queue_Test.cpp
//
// = DESCRIPTION
//      This is:
//      0) a test that ensures key ACE_Message_Queue features are
//         working properly, including timeouts and priorities
//      1) a simple test of the ACE_Message_Queue that illustrates how to
//         use the forward and reverse iterators
//      2) a simple performance measurement test for both single-threaded
//         (null synch), thread-safe ACE_Message_Queues, and
//         ACE_Message_Queue_Vx, which wraps VxWorks message queues
//      3) a test/usage example of ACE_Message_Queue_Vx
//      4) a test of the message counting in a message queue under load.
//
// = AUTHORS
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    David L. Levine <levine@cs.wustl.edu>, and
//    Douglas C. Schmidt <schmidt@vanderbilt.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Atomic_Op.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Message_Queue_NT.h"
#include "ace/Message_Queue_Vx.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Null_Condition.h"
#include "ace/High_Res_Timer.h"
#include "ace/Task.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"



const ACE_TCHAR usage[] = ACE_TEXT ("usage: Message_Queue_Test <number of messages>\n");

typedef ACE_Message_Queue<ACE_NULL_SYNCH> QUEUE;
typedef ACE_Message_Queue_Iterator<ACE_NULL_SYNCH> ITERATOR;
typedef ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH> REVERSE_ITERATOR;

#if defined (ACE_HAS_WINCE)
static const int MESSAGE_FACTOR = 10000;
#else
static const int MESSAGE_FACTOR = 100000;
#endif
static const int MAX_MESSAGES = 10000;
static const int MAX_MESSAGE_SIZE = 32;
static const char test_message[] = "ACE_Message_Queue Test Message";

static int max_messages = MAX_MESSAGES;

// Dynamically allocate to avoid a static.
static ACE_High_Res_Timer *timer = 0;

#if defined (ACE_HAS_THREADS)
typedef ACE_Message_Queue<ACE_MT_SYNCH> SYNCH_QUEUE;

struct Queue_Wrapper
{
  // = TITLE
  //     Container for data passed to sender and receiver in
  //     performance test.
  //
  // = DESCRIPTION
  //     For use in multithreaded performance test.

  ACE_Message_Queue_Base *q_;
  // The message queue.

  ACE_Message_Block **send_block_;
  // Pointer to messages blocks for sender to send to reciever.

  Queue_Wrapper (void)
    : q_ (0), send_block_ (0)
  {
  }
  // Default constructor.
};

// For the message counting test, there are two tasks, producer and consumer.
// Each will spawn a number of threads, and the two tasks share a queue.
class Counting_Test_Producer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Counting_Test_Producer (ACE_Message_Queue<ACE_MT_SYNCH> *queue)
    : ACE_Task<ACE_MT_SYNCH> (0, queue), sequence_ (0), produced_ (0) {}
  virtual int svc (void);

  ACE_Atomic_Op<ACE_Thread_Mutex, long> sequence_;
  ACE_Atomic_Op<ACE_Thread_Mutex, long> produced_;
};

class Counting_Test_Consumer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Counting_Test_Consumer (ACE_Message_Queue<ACE_MT_SYNCH> *queue)
    : ACE_Task<ACE_MT_SYNCH> (0, queue), consumed_ (0) {}
  virtual int svc (void);

  ACE_Atomic_Op<ACE_Thread_Mutex, long> consumed_;
};

int
Counting_Test_Producer::svc (void)
{
  // Going to produce a lot of blocks. Since we don't necessarily want them
  // all consumed, there's no arrangement with the consumer to be sure that
  // the same number produced will be consumed; the test check will compare
  // the number produced, consumed, and remaining to be sure it ends up
  // correct.
  // Also, to be sure there's not just 1 producer and 1 consumer pinging
  // back and forth, make the producers randomly delay between blocks.
  ACE_OS::srand (static_cast<ACE_RANDR_TYPE> (ACE_OS::time ()));
  int multiple = ACE_OS::rand () % 10;
  int delay_ms = (ACE_OS::rand () % 10) / 2;
  // The delay usually causes the test to time out in the automated
  // regression testing. I just left it here in case it's needed someday.
  delay_ms = 0;
  long count = MESSAGE_FACTOR * (multiple ? multiple : 1);
  long produced = 0;
  // Some of the threads enqueue single blocks, others sequences.
  long lsequence = ++(this->sequence_);
  int seq = static_cast<int> (lsequence);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Producer will enqueue %B blocks in seq of %d, ")
              ACE_TEXT ("%d msec delay\n"),
              (size_t)count,
              seq,
              delay_ms));

  ACE_Message_Block *first = 0, *prev = 0, *b = 0;
  ACE_Time_Value delay (0, delay_ms);
  ACE_Time_Value timeout (10);
  while (produced < count)
    {
      ACE_NEW_NORETURN (b, ACE_Message_Block (1));
      if (b == 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Producer out of memory\n")));
          break;
        }
      first = b;
      prev = first;
      for (int s = 1; s < seq; ++s)
        {
          ACE_NEW_NORETURN (b, ACE_Message_Block (1));
          if (b == 0)
            break;
          prev->next (b);
          b->prev (prev);
          prev = b;
        }
      if (b == 0)
        {
          if (first != b)
            {
              while (first->next () != 0)
                {
                  b = first->next ();
                  first->release ();
                  first = b;
                }
              first->release ();
            }
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Producer out of memory\n")));
          break;
        }
      // To be sure we can keep going on slow or completed consumers, but not
      // delay excessively if the consumers have stopped, limit the time
      // spent waiting to 10 seconds.
      ACE_Time_Value block = ACE_OS::gettimeofday ();
      block += timeout;
      if (this->putq (first, &block) == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Producer cannot putq; giving up\n")));
          while (first->next () != 0)
            {
              b = first->next ();
              first->release ();
              first = b;
            }
          first->release ();
          break;
        }
      produced += seq;
      if (delay_ms)
        ACE_OS::sleep (delay);
    }
  this->produced_ += produced;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Producer done\n")));
  return 0;
}

int
Counting_Test_Consumer::svc (void)
{
  // Consume lots of blocks and release them. To mimic a thread with work
  // to do, put a small random delay between dequeuing the blocks. Consume
  // a calculated number of blocks then stop; the test checker will determine
  // if the number consumed plus the number remaining is correct for the
  // number produced.
  ACE_RANDR_TYPE seed = static_cast<ACE_RANDR_TYPE> (ACE_OS::time ());

  int multiple = ACE_OS::rand_r (seed) % 10;
  int delay_ms = ACE_OS::rand_r (seed) % 10;
  // The delay usually causes the test to time out in the automated
  // regression testing. I just left it here in case it's needed someday.
  delay_ms = 0;
  long count = MESSAGE_FACTOR * (multiple ? multiple : 1);
  long consumed = 0;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Consumer will dequeue %B blocks, ")
              ACE_TEXT ("%d msec delay\n"),
              (size_t)count,
              delay_ms));
  ACE_Message_Block *b = 0;
  ACE_Time_Value delay (0, delay_ms);
  ACE_Time_Value timeout (2);
  while (consumed < count)
    {
      // To be sure we can wait in the case of an empty queue, but not
      // delay excessively if the producers have stopped, limit the time
      // spent waiting to 2 seconds.
      ACE_Time_Value block = ACE_OS::gettimeofday ();
      block += timeout;
      if (this->getq (b, &block) == -1)
        {
          if (errno == EWOULDBLOCK)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Consumer timed out\n")));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%t) Consumer %p\n"),
                        ACE_TEXT ("getq")));
          break;
        }
      ++consumed;
      b->release ();
      if (delay_ms)
        ACE_OS::sleep (delay);
    }
  this->consumed_ += consumed;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Consumer done\n")));
  return 0;
}

static int
counting_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting counting test\n")));

  ACE_Message_Queue<ACE_MT_SYNCH> q (2 * 1024 * 1024);  // 2MB high water
  Counting_Test_Producer p (&q);
  Counting_Test_Consumer c (&q);
  // Activate consumers first; if the producers fail to start, consumers will
  // stop quicker.
  if (c.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 5) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Consumers %p\n"),
                       ACE_TEXT ("activate")),
                      -1);
  if (p.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 5) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Producers %p\n"),
                  ACE_TEXT ("activate")));
      c.wait ();
      return -1;
    }
  // Producers and consumers are both running; wait for them then
  // check the results.
  p.wait ();
  c.wait ();
  // This compare relies on the flush() method counting blocks as it
  // walks the chain releasing them, and doesn't rely on the count.
  int status = 0;
  long q_count = static_cast<long> (q.message_count ());
  long remaining = q.flush ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Queue message_count is %b; %b flushed\n"),
              (ssize_t)q_count,
              (ssize_t)remaining));
  if (q_count != remaining)
    {
      status = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("message_count and flushed should be equal!\n")));
    }
  long expected = p.produced_.value () - c.consumed_.value ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Produced %b, consumed %b; diff %b\n"),
              (ssize_t)p.produced_.value (),
              (ssize_t)c.consumed_.value (),
              (ssize_t)expected));
  if (expected != remaining)
    {
      status = -1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Producer-consumer diff is %b; should be %b\n"),
                  (ssize_t)expected,
                  (ssize_t)remaining));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ending counting test\n")));
  return status;
}

#endif /* ACE_HAS_THREADS */

static int
iterator_test (void)
{
  const int ITERATIONS = 5;
  ACE_TCHAR buffer[ITERATIONS][BUFSIZ];
  // Use queue size from of 32 Kb (more if using wide-char), instead of the
  // default of 16 Kb (defined by ACE_Message_Queue_Base::DEFAULT_HWM),
  // so that the test runs on machines with 8Kb pagesizes.

  //  QUEUE queue (32 * 1024 * sizeof (ACE_TCHAR));
  QUEUE queue (sizeof(buffer));

  int i;

  for (i = 0; i < ITERATIONS; i++)
    {
      ACE_OS::sprintf (buffer[i],
                       ACE_TEXT ("%d"),
                       i + 1);

      ACE_Message_Block *entry = 0;
      ACE_NEW_RETURN (entry,
                      ACE_Message_Block ((char *) buffer[i],
                                         sizeof buffer[i]),
                      -1);

      if (queue.is_full ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("QUEUE:: the message queue is full on iteration %u!\n"),
                           i + 1),
                          -1);

      if (queue.enqueue (entry) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("QUEUE::enqueue\n")),
                          -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nForward Iterations\n")));
  {
    ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s\n"),
                  entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nReverse Iterations\n")));
  {
    REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s\n"),
                  entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nForward Iterations\n")));
  {
    QUEUE::ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s\n"),
                  entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nReverse Iterations\n")));
  {
    QUEUE::REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s\n"),
                  entry->base ()));
  }

  return 0;
}

#if defined (ACE_HAS_THREADS)

static int
chained_block_test (void)
{

  QUEUE q;
  const char * s = "123456789";      // Will be length 10 when copied to block
  const size_t slen = 10;
  const size_t num_blks = 10;
  ACE_Message_Block b[num_blks];
  size_t i;
  int status = 0;

  for (i = 0; i < num_blks; ++i)
    {
      b[i].init (slen);
      b[i].copy (s);
    }

  // Test enqueueing single and chained blocks and be sure they end up with
  // the proper enqueued block count and sizes. Then be sure they are dequeued
  // in the proper order.
  b[0].next (&b[1]);
  b[1].next (&b[2]);
  // b[3] and b[4] are unchained.
  b[5].next (&b[6]);
  b[6].next (&b[7]);
  b[7].next (&b[8]);
  // b[9] is unchained
  q.enqueue_tail (&b[3]);
  q.enqueue_tail (&b[4]);
  int num = q.enqueue_head (&b[0]);
  if (num != 5)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Chained enqueue expected 5; has %d\n"),
                  num));
      status = -1;
    }
  num = q.enqueue_tail (&b[5]);
  if (num != 9)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Chained enqueue expected 9; has %d\n"),
                  num));
      status = -1;
    }
  num = q.enqueue_tail (&b[9]);
  if (num != 10)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Chained enqueue expected 10; has %d\n"),
                  num));
      status = -1;
    }
  size_t msgs, bytes;
  msgs = q.message_count ();
  bytes = q.message_bytes ();
  if (msgs != 10 || bytes != 100)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Chained enqueue totals: %d msgs, %d bytes; ")
                  ACE_TEXT ("should be 10 msgs, 100 bytes\n"),
                  (int)msgs, (int)bytes));
      status = -1;
    }

  // Now see if we can dequeue them, checking the order.
  ACE_Time_Value nowait (ACE_OS::gettimeofday ());
  ACE_Message_Block *bp;
  int qstat;
  for (i = 0; i < num_blks; ++i)
    {
      qstat = q.dequeue_head (bp, &nowait);
      if (qstat == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Checking chained blocks, pass %d: %p\n"),
                      (int)i, ACE_TEXT ("dequeue_head")));
          status = -1;
        }
      else if (bp != &b[i])
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Checking chained blocks, pass %d: ")
                      ACE_TEXT ("block out of order\n"),
                      (int)i));
          status = -1;
        }
    }

  if (status == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Chained block test OK\n")));
  return status;
}

static int
single_thread_performance_test (int queue_type = 0)
{
  const char test_message[] =
    "ACE_Message_Queue Test Message";
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue<ACE_NULL_SYNCH>, single thread");
  int i = 0;

  // Create a message queue.
  ACE_Message_Queue_Base *msgq = 0;

  if (queue_type == 0)
    ACE_NEW_RETURN (msgq,
                    QUEUE,
                    -1);
#if defined (ACE_VXWORKS)
  else
    {
      ACE_NEW_RETURN (msgq,
                      ACE_Message_Queue_Vx (max_messages,
                                            MAX_MESSAGE_SIZE),
                      -1);
      message = "ACE_Message_Queue_Vx, single thread test";
    }
#elif defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
  else
    {
      ACE_NEW_RETURN (msgq,
                      ACE_Message_Queue_NT,
                      -1);
      message = ACE_TEXT ("ACE_Message_Queue_NT, single thread test");
    }
#endif /* ACE_VXWORKS */

  // Create the messages.  Allocate off the heap in case messages
  // is large relative to the amount of stack space available.
  ACE_Message_Block **send_block = 0;
  ACE_NEW_RETURN (send_block,
                  ACE_Message_Block *[max_messages],
                  -1);

  for (i = 0; i < max_messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    ACE_Message_Block (test_message,
                                       MAX_MESSAGE_SIZE),
                    -1);

  ACE_Message_Block **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  ACE_Message_Block *[max_messages],
                  -1);

#if defined (ACE_VXWORKS)
  // Set up blocks to receive the messages.  Allocate these off the
  // heap in case messages is large relative to the amount of
  // stack space available.
  ACE_Message_Block *receive_block = 0;
  ACE_NEW_RETURN (receive_block,
                  ACE_Message_Block[max_messages],
                  -1);

  for (i = 0; i < max_messages; ++i)
    {
      receive_block[i].init (MAX_MESSAGE_SIZE);

      // For VxWorks Message Queues, the receive block pointer must be
      // assigned.  It will be used by dequeue_head ().
      receive_block_p[i] = &receive_block[i];
    }
#endif /* ACE_VXWORKS */

  timer->start ();

  // Send/receive the messages.
  for (i = 0; i < max_messages; ++i)
    {
      if (msgq->enqueue_tail (send_block[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("enqueue")),
                          -1);

      if (msgq->dequeue_head (receive_block_p[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("dequeue_head")),
                          -1);
    }

  timer->stop ();

  ACE_Time_Value tv;
  timer->elapsed_time (tv);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%s: %u messages took %u msec (%f msec/message)\n"),
              message,
              max_messages,
              tv.msec (),
              (double) tv.msec () / max_messages));
  timer->reset ();

  delete [] receive_block_p;
#if defined (ACE_VXWORKS)
  delete [] receive_block;
#endif /* ACE_VXWORKS */

  for (i = 0; i < max_messages; ++i)
    delete send_block[i];
  delete [] send_block;
  delete msgq;

  return 0;
}

static void *
receiver (void *arg)
{
  Queue_Wrapper *queue_wrapper =
    reinterpret_cast<Queue_Wrapper *> (arg);
  int i;

  ACE_Message_Block **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  ACE_Message_Block *[max_messages],
                  (void *) -1);

#if defined (ACE_VXWORKS)
  // Set up blocks to receive the messages.  Allocate these off the
  // heap in case messages is large relative to the amount of stack
  // space available.
  ACE_Message_Block *receive_block;
  ACE_NEW_RETURN (receive_block,
                  ACE_Message_Block[max_messages],
                  (void *) -1);

  for (i = 0; i < max_messages; ++i)
    {
      receive_block[i].init (MAX_MESSAGE_SIZE);

      // For VxWorks Message Queues, the receive block pointer must be
      // assigned.  It will be used by <dequeue_head>.
      receive_block_p[i] = &receive_block[i];
    }
#endif /* ACE_VXWORKS */

  for (i = 0; i < max_messages; ++i)
    if (queue_wrapper->q_->dequeue_head (receive_block_p[i]) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("dequeue_head")),
                        0);
  timer->stop ();

  delete [] receive_block_p;
#if defined (ACE_VXWORKS)
  delete [] receive_block;
#endif /* ACE_VXWORKS */

  return 0;
}

static void *
sender (void *arg)
{
  Queue_Wrapper *queue_wrapper =
    reinterpret_cast<Queue_Wrapper *> (arg);
  int i;

  timer->start ();

  // Send the messages.
  for (i = 0; i < max_messages; ++i)
    if (queue_wrapper->q_->
        enqueue_tail (queue_wrapper->send_block_[i]) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("enqueue")),
                        0);
  return 0;
}

static
int
performance_test (int queue_type = 0)
{
  Queue_Wrapper queue_wrapper;
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue<ACE_SYNCH>");
  int i = 0;

  // Create the messages.  Allocate off the heap in case messages is
  // large relative to the amount of stack space available.  Allocate
  // it here instead of in the sender, so that we can delete it after
  // the _receiver_ is done.
  ACE_Message_Block **send_block = 0;
  ACE_NEW_RETURN (send_block,
                  ACE_Message_Block *[max_messages],
                  -1);

  for (i = 0; i < max_messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    ACE_Message_Block (test_message,
                                       MAX_MESSAGE_SIZE),
                    -1);

  queue_wrapper.send_block_ = send_block;

  if (queue_type == 0)
    ACE_NEW_RETURN (queue_wrapper.q_,
                    SYNCH_QUEUE,
                    -1);
#if defined (ACE_VXWORKS)
  else
    {
      ACE_NEW_RETURN (queue_wrapper.q_,
                      ACE_Message_Queue_Vx (max_messages,
                                            MAX_MESSAGE_SIZE),
                      -1);
      message = "ACE_Message_Queue_Vx";
    }
#elif defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
  else
    {
      ACE_NEW_RETURN (queue_wrapper.q_,
                      ACE_Message_Queue_NT,
                      -1);
      message = ACE_TEXT ("ACE_Message_Queue_NT");
    }
#endif /* ACE_VXWORKS */

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) sender,
                                              &queue_wrapper,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawning sender thread")),
                      -1);

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) receiver,
                                              &queue_wrapper,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawning receiver thread")),
                      -1);

  ACE_Thread_Manager::instance ()->wait ();
  ACE_Time_Value tv;
  timer->elapsed_time (tv);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%s: %u messages took %u msec (%f msec/message)\n"),
              message,
              max_messages,
              tv.msec (),
              (double) tv.msec () / max_messages));
  timer->reset ();

  delete queue_wrapper.q_;
  queue_wrapper.q_ = 0;

  for (i = 0; i < max_messages; ++i)
    delete send_block[i];
  delete [] send_block;

  return 0;
}

// Ensure that the timedout dequeue_head() sets errno code properly.

static int
timeout_test (void)
{
  SYNCH_QUEUE mq;
  int status = 0;

  if (!mq.is_empty ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("New queue is not empty!\n")));
      status = 1;
    }
  else
    {
      ACE_Message_Block *b;
      ACE_Time_Value tv (ACE_OS::gettimeofday ());   // Now

      if (mq.dequeue_head (b, &tv) != -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Dequeued from empty queue!\n")));
          status = 1;
        }
      else if (errno != EWOULDBLOCK)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Dequeue timeout should be EWOULDBLOCK, got")));
          status = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Timed dequeue test: OK\n")));
          status = 0;     // All is well
        }
    }

  return status;
}
#endif /* ACE_HAS_THREADS */

// Check to make sure that dequeue_prio() respects FIFO ordering.
// @@ At some point, this function should be enhanced to do a more
// thorough check...

static int
prio_test (void)
{
  const char S1[] = "first";
  const char S2[] = "second";
  const int PRIORITY = 50;
  QUEUE mq;
  int status;

  ACE_Message_Block mb1 (S1, sizeof S1, PRIORITY);
  ACE_Message_Block mb2 (S2, sizeof S2, PRIORITY);

  mq.enqueue_prio (&mb1);
  mq.enqueue_prio (&mb2);

  ACE_Message_Block *mb1p;
  ACE_Message_Block *mb2p;

  mq.dequeue_prio (mb1p);
  mq.dequeue_prio (mb2p);

  ACE_DEBUG ((LM_DEBUG, "message 1 = %C\nmessage 2 = %C\n",
              mb1p->rd_ptr (),
              mb2p->rd_ptr ()));

  if (ACE_OS_String::strcmp (mb1p->rd_ptr (), S1) == 0
      && ACE_OS_String::strcmp (mb2p->rd_ptr (), S2) == 0)
    status = 0;
  else
    status = 1;

  return status;
}

static int
close_test (void)
{
  int status = 0;

  int flushed_messages;

  QUEUE mq1;
  flushed_messages = mq1.close ();

  if (flushed_messages != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Closing queue should flush 0 messages, close() reports - %d\n"),
                  flushed_messages ));
      status = 1;
      return status;
    }

  // There was a bug that return previous queue state instead of
  // number of flushed messages. Thus, insert 2 messages != ACTIVATE
  // queue state
  ACE_Message_Block *pMB1;
  ACE_Message_Block *pMB2;
  ACE_NEW_NORETURN (pMB1, ACE_Message_Block (1));
  ACE_NEW_NORETURN (pMB2, ACE_Message_Block (1));
  QUEUE mq2;
  mq2.enqueue_head (pMB1);
  mq2.enqueue_head (pMB2);
  flushed_messages = mq2.close ();

  if (flushed_messages != 2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Closing queue should flush 2 messages, close() reports - %d\n"),
                  flushed_messages ));
      status = 1;
      return status;
    }
  return status;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Message_Queue_Test"));

  if (argc == 2)
    {
      if (!ACE_OS::strcmp (argv[1], ACE_TEXT ("-?")))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%s/n"),
                      usage));
        }
      else
        {
          max_messages = ACE_OS::atoi (argv[1]);
        }
    }

  int status = prio_test ();

  // The iterator test occasionally causes a page fault or a hang on
  // VxWorks.
  if (status == 0)
    status = iterator_test ();

  ACE_NEW_RETURN (timer,
                  ACE_High_Res_Timer,
                  -1);

  if (status == 0)
    status = close_test ();

#if defined (ACE_HAS_THREADS)
  if (status == 0)
    status = timeout_test ();

  if (status == 0)
    status = chained_block_test ();

  if (status == 0)
    status = single_thread_performance_test ();

# if defined (ACE_VXWORKS) || defined (ACE_HAS_WIN32_OVERLAPPED_IO)
  // Test ACE_Message_Queue_Vx. or ACE_Message_Queue_NT
  if (status == 0)
    status = single_thread_performance_test (1);
# endif /* ACE_VXWORKS */

  if (status == 0)
    status = performance_test ();

# if defined (ACE_VXWORKS) || defined (ACE_HAS_WIN32_OVERLAPPED_IO)
  // Test ACE_Message_Queue_Vx or ACE_Message_Queue_NT
  if (status == 0)
    status = performance_test (1);
# endif /* ACE_VXWORKS */

  if (counting_test () != 0)
    status = -1;
#endif /* ACE_HAS_THREADS */

  if (status != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("test failed")));
  delete timer;
  timer = 0;



  ACE_END_TEST;
  return status;
}

