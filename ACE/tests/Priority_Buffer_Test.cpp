
//=============================================================================
/**
 *  @file    Priority_Buffer_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test to illustrate the priority mechanism of
 *    <ACE_Message_Queue>s. The producer uses an <ACE_Message_Queue>
 *    to enqueue a bunch of messages with different priorities which
 *    are then dequeued by the consumer.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Message_Queue.h"
#include "ace/Thread_Manager.h"



#if defined (ACE_HAS_THREADS)

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// Global message count.
static int message_count = 0;

// Make the queue be capable of being *very* large.
static const long max_queue = LONG_MAX;

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// producer sends a 0-sized message to inform the consumer to stop
// reading and exit.

static void *
consumer (void *args)
{
  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue =
    reinterpret_cast<ACE_Message_Queue<ACE_MT_SYNCH> *> (args);

  u_long cur_priority = 27;
  ACE_UNUSED_ARG (cur_priority);
  // To suppress ghs warning about unused local variable
  // "cur_priority".

  int local_count = 0;

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.
  for (char c = 'z'; ; c--)
    {
      ACE_Message_Block *mb = 0;

      int result = msg_queue->dequeue_head (mb);

      if (result == -1)
        break;

      local_count++;

      size_t length = mb->length ();

      if (length > 0)
        {
          // This isn't a "shutdown" message, so process it
          // "normally."
          ACE_TEST_ASSERT (c == *mb->rd_ptr ());
          ACE_TEST_ASSERT (mb->msg_priority () < cur_priority);
          cur_priority = mb->msg_priority ();
        }

      // Free up the buffer memory and the Message_Block. Note that
      // the destructor of Message Block will delete the the actual
      // buffer.
      mb->release ();

      if (length == 0)
        // This was a "shutdown" message, so break out of the loop.
        break;
    }

  ACE_TEST_ASSERT (local_count == message_count);
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
  ACE_Message_Queue<ACE_MT_SYNCH> *msg_queue =
    reinterpret_cast<ACE_Message_Queue<ACE_MT_SYNCH> *> (args);

  ACE_Message_Block *mb = 0;

  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    {
      ++message_count;

      // Allocate a new message

      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (1),
                      0);
      *mb->wr_ptr () = *c;

      // Set the priority.
      mb->msg_priority (message_count);
      mb->wr_ptr (1);

      // Enqueue in priority order.
      if (msg_queue->enqueue_prio (mb) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%t) %p\n"),
                           ACE_TEXT ("put_next")),
                          0);
    }

  // Now send a 0-sized shutdown message to the other thread
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block ((size_t) 0),
                  0);

  if (msg_queue->enqueue_tail (mb) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("put_next")));

  ++message_count;

  // Now read all the items out in priority order (i.e., ordered by
  // the size of the lines!).
  consumer (msg_queue);

  return 0;
}

#endif /* ACE_HAS_THREADS */

// Spawn off one thread that copies stdin to stdout in order of the
// size of each line.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Priority_Buffer_Test"));

#if defined (ACE_HAS_THREADS)
  // Message queue.
  ACE_Message_Queue<ACE_MT_SYNCH> msg_queue (max_queue);

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (producer),
       (void *) &msg_queue,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn")),
                      1);

  // Wait for producer and consumer threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
