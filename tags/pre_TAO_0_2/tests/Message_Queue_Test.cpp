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
//      1) a simple test of the ACE_Message_Queue that illustrates how to
//         use the forward and reverse iterators;
//      2) a simple performance measurement test for both single-threaded
//         (null synch) and thread-safe ACE_Message_Queues, and
//         ACE_Message_Queue_Vx, which wraps VxWorks message queues; and
//      3) a test/usage example of ACE_Message_Queue_Vx.
//
// = AUTHORS
//    Irfan Pyarali and David L. Levine
//
// ============================================================================

#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(tests, Message_Queue_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

const ASYS_TCHAR usage[] = "usage: Message_Queue_Test <number of messages>\n";

typedef ACE_Message_Queue<ACE_NULL_SYNCH> QUEUE;
typedef ACE_Message_Queue_Iterator<ACE_NULL_SYNCH> ITERATOR;
typedef ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH> REVERSE_ITERATOR;

const int MAX_MESSAGES = 10000;
const int MAX_MESSAGE_SIZE = 32;
const ASYS_TCHAR test_message[] = "ACE_Message_Queue Test Message";

int messages = MAX_MESSAGES;

// Dynamically allocate to avoid a static.
static ACE_High_Res_Timer *timer = 0;

#if defined (ACE_HAS_THREADS)
typedef ACE_Message_Queue<ACE_SYNCH> SYNCH_QUEUE;

struct Queue_Wrapper
{
  // = TITLE
  //     Container for data passed to sender and receiver in performance test.
  //
  // = DESCRIPTION
  //     For use in multithreaded performance test.
  union
    {
      SYNCH_QUEUE *sq_;
      // The message queue, synchronized.

      QUEUE *q_;
      // The message queue, unsynchronized.  For VxWorks message queue.
    };
  u_int synch_queue_;
  // Tag field indicating whether to use the synchronized or unsynchronized
  // message queue.

  ACE_Message_Block **send_block_;
  // Pointer to messages blocks for sender to send to reciever.

  Queue_Wrapper () : q_ (0), synch_queue_ (0), send_block_ (0) {}
  // Default constructor.
};

#endif /* ACE_HAS_THREADS */

