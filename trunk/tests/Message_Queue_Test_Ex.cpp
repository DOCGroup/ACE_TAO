// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Message_Queue_Test_Ex.cpp
//
// = DESCRIPTION
//      This is:
//      1. A simple test of the ACE_Message_Queue_Ex that executes
//         a performance measurement test for both single-threaded
//         (null synch) and thread-safe ACE_Message_Queue_Ex
//         instantiations.
//      2. An example of using a user-defined class to parameterize
//         ACE_Message_Queue_Ex.
//
// = AUTHORS
//    Michael Vitlo <mvitalo@sprynet.com>, copied the code from:
//    Irfan Pyarali <irfan@cs.wustl.edu> and David L. Levine <levine@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/High_Res_Timer.h"
#include "ace/Message_Block.h"

// User-defined class used for queue data.
class User_Class
{
public:
  User_Class (const char inputMsg[])
    : message_ (0)
  {
    ACE_NEW (this->message_, char[ACE_OS::strlen (inputMsg) + 1]);
    ACE_OS::strcpy (this->message_, inputMsg);
  }

  ~User_Class (void) { delete [] this->message_; }
private:
  char *message_;
};

const ACE_TCHAR usage[] = ACE_TEXT ("usage: Message_Queue_Test_Ex <number of messages>\n");

typedef ACE_Message_Queue_Ex<User_Class, ACE_NULL_SYNCH> QUEUE;

static const int MAX_MESSAGES = 10000;
static const char test_message[] = "ACE_Message_Queue_Ex Test Message";

static int max_messages = MAX_MESSAGES;

// Dynamically allocate to avoid a static.
static ACE_High_Res_Timer *timer = 0;

#if defined (ACE_HAS_THREADS)
typedef ACE_Message_Queue_Ex<User_Class, ACE_MT_SYNCH> SYNCH_QUEUE;

struct Queue_Wrapper
{
  // = TITLE
  //     Container for data passed to sender and receiver in
  //     performance test.
  //
  // = DESCRIPTION
  //     For use in multithreaded performance test.

  SYNCH_QUEUE *q_;
  // The message queue.

  User_Class **send_block_;
  // Pointer to messages blocks for sender to send to reciever.

  Queue_Wrapper (void)
    : q_ (0), send_block_ (0)
  {
  }
  // Default constructor.
};

#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_THREADS)

static int
single_thread_performance_test (void)
{
  const char test_message[] =
    "ACE_Message_Queue_Ex Test Message";
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex<ACE_NULL_SYNCH>, single thread");

  // Create a message queue.
  QUEUE *msgq = 0;

  ACE_NEW_RETURN (msgq,
                  QUEUE,
                  -1);

  // Create the messages.  Allocate off the heap in case messages is
  // large relative to the amount of stack space available.
  User_Class **send_block = 0;
  ACE_NEW_RETURN (send_block,
                  User_Class *[max_messages],
                  -1);

  int i;

  for (i = 0; i < max_messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    User_Class (test_message),
                    -1);

  User_Class **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  User_Class *[max_messages],
                  -1);

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
    ACE_reinterpret_cast (Queue_Wrapper *,
                          arg);
  int i;

  User_Class **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  User_Class *[max_messages],
                  (void *) -1);

#if defined (VXWORKS)
  // Set up blocks to receive the messages.  Allocate these off the
  // heap in case messages is large relative to the amount of stack
  // space available.
  User_Class *receive_block;
  ACE_NEW_RETURN (receive_block,
                  User_Class[max_messages],
                  (void *) -1);

  for (i = 0; i < max_messages; ++i)
    {
      receive_block[i].init (MAX_MESSAGE_SIZE);

      // For VxWorks Message Queues, the receive block pointer must be
      // assigned.  It will be used by <dequeue_head>.
      receive_block_p[i] = &receive_block[i];
    }
#endif /* VXWORKS */

  for (i = 0; i < max_messages; ++i)
    if (queue_wrapper->q_->dequeue_head (receive_block_p[i]) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("dequeue_head")),
                        0);
  timer->stop ();

  delete [] receive_block_p;
#if defined (VXWORKS)
  delete [] receive_block;
#endif /* VXWORKS */

  return 0;
}

static void *
sender (void *arg)
{
  Queue_Wrapper *queue_wrapper =
    ACE_reinterpret_cast (Queue_Wrapper *, arg);
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
performance_test (void)
{
  Queue_Wrapper queue_wrapper;
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex<ACE_SYNCH>");
  int i;

  // Create the messages.  Allocate off the heap in case messages is
  // large relative to the amount of stack space available.  Allocate
  // it here instead of in the sender, so that we can delete it after
  // the _receiver_ is done.
  User_Class **send_block = 0;
  ACE_NEW_RETURN (send_block,
                  User_Class *[max_messages],
                  -1);

  for (i = 0; i < max_messages; ++i)
    ACE_NEW_RETURN (send_block[i],
                    User_Class (test_message),
                    -1);

  queue_wrapper.send_block_ = send_block;

  ACE_NEW_RETURN (queue_wrapper.q_,
                    SYNCH_QUEUE,
                    -1);

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
#endif /* ACE_HAS_THREADS */

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Message_Queue_Test_Ex"));

  int status = 0;

  if (argc == 2)
    if (! ACE_OS::strcmp (argv[1], ACE_TEXT ("-?")))
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%s/n"),
                  usage));
    else
      max_messages = ACE_OS::atoi (argv[1]);

  // Be sure that the a timed out get sets the error code properly.
  ACE_Message_Queue_Ex<User_Class, ACE_SYNCH> q1;
  if (!q1.is_empty ()) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("New queue is not empty!\n")));
    status = 1;
  }
  else {
    User_Class *b;
    ACE_Time_Value tv (ACE_OS::gettimeofday ());   // Now
    if (q1.dequeue_head (b, &tv) != -1) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Dequeued from empty queue!\n")));
      status = 1;
    }
    else if (errno != EWOULDBLOCK) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Dequeue timeout should be EWOULDBLOCK, got")));
      status = 1;
    }
    else {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Timed dequeue test: OK\n")));
      status = 0;     // All is well
    }
  }

#if !defined (VXWORKS)
#endif /* ! VXWORKS */
  ACE_NEW_RETURN (timer,
                  ACE_High_Res_Timer,
                  -1);
#if defined (ACE_HAS_THREADS)
  if (status == 0)
    single_thread_performance_test ();

  if (status == 0)
    performance_test ();
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Message_Queue_Ex<User_Class, ACE_NULL_SYNCH>;
#if defined (ACE_HAS_THREADS)
template class ACE_Message_Queue_Ex<User_Class, ACE_MT_SYNCH>;
#endif /* ACE_HAS_THREADS */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Message_Queue_Ex<User_Class, ACE_NULL_SYNCH>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Message_Queue_Ex<User_Class, ACE_MT_SYNCH>
#endif /* ACE_HAS_THREADS */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
