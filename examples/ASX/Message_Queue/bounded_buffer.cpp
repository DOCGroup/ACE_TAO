// This short program copies stdin to stdout via the use of an ASX
// @(#)bounded_buffer.cpp	1.1	10/18/96

// Message_Queue.  It illustrates an implementation of the classic
// "bounded buffer" program.

#include "ace/Log_Msg.h"
#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)

// Global thread manager.
static ACE_Thread_Manager thr_mgr;

// Message list.
static ACE_Message_Queue<ACE_MT_SYNCH> msg_queue;

// The producer reads data from the stdin stream, creates a message,
// and then queues the message in the message list, where it is
// removed by the consumer thread.  A 0-sized message is enqueued when
// there is no more data to read.  The consumer uses this as a flag to
// know when to exit. 

static void *
producer (ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue)
{
  // Insert thread into thr_mgr.
  ACE_Thread_Control thread_control (&thr_mgr); 

  // Keep reading stdin, until we reach EOF. 

  for (int n; ; )
    {
      // Allocate a new message.
      ACE_Message_Block *mb = new ACE_Message_Block (BUFSIZ);

      n = ACE_OS::read (ACE_STDIN, mb->rd_ptr (), mb->size ());

      if (n <= 0)
        {
          // Send a shutdown message to the other thread and exit.
          mb->length (0);
          if (msg_queue->enqueue_tail (mb) == -1)
            ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put_next"));
      	  break;
        }

      // Send the message to the other thread.
      else
	{
	  mb->msg_priority (n);
	  mb->wr_ptr (n);
	  if (msg_queue->enqueue_tail (mb) == -1)
	    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put_next"));
	}
    }

  // The destructor of ACE_Thread_Control removes the exiting thread
  // from the thr_mgr automatically.
  return 0; 
}

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// producer sends a 0-sized message to inform the consumer to stop
// reading and exit. 

static void *consumer (ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue)
{
  // Insert thread into thr_mgr.
  ACE_Thread_Control thread_control (&thr_mgr); 
  int result = 0;

  // Keep looping, reading a message out of the queue, until we timeout
  // or get a message with a length == 0, which signals us to quit.

  for (;;)
    {
      ACE_Message_Block *mb;

      ACE_Time_Value timeout (ACE_OS::time (0) + 4, 0); // Wait for upto 4 seconds

      result = msg_queue->dequeue_head (mb, &timeout);

      if (result == -1)
	break;

      int length = mb->length ();

      if (length > 0)
	ACE_OS::write (ACE_STDOUT, mb->rd_ptr (), length);

      delete mb;

      if (length == 0)
	break;
    }

  if (result == -1 && errno == EWOULDBLOCK)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n%a", "timed out waiting for message", 1));

  // The destructor of ACE_Thread_Control removes the exiting thread
  // from the thr_mgr automatically.
  return 0;
}

/* Spawn off two threads that copy stdin to stdout. */

int main (int, char *[])
{
  if (thr_mgr.spawn (ACE_THR_FUNC (producer), (void *) &msg_queue,
			 THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);
  else if (thr_mgr.spawn (ACE_THR_FUNC (consumer), (void *) &msg_queue,
			      THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  // Wait for producer and consumer threads to exit.
  thr_mgr.wait ();
  return 0;
}
#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
