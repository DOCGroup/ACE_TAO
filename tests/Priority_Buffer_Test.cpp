// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Priority_Buffer_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate the priority mechanism of
//      ACE Message_Queues. The producer uses an ASX Message_Queue to
//      enqueue a bunch of messages with different priorities which
//      are then dequeued by the consumer.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================


#include "ace/Message_Queue.h"
#include "ace/Service_Config.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Global message count.
static int count = 0;

// Make the queue be capable of being *very* large.
static const long max_queue = LONG_MAX;

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// producer sends a 0-sized message to inform the consumer to stop
// reading and exit.

static void *
consumer (void *args)
{
  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue = (ACE_Message_Queue<ACE_MT_SYNCH> *) args;

  u_long cur_priority = 27;
  int local_count = 0;

  // Keep looping, reading a message out of the queue, until we
  // get a message with a length == 0, which signals us to quit.
  for (char c = 'z'; ; c--)
    {
      ACE_Message_Block *mb = 0;

      int result = msg_queue->dequeue_head (mb);

      if (result == -1)
        break;

      local_count++;

      int length = mb->length ();
      ACE_ASSERT (mb->msg_priority () < cur_priority);
      cur_priority = mb->msg_priority ();

      if (length > 0)
	ACE_ASSERT (c == *mb->rd_ptr ());

      // Free up the buffer memory and the Message_Block. Note that
      // the destructor of Message Block will delete the the actual
      // buffer.
      mb->release ();

      if (length == 0)
	break;
    }
  ACE_ASSERT (local_count == count);
  return 0;
}

// The producer reads data from the stdin stream, creates a message,
// and then queues the message in the message list, where it is
// removed by the consumer thread.  A 0-sized message is enqueued when
// there is no more data to read.  The consumer uses this as a flag to
// know when to exit.

static void *
producer (void *args)
{
  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue = (ACE_Message_Queue<ACE_MT_SYNCH> *) args;

  // Insert thread into thr_mgr.
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;

  ACE_Message_Block *mb = 0;

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    {
      count++;

      // Allocate a new message
      
      ACE_NEW_RETURN (mb, ACE_Message_Block (1), 0);
      *mb->rd_ptr () = *c;

      // Set the priority.
      mb->msg_priority (count);
      mb->wr_ptr (1);
      
      // Enqueue in priority order.
      if (msg_queue->enqueue_prio (mb) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "put_next"), 0);
    }

  // Now send a 0-sized shutdown message to the other thread
  ACE_NEW_RETURN (mb, ACE_Message_Block ((size_t) 0), 0);

  if (msg_queue->enqueue_tail (mb) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put_next"));

  count++;

  // Now read all the items out in priority order (i.e., ordered by
  // the size of the lines!).
  consumer (msg_queue);

  // The destructor of ACE_Thread_Control removes the exiting thread
  // from the thr_mgr automatically.
  return 0; 
}

#endif /* ACE_HAS_THREADS */

// Spawn off one thread that copies stdin to stdout in order of the
// size of each line.

int 
main (int, char *[])
{
  ACE_START_TEST ("Priority_Buffer_Test");

#if defined (ACE_HAS_THREADS)
  // Message queue.
  ACE_Message_Queue<ACE_MT_SYNCH> msg_queue (max_queue);

  if (ACE_Service_Config::thr_mgr ()->spawn (ACE_THR_FUNC (producer), 
					     (void *) &msg_queue,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  // Wait for producer and consumer threads to exit.
  ACE_Service_Config::thr_mgr ()->wait ();
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
