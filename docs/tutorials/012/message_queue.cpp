
// $Id$

#include "block.h"
#include "work.h"
#include "task.h"

/*
  I want to be sure that our Task object gets destructed correctly, so
  I'll do most of the application 'work' in run_test() instead of
  main()
*/
int run_test (int iterations, int threads)
{
     /*
       Create the Task which is our thread pool for doing work
     */
  Task task;

  if (task.start (threads) == -1)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "start"), -1);
  }

   /*
     Give the Task a chance to enter it's svc() method.  This isn't
     really necessary and you probably wouldn't do it in a real
     application but it makes the output more interesting.
   */
  ACE_OS::sleep (ACE_Time_Value (1));

  for (int i = 0; i < iterations; ++i)
  {
    /*
       Construct a Work object that we'll put into the Queue.  Give it 
       the iteration number so that it can identify itself in the output.
     */
    Work * data = new Work(i);

    /*
       Create a block that contains our Work object but also has
       enough room for a text message.
     */
    Message_Block *message = new Message_Block (128, data);

    /*
       As before, put a text message into the block. 
     */
    ACE_OS::sprintf (message->wr_ptr (), "This is message %d.", i);
    message->wr_ptr (strlen (message->rd_ptr ())+1);

     /*
       Add the work to our thread pool
     */
    if (task.putq (message) == -1)
    {
      break;
    }
  }

   /*
     Insert a HANGUP message block to tell the thread pool to shut
     itself down.
    */
  Message_Block *message = new Message_Block (0,0);
  message->msg_type (ACE_Message_Block::MB_HANGUP);
  task.putq (message);

   /*
     Wait for the all threads of the Task to exit.  It is rather rude
     to let the Task go out of scope without doing this first.
   */
  task.wait ();

  return (0);
}

int main (int argc, char *argv[])
{
     /*
       Give the user a chance to override the default number of
       iterations and pool threads.
     */
  int iterations = argc > 1 ? atoi (argv[1]) : 4;
  int threads = argc > 2 ? atoi (argv[2]) : 2;

   /*
     Use the function above to do the actual test.  As I said, this
     lets us see the Task go out of scope and destruct before our
     "exiting" message below.
   */
  (void) run_test (iterations, threads);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Application exiting\n"));

  return (0);
}
