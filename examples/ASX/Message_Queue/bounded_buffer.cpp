// $Id$

// This short program copies stdin to stdout via the use of an ASX
// Message_Queue.  It illustrates an implementation of the classic
// "bounded buffer" program.

#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_THREADS)

// The producer reads data from the stdin stream, creates a message,
// and then queues the message in the message list, where it is
// removed by the consumer thread.  A 0-sized message is enqueued when
// there is no more data to read.  The consumer uses this as a flag to
// know when to exit.

static void *
producer (ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue)
{
  // Keep reading stdin, until we reach EOF.

  for (int n; ; )
    {
      // Allocate a new message.
      ACE_Message_Block *mb = 0;

      ACE_NEW_RETURN (mb, ACE_Message_Block (BUFSIZ), 0);

      n = ACE_OS::read (ACE_STDIN, mb->wr_ptr (), mb->size ());

      if (n <= 0)
        {
          // Send a shutdown message to the other thread and exit.
          mb->length (0);
          if (msg_queue->enqueue_tail (mb) == -1)
            ACE_ERROR ((LM_ERROR,
                        "(%t) %p\n",
                        "put_next"));
          break;
        }
      // Send the message to the other thread.
      else
        {
          mb->msg_priority (n);
          mb->wr_ptr (n);
          if (msg_queue->enqueue_tail (mb) == -1)
            ACE_ERROR ((LM_ERROR,
                        "(%t) %p\n",
                        "put_next"));
        }
    }

  return 0;
}

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// producer sends a 0-sized message to inform the consumer to stop
// reading and exit.

static void *consumer (ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue)
{
  int result = 0;

  // Keep looping, reading a message out of the queue, until we timeout
  // or get a message with a length == 0, which signals us to quit.

  for (;;)
    {
      ACE_Message_Block *mb = 0;

      ACE_Time_Value timeout (ACE_OS::time (0) + 4, 0); // Wait for upto 4 seconds

      result = msg_queue->dequeue_head (mb, &timeout);

      if (result == -1)
        break;

      int length = ACE_Utils::truncate_cast<int> (mb->length ());

      if (length > 0)
        ACE_OS::write (ACE_STDOUT, mb->rd_ptr (), length);

      mb->release ();

      if (length == 0)
        break;
    }

  if (result == -1 && errno == EWOULDBLOCK)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n%a",
                "timed out waiting for message",
                1));
  return 0;
}

// Spawn off two threads that copy stdin to stdout.

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Message list.
  ACE_Message_Queue<ACE_MT_SYNCH> msg_queue;

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (producer),
       (void *) &msg_queue,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      1);
  else if (ACE_Thread_Manager::instance ()->spawn
           (ACE_THR_FUNC (consumer),
            (void *) &msg_queue,
            THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      1);

  // Wait for producer and consumer threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
