// $Id$

/* To illustrate the ACE_Message_Queue, we use a derivative of
   ACE_Task<>.  We also derive from ACE_Message_Block to show that we
   don't have memory leaks.  */
#include "task.h"
#include "block.h"

int 
run_test (int iterations,
          int threads)
{
  /* Create and start an instance of our Task object.  */
  Task task (threads);

  if (task.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  /* Give the threads a moment to open.  This isn't really necessary
    but if we don't we find that all of our blocks are constructed and
    enqueued before any of the threads get created. Basically, the
    sleep() makes the output look more interesting.  */
  ACE_OS::sleep (ACE_Time_Value (1));

  int i;
  for (i = 0; i < iterations; ++i)
    {
      /* Create a new message block to hold our data.  Here, we ask
        for a block that has 128 bytes of data space.  */
      Block *message;
      ACE_NEW_RETURN (message,
                      Block (128),
                      -1);

      /* Grab the "write pointer".  This is a pointer into the data
        area where we can write our data.  After writting the data you
        have to increment the wr_ptr() so that subsequent writes won't
        clobber what you've put there.  */
      ACE_OS::sprintf (message->wr_ptr (),
                       "This is message %d.",
                       i);
      message->wr_ptr (ACE_OS::strlen (message->rd_ptr ()));

      /* Put the message block into the queue.  One of the threads in
        the Task object will pick up the block and "do work" on it.  */
      if (task.putq (message) == -1)
        break;
    }

  /* Once we're done, we have to signal the Task objects to shut
    down. There are several choices including: - Send a message of
    zero length - Send a message with a special content I don't like
    these choices because they're likely to interfere with application
    logic.  Instead, I use the message type feature to send a message
    of type "hangup".  The default type is MB_DATA, so when the tasks
    get a MB_HANGUP type, they know to go away.  */
  Block *message;

  ACE_NEW_RETURN (message,
                  Block (),
                  -1);
  message->msg_type (ACE_Message_Block::MB_HANGUP);
  task.putq (message);

  /* Wait for the threads in our task object to go away.  */
  task.wait ();
  
  return 0;
}

int 
main (int argc, char *argv[])
{
  /* Set the number of iterations through our putq() loop and the
    number of threads to use in our Task<> derivative.  */
  int iterations = argc > 1 ? atoi (argv[1]) : 9;
  int threads = argc > 2 ? atoi (argv[2]) : 2;
  
  run_test (iterations, threads);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Application exiting\n"));
  
  return 0;
}
