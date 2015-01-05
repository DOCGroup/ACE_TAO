
//=============================================================================
/**
 *  @file    Message_Queue_Test_Ex.cpp
 *
 *  $Id$
 *
 *    This is:
 *    1. A simple test of the ACE_Message_Queue_Ex that executes
 *       a performance measurement test for both single-threaded
 *       (null synch) and thread-safe ACE_Message_Queue_Ex
 *       instantiations.
 *    2. An example of using a user-defined class to parameterize
 *       ACE_Message_Queue_Ex.
 *
 *
 *  @author Michael Vitlo <mvitalo@sprynet.com>
 *  @author copied the code from: Irfan Pyarali <irfan@cs.wustl.edu> and David L. Levine <levine@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Thread_Manager.h"

#include "ace/Auto_Ptr.h"
#include "ace/Message_Queue.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Null_Condition.h"
#include "ace/High_Res_Timer.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Barrier.h"
#include "Message_Queue_Test_Ex.h"    // Declares User_Class

const ACE_TCHAR usage[] =
  ACE_TEXT ("usage: Message_Queue_Test_Ex <number of messages>\n");

typedef ACE_Message_Queue_Ex<User_Class, ACE_NULL_SYNCH> QUEUE;

static const int MAX_MESSAGES = 10000;
static const char test_message[] = "ACE_Message_Queue_Ex Test Message";

static int max_messages = MAX_MESSAGES;
static int chain_limit = 4;
static ACE_Barrier tester_barrier (2);

// Dynamically allocate to avoid a static.
static ACE_High_Res_Timer *timer = 0;

// Helper printing function
static void
print_message (const ACE_TCHAR *message)
{
  ACE_Time_Value tv;
  timer->elapsed_time (tv);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%s: %u messages took %u msec (%f msec/message)\n"),
              message,
              max_messages,
              tv.msec (),
              (double) tv.msec () / max_messages));
}

#if defined (ACE_HAS_THREADS)
typedef ACE_Message_Queue_Ex<User_Class, ACE_MT_SYNCH> SYNCH_QUEUE;

/**
 * Container for data passed to sender and receiver in
 * performance test.
 *
 * For use in multithreaded performance test.
 */
struct Queue_Wrapper
{
  /// The message queue.
  SYNCH_QUEUE *q_;

  /// Pointer to messages blocks for sender to send to reciever.
  User_Class **send_block_;

  /// Default constructor.
  Queue_Wrapper (void)
    : q_ (0), send_block_ (0)
  {
  }
};

struct MQ_Ex_N_Tester_Wrapper
{
  //  = TITLE
  //      Container for data passed to sender in the MQ_Ex_N_Tester
  //      performance test.
  //
  //  = DESCRIPTION
  //     For use in multithreaded performance test.
  MQ_Ex_N_Tester *tester_;
  User_Class     *head_send_block_;
};

#endif /* ACE_HAS_THREADS */

// Encapsulates the sent messages creation and destruction
struct Send_Messages
{
  Send_Messages (int number_of_messages, int chain_limit):
    send_block_ (0),
    number_of_messages_ (number_of_messages),
    chain_limit_ (chain_limit)
  {
  }

  int create_messages (const char test_message[])
  {
    int limit = this->number_of_messages_ / this->chain_limit_;
    ACE_NEW_RETURN (this->send_block_,
                    User_Class *[limit],
                    -1);

    int i, j;
    for (i = 0; i < limit; ++i)
      {
        User_Class *&temp1 = this->send_block_[i];
        ACE_NEW_RETURN (temp1,
                        User_Class (test_message),
                        -1);
        User_Class *tail = temp1;
        for (j = 1; j < this->chain_limit_; ++j)
          {
            User_Class *temp2 = 0;
            ACE_NEW_RETURN (temp2,
                            User_Class (test_message),
                            -1);
            tail->next (temp2);
            tail = temp2;
          }
      }
    this->head_send_block_ = this->send_block_[0];
    return 0;
  }

