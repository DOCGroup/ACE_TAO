// $Id$

/* Most of this is the same as the previous tutorial, so I'll just
   point out the differences.  */
#include "task.h"
#include "block.h"
#include "data.h"


static int
run_test (int iterations,
          int threads)

{
  Task task (threads);

  if (task.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_OS::sleep (ACE_Time_Value (1));

  int i;
  for (i = 0; i < iterations; ++i)
    {
      /* Construct a Data object that we'll put into the Queue.  */
      Data data (i);

      /* Create a block large enough for our Data object as well as a
        text message.  */
      Block *message;

      ACE_NEW_RETURN (message,
                      Block (sizeof (data) + 128),
                      -1);

      /* As before, put a text message into the block.  */
      ACE_OS::sprintf (message->wr_ptr (), "This is message %d.", i);
      message->wr_ptr (strlen (message->rd_ptr ()));

      message->wr_ptr (1);        // Move beyond the NULL

      /* To copy arbitrary data into a message block, we use the
        copy() method.  Since it wants a 'const char*', we have to
        cast our Data pointer.

        Note that copy() will advance the wr_ptr() for us.  This means
        we don't have to do it ourselves!  If you do advance it, it
        will be way beyond what you want.  */
      message->copy ((const char *) &data,
                     sizeof (data));

      if (task.putq (message) == -1)
        break;
    }

  Block *message;
  ACE_NEW_RETURN (message,
                  Block,
                  -1);
  message->msg_type (ACE_Message_Block::MB_HANGUP);
  task.putq (message);

  task.wait ();

  return 0;
}

int
main (int argc, char *argv[])
{
  int iterations = argc > 1 ? ACE_OS::atoi (argv[1]) : 4;
  int threads = argc > 2 ? ACE_OS::atoi (argv[2]) : 2;

  run_test (iterations,
            threads);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Application exiting\n"));

  return 0;
}