#if !defined (VXWORKS)
static
int
iterator_test ()
{
  const int ITERATIONS = 5;
  ASYS_TCHAR buffer[ITERATIONS][BUFSIZ];
  QUEUE queue (32 * 1024);
  int i;

  for (i = 0; i < ITERATIONS; i++)
    {
      ACE_OS::sprintf (buffer[i], ASYS_TEXT ("%d"), i+1);

      ACE_Message_Block *entry;
      ACE_NEW_RETURN (entry, ACE_Message_Block ((char *) buffer[i], sizeof buffer[i]), -1);

      if (queue.is_full ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("QUEUE:: the message queue is full on iteration %u!\n"),
                           i+1),
                          -1);

      if (queue.enqueue (entry) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("QUEUE::enqueue\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nForward Iterations\n")));
  {
    ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nReverse Iterations\n")));
  {
    REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nForward Iterations\n")));
  {
    QUEUE::ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nReverse Iterations\n")));
  {
    QUEUE::REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  return 0;
}
#endif /* ! VXWORKS */

#if defined (ACE_HAS_THREADS)

static
int
single_thread_performance_test (int queue_type = 0)
{
  const ASYS_TCHAR test_message[] = "ACE_Message_Queue Test Message";
  const ASYS_TCHAR *message = "ACE_Message_Queue<ACE_NULL_SYNCH>, single thread";
  int i;

  // Create a message queue.
  QUEUE *msgq = 0;

  if (queue_type == 0)
    {
      ACE_NEW_RETURN (msgq, QUEUE, -1);
    }
#if defined (VXWORKS)
  else
    {
      ACE_NEW_RETURN (msgq,
                      ACE_Message_Queue_Vx (messages, MAX_MESSAGE_SIZE),
                      -1);
      message = "ACE_Message_Queue_Vx, single thread";
    }
#endif /* VXWORKS */

  // Create the messages.  Allocate off the heap in case messages
  // is large relative to the amount of stack space available.
  ACE_Message_Block **send_block;
  ACE_NEW_RETURN (send_block, ACE_Message_Block *[messages], -1);

  for (i = 0; i < messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    ACE_Message_Block (test_message, MAX_MESSAGE_SIZE),
                    -1);

  ACE_Message_Block **receive_block_p;
  ACE_NEW_RETURN (receive_block_p, ACE_Message_Block *[messages], -1);

#if defined (VXWORKS)
  // Set up blocks to receive the messages.  Allocate these off the
  // heap in case messages is large relative to the amount of
  // stack space available.
  ACE_Message_Block *receive_block;
  ACE_NEW_RETURN (receive_block, ACE_Message_Block[messages], -1);

  for (i = 0; i < messages; ++i)
    {
      // There's got to be a better way to do this.  init () doesn't
      // delete any existing data block, so we do it explicitly.
      delete receive_block[i].data_block ();
      receive_block[i].init (MAX_MESSAGE_SIZE);

      // For VxWorks Message Queues, the receive block pointer must be
      // assigned.  It will be used by dequeue_head ().
      receive_block_p[i] = &receive_block[i];
    }
#endif /* VXWORKS */

  timer->start ();

  // Send/receive the messages.
  for (i = 0; i < messages; ++i)
    {
      if (msgq->enqueue_tail (send_block[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enqueue"), -1);

      if (msgq->dequeue_head (receive_block_p[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "dequeue_head"), -1);
    }

  timer->stop ();

  ACE_Time_Value tv;
  timer->elapsed_time (tv);
  ACE_DEBUG ((LM_INFO, "%s: %u messages took %u msec (%f msec/message)\n",
              message,
              messages,
              tv.msec (),
              (double) tv.msec () / messages));
  timer->reset ();

  delete [] receive_block_p;
#if defined (VXWORKS)
  delete [] receive_block;
#endif /* VXWORKS */

  for (i = 0; i < messages; ++i)
    delete send_block[i];
  delete [] send_block;
  delete msgq;

  return 0;
}

static
void *
receiver (void *arg)
{
  Queue_Wrapper *queue_wrapper = ACE_reinterpret_cast (Queue_Wrapper *, arg);
  int i;

  ACE_Message_Block **receive_block_p;
  ACE_NEW_RETURN (receive_block_p, ACE_Message_Block *[messages], (void *) -1);

#if defined (VXWORKS)
  // Set up blocks to receive the messages.  Allocate these off the
  // heap in case messages is large relative to the amount of
  // stack space available.
  ACE_Message_Block *receive_block;
  ACE_NEW_RETURN (receive_block, ACE_Message_Block[messages], (void *) -1);

  for (i = 0; i < messages; ++i)
    {
      // There's got to be a better way to do this.  init () doesn't
      // delete any existing data block, so we do it explicitly.
      delete receive_block[i].data_block ();
      receive_block[i].init (MAX_MESSAGE_SIZE);

      // For VxWorks Message Queues, the receive block pointer must be
      // assigned.  It will be used by dequeue_head ().
      receive_block_p[i] = &receive_block[i];
    }
#endif /* VXWORKS */

  for (i = 0; i < messages; ++i)
    if (queue_wrapper->synch_queue_)
      { // Do not remove the brace!!!!
        if (queue_wrapper->sq_->dequeue_head (receive_block_p[i]) == -1)
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "dequeue_head"), 0);
      } // Do not remove the brace!!!!
    else
      if (queue_wrapper->q_->dequeue_head (receive_block_p[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "dequeue_head"), 0);

  timer->stop ();

  delete [] receive_block_p;
#if defined (VXWORKS)
  delete [] receive_block;
#endif /* VXWORKS */

  return 0;
}

static
void *
sender (void *arg)
{
  Queue_Wrapper *queue_wrapper = ACE_reinterpret_cast (Queue_Wrapper *, arg);
  int i;

  timer->start ();

  // Send the messages.
  for (i = 0; i < messages; ++i)
    if (queue_wrapper->synch_queue_)
      { // Do not remove the brace!!!!
        if (queue_wrapper->sq_->
              enqueue_tail (queue_wrapper->send_block_[i]) == -1)
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enqueue"), 0);
      } // Do not remove the brace!!!!
    else
      if (queue_wrapper->q_->
            enqueue_tail (queue_wrapper->send_block_[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enqueue"), 0);

  return 0;
}

static
int
performance_test (int queue_type = 0)
{
  Queue_Wrapper queue_wrapper;
  const ASYS_TCHAR *message = "ACE_Message_Queue<ACE_SYNCH>";
  int i;

  // Create the messages.  Allocate off the heap in case messages
  // is large relative to the amount of stack space available.
  // Allocate it here instead of in the sender, so that we can
  // delete it after the _receiver_ is done.
  ACE_Message_Block **send_block;
  ACE_NEW_RETURN (send_block, ACE_Message_Block *[messages], -1);

  for (i = 0; i < messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    ACE_Message_Block (test_message, MAX_MESSAGE_SIZE),
                    -1);

  queue_wrapper.send_block_ = send_block;

  if (queue_type == 0)
    {
      queue_wrapper.synch_queue_ = 1;
      ACE_NEW_RETURN (queue_wrapper.sq_, SYNCH_QUEUE, -1);
    }
#if defined (VXWORKS)
  else
    {
      queue_wrapper.synch_queue_ = 0;
      ACE_NEW_RETURN (queue_wrapper.q_,
                      ACE_Message_Queue_Vx (messages, MAX_MESSAGE_SIZE),
                      -1);
      message = "ACE_Message_Queue_Vx";
    }
#endif /* VXWORKS */

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) sender,
                                              &queue_wrapper,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", "spawning sender thread"),
                      -1);

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) receiver,
                                              &queue_wrapper,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", "spawning receiver thread"),
                      -1);

  ACE_Thread_Manager::instance ()->wait ();
  ACE_Time_Value tv;
  timer->elapsed_time (tv);
  ACE_DEBUG ((LM_INFO, "%s: %u messages took %u msec (%f msec/message)\n",
              message,
              messages,
              tv.msec (),
              (double) tv.msec () / messages));
  timer->reset ();

  if (queue_type == 0)
    {
      delete queue_wrapper.sq_;
      queue_wrapper.sq_ = 0;
    }
#if defined (VXWORKS)
  else
    {
      delete queue_wrapper.q_;
      queue_wrapper.q_ = 0;
    }
#endif /* VXWORKS */

  for (i = 0; i < messages; ++i)
    delete send_block[i];
  delete [] send_block;

  return 0;
}
#endif /* ACE_HAS_THREADS */

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Message_Queue_Test"));

  int status = 0;

  if (argc == 2)
    if (! ACE_OS::strcmp (argv[1], "-?"))
      ACE_OS::printf (usage);
    else
      messages = ACE_OS::atoi (argv[1]);

#if !defined (VXWORKS)
  // The iterator test occasionally causes a page fault or a hang on VxWorks.
  if (status == 0)
    status = iterator_test ();
#endif /* ! VXWORKS */

  ACE_NEW_RETURN (timer, ACE_High_Res_Timer, -1);

#if defined (ACE_HAS_THREADS)
  if (status == 0)
    single_thread_performance_test ();

# if defined (VXWORKS)
  // Test ACE_Message_Queue_Vx.
  if (status == 0)
    single_thread_performance_test (1);
# endif /* VXWORKS */

  if (status == 0)
    performance_test ();

# if defined (VXWORKS)
  // Test ACE_Message_Queue_Vx.
  if (status == 0)
    performance_test (1);
# endif /* VXWORKS */
#endif /* ACE_HAS_THREADS */

  if (status != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "test failed"));

  delete timer;
  timer = 0;

  ACE_END_TEST;
  return status;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>
#pragma instantiate ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