  ~Send_Messages ()
  {
    int j, i = 0;
    int limit = this->number_of_messages_ / this->chain_limit_;
    for (; i < limit; ++i)
      {
        User_Class *&temp1 = this->send_block_[i];
        for (j = 0; j < this->chain_limit_; ++j)
          {
            User_Class *temp2 = temp1->next ();
            delete temp1;
            temp1 = temp2;
          }
      }
    delete [] this->send_block_;
  }

  User_Class *  head_send_block_;
  User_Class ** send_block_;
  int           number_of_messages_;
  int           chain_limit_;
};

// Encapsulates the received messages creation and destruction
struct Receive_Messages
{
  Receive_Messages (int number_of_messages) :
    receive_block_ (0),
    number_of_messages_ (number_of_messages)
  {
  }

  int create (void)
  {
    ACE_NEW_RETURN (this->receive_block_,
                    User_Class *[this->number_of_messages_],
                    -1);
    return 0;
  }

  ~Receive_Messages ()
  {
    delete [] this->receive_block_;
  }

  User_Class **receive_block_;
  int          number_of_messages_;
};

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

  int i = 0;

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
  print_message (message);
  timer->reset ();

  delete [] receive_block_p;

  for (i = 0; i < max_messages; ++i)
    delete send_block[i];
  delete [] send_block;
  delete msgq;

  return 0;
}

int
MQ_Ex_N_Tester::single_thread_performance_test (void)
{
  // Create the messages.  Allocate off the heap in case messages is
  // large relative to the amount of stack space available.

  if ((0 != this->test_enqueue_tail ()) ||
      (0 != this->test_enqueue_head ())    )
    {
      return -1;
    }

  return 0;
}

int
MQ_Ex_N_Tester::test_enqueue_tail (void)
{
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex_N<ACE_NULL_SYNCH>, test_enqueue_tail");

  // Send_Messages creates messages and deletes them when it gets out of scope
  Send_Messages messages (max_messages, chain_limit);
  if (-1 == messages.create_messages (test_message))
    {
      return -1;
    }
  Receive_Messages r_messages (max_messages);
  if (-1 == r_messages.create ())
    {
      return -1;
    }

  // prepare
  int limit = max_messages / chain_limit;
  timer->start ();
  // Send with just one call
  for (int i = 0; i < limit; ++i)
    {
      if (-1 == this->st_queue_.enqueue_tail (messages.send_block_[i]))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("enqueue_tail_n")),
                            -1);
        }

      for (int j = 0, k = 0; j < chain_limit; ++j, ++k)
        {
          if (this->st_queue_.dequeue_head (r_messages.receive_block_[k]) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("%p\n"),
                                 ACE_TEXT ("dequeue_head")),
                                -1);
            }
        }
    }
  timer->stop ();

  print_message (message);

  timer->reset ();

  return 0;
}

int
MQ_Ex_N_Tester::test_enqueue_head (void)
{
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex_N<ACE_NULL_SYNCH>, test_enqueue_head");

  // Send_Messages creates messages and deletes them when it gets out of scope
  Send_Messages messages (max_messages, chain_limit);
  if (-1 == messages.create_messages (test_message))
    {
      return -1;
    }
  Receive_Messages r_messages (max_messages);
  if (-1 == r_messages.create ())
    {
      return -1;
    }

  // prepare
  int i, j, k = 0;

  int limit = max_messages / chain_limit;
  timer->start ();

  // Send/receive the messages.
  // Send with just one call
  for (i = 0; i < limit; ++i)
    {
      if (-1 == this->st_queue_.enqueue_head (messages.send_block_[i]))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("enqueue_tail_n")),
                            -1);
        }

      for (j = 0; j < chain_limit; ++j, ++k)
        {
          if (this->st_queue_.dequeue_head (r_messages.receive_block_[k]) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("%p\n"),
                                 ACE_TEXT ("dequeue_head")),
                                -1);
            }
        }
    }
  timer->stop ();

  print_message (message);

  timer->reset ();

  return 0;

}

#if defined (ACE_HAS_THREADS)

static void *
receiver (void *arg)
{
  Queue_Wrapper *queue_wrapper = reinterpret_cast<Queue_Wrapper *> (arg);
  int i;

  User_Class **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  User_Class *[max_messages],
                  (void *) -1);

  for (i = 0; i < max_messages; ++i)
    if (queue_wrapper->q_->dequeue_head (receive_block_p[i]) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("dequeue_head")),
                        0);
  timer->stop ();

  delete [] receive_block_p;

  return 0;
}

static void *
sender (void *arg)
{
  Queue_Wrapper *queue_wrapper = reinterpret_cast<Queue_Wrapper *> (arg);
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

static int
performance_test (void)
{
  Queue_Wrapper queue_wrapper;
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex<ACE_SYNCH>");
  int i = 0;

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
  print_message (message);
  timer->reset ();

  delete queue_wrapper.q_;
  queue_wrapper.q_ = 0;

  for (i = 0; i < max_messages; ++i)
    delete send_block[i];
  delete [] send_block;

  return 0;
}

int
MQ_Ex_N_Tester::performance_test (void)
{
  const ACE_TCHAR *message =
    ACE_TEXT ("ACE_Message_Queue_Ex_N<ACE_SYNCH>");

  Send_Messages messages (max_messages, chain_limit);
  if (-1 == messages.create_messages (test_message))
    {
      return -1;
    }

  MQ_Ex_N_Tester_Wrapper tester_wrapper;
  tester_wrapper.head_send_block_ = messages.head_send_block_;
  tester_wrapper.tester_ = this;

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) &MQ_Ex_N_Tester::sender,
                                              &tester_wrapper,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawning sender thread")),
                      -1);

  if (ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) &MQ_Ex_N_Tester::receiver,
                                              this,
                                              THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawning receiver thread")),
                      -1);

  ACE_Thread_Manager::instance ()->wait ();

  print_message (message);

  timer->reset ();

  return 0;
}

ACE_THR_FUNC_RETURN
MQ_Ex_N_Tester::receiver (void *args)
{
  MQ_Ex_N_Tester *tester = reinterpret_cast<MQ_Ex_N_Tester *> (args);

  User_Class **receive_block_p = 0;
  ACE_NEW_RETURN (receive_block_p,
                  User_Class *[max_messages],
                  (ACE_THR_FUNC_RETURN) -1);

  int i;
  tester_barrier.wait ();
  for (i = 0; i < max_messages; ++i)
    {
      if (tester->mt_queue_.dequeue_head (receive_block_p[i]) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("dequeue_head")));
          return (ACE_THR_FUNC_RETURN) -1;
        }
    }
  timer->stop ();

  delete [] receive_block_p;

  return 0;
}

ACE_THR_FUNC_RETURN
MQ_Ex_N_Tester::sender (void *args)
{
  MQ_Ex_N_Tester_Wrapper *tester_wrapper =
    reinterpret_cast<MQ_Ex_N_Tester_Wrapper *> (args);
  MQ_Ex_N_Tester *tester = tester_wrapper->tester_;

  Send_Messages messages (max_messages, chain_limit);
  if (-1 == messages.create_messages (test_message))
    {
      return (ACE_THR_FUNC_RETURN) -1;
    }
  int limit = max_messages / chain_limit;
  tester_barrier.wait ();
  timer->start ();
  // Send/receive the messages.
  timer->start ();
  // Send with just one call
  for (int i = 0; i < limit; ++i)
    {
      if (-1 == tester->mt_queue_.enqueue_tail (messages.send_block_[i]))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("enqueue_tail_n")));
          return (ACE_THR_FUNC_RETURN) -1;
        }
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */

int basic_queue_test (ACE_Message_Queue_Ex<User_Class, ACE_SYNCH>& q)
{
    int status = 0;
    if (!q.is_empty ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("New queue is not empty!\n")));
        status = 1;
      }
    else
      {
        User_Class *b;
        ACE_Time_Value tv (ACE_OS::gettimeofday ());   // Now
        if (q.dequeue_head (b, &tv) != -1)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Dequeued from empty queue!\n")));
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
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Timed dequeue test: OK\n")));
            status = 0;     // All is well
          }
      }

    return status;
}

int queue_priority_test (ACE_Message_Queue_Ex<User_Class, ACE_SYNCH>& q)
{
  int status = 0;
  if (!q.is_empty ())
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Prio test queue not empty\n")), 1);

  // Set up a few objects with names for how they should come out of the queue.
  ACE_Auto_Basic_Ptr<User_Class> b1, b2, b3, b4;
  b1.reset (new User_Class ("first"));
  b2.reset (new User_Class ("second"));
  b3.reset (new User_Class ("third"));
  b4.reset (new User_Class ("fourth"));
  unsigned long prio =
    ACE_Message_Queue_Ex<User_Class, ACE_SYNCH>::DEFAULT_PRIORITY;

  prio += 1;
  if (-1 == q.enqueue_prio (b2.get (), 0, prio))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("b2")), 1);
  if (-1 == q.enqueue_prio (b3.get (), 0, prio))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("b3")), 1);
  prio -= 1;
  if (-1 == q.enqueue_prio (b4.get (), 0, prio))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("b4")), 1);
  prio += 5;
  if (-1 == q.enqueue_prio (b1.get (), 0, prio))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("b1")), 1);

  User_Class *b = 0;
  if (q.dequeue_head (b) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("dequeue 1")));
      ++status;
    }
  else
    {
      if (ACE_OS::strcmp (b->message (), "first") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("First dequeued was %C\n"),
                      b->message ()));
          ++status;
        }
    }
  if (q.dequeue_head (b) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("dequeue 2")));
      ++status;
    }
  else
    {
      if (ACE_OS::strcmp (b->message (), "second") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Second dequeued was %C\n"),
                      b->message ()));
          ++status;
        }
    }
  if (q.dequeue_head (b) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("dequeue 3")));
      ++status;
    }
  else
    {
      if (ACE_OS::strcmp (b->message (), "third") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Third dequeued was %C\n"),
                      b->message ()));
          ++status;
        }
    }
  if (q.dequeue_head (b) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("dequeue 4")));
      ++status;
    }
  else
    {
      if (ACE_OS::strcmp (b->message (), "fourth") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Fourth dequeued was %C\n"),
                      b->message ()));
          ++status;
        }
    }

  if (status == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Priority queueing test: OK\n")));
  return status;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Message_Queue_Test_Ex"));

  if (argc == 2)
    {
      if (! ACE_OS::strcmp (argv[1], ACE_TEXT ("-?")))
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

  int status = 0;

  // Be sure that the a timed out get sets the error code properly.
  ACE_Message_Queue_Ex<User_Class, ACE_SYNCH> q1;
  ACE_Message_Queue_Ex_N<User_Class, ACE_SYNCH> q2;
  if (0 != basic_queue_test (q1) ||
      0 != basic_queue_test (q2))
    {
      ++status;
    }

  // Check priority operations.
  if (0 != queue_priority_test (q1))
    {
      ++status;
    }

  ACE_NEW_RETURN (timer,
                  ACE_High_Res_Timer,
                  -1);

  status += single_thread_performance_test ();

#if defined (ACE_HAS_THREADS)
  status += performance_test ();
#endif /* ACE_HAS_THREADS */

  {
    MQ_Ex_N_Tester ex_n_tester;
    status += ex_n_tester.single_thread_performance_test ();
#if defined (ACE_HAS_THREADS)
    status += ex_n_tester.performance_test ();
#endif /* ACE_HAS_THREADS */
    }

  if (status != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("test failed")));
  delete timer;
  timer = 0;

  ACE_END_TEST;
  return status;
}
