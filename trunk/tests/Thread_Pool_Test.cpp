// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Thread_Pool_Test.cpp
//
// = DESCRIPTION
//      This test program illustrates how the ACE task synchronization
//      mechanisms work in conjunction with the ACE_Task and the
//      ACE_Thread_Manager.  If the manual flag is not set input comes
//      from stdin until the user enters a return only.  This stops
//      all workers via a message block of length 0. This is an
//      alternative shutdown of workers compared to queue deactivate.
//
// = AUTHOR
//    Karlheinz Dorn, Doug Schmidt, and Prashant Jain
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"
#include "ace/Task.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Number of iterations to run the test.
static size_t n_iterations = 100;

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Thread_Pool (ACE_Thread_Manager *thr_mgr, int n_threads);
  
  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv=0);
  // This allows the producer to pass messages to the <Thread_Pool>.

private:
  virtual int close (u_long);

  // = Not needed for this test.
  virtual int open (void *) { return 0; }
};

int 
Thread_Pool::close (u_long) 
{ 
  ACE_DEBUG ((LM_DEBUG, "(%t) close of worker\n"));
  return 0;
}

Thread_Pool::Thread_Pool (ACE_Thread_Manager *thr_mgr, 
			  int n_threads) 
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr) 
{
  // Create worker threads.
  if (this->activate (THR_NEW_LWP, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}
  
// Simply enqueue the Message_Block into the end of the queue.

int
Thread_Pool::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{ 
  return this->putq (mb, tv); 
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int 
Thread_Pool::svc (void) 
{  
  ACE_NEW_THREAD;
  // Note that the ACE_Task::svc_run () method automatically adds us to
  // the Thread_Manager when the thread begins.

  int result = 0;
  int count = 1;

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (;; count++)
    {       
      ACE_Message_Block *mb;

      ACE_ASSERT (this->getq (mb) != -1);

      int length = mb->length ();

      if (length > 0)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) in iteration %d, length = %d, text = \"%*s\"\n",
		    count, length, length - 1, mb->rd_ptr ()));

      // We're responsible for deallocating this.
      delete mb;

      if (length == 0)
	{
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%t) in iteration %d, got NULL message, exiting\n",
		      count));
	  break;
	}
    }

  // Note that the ACE_Task::svc_run () method automatically removes
  // us from the Thread_Manager when the thread exits.
  return 0;
}

static void 
produce (Thread_Pool &thread_pool)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) producer start, dumping the Thread_Pool\n"));
  thread_pool.dump ();

  for (int n;;)
    {
      // Allocate a new message.
      ACE_Message_Block *mb;
      ACE_NEW (mb, ACE_Message_Block (BUFSIZ));

#if defined (manual)
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) press chars and enter to put a new message into task queue..."));
      n = ACE_OS::read (ACE_STDIN, mb->rd_ptr (), mb->size ());
#else // Automatically generate messages.
      static int count = 0;

      ACE_OS::sprintf (mb->rd_ptr (), "%d\n", count);

      n = ACE_OS::strlen (mb->rd_ptr ());

      if (count == n_iterations)
	n = 1; // Indicate that we need to shut down.
      else
	count++;

      if (count == 0 || (count % 20 == 0))
	ACE_OS::sleep (1);
#endif /* manual */
      if (n > 1)
        {
	  // Send a normal message to the waiting threads and continue
	  // producing.
	  mb->wr_ptr (n);

	  // Pass the message to the Thread_Pool.
	  if (thread_pool.put (mb) == -1)
            ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
	}
      else
	{
          // Send a shutdown message to the waiting threads and exit.
	  ACE_DEBUG ((LM_DEBUG, "\n(%t) start loop, dump of task:\n"));
	  thread_pool.dump ();

	  for (int i = thread_pool.thr_count (); i > 0; i--)
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "(%t) EOF, enqueueing NULL block for thread = %d\n", 
			  i));

	      // Enqueue a NULL message to flag each consumer to
	      // shutdown.
	      if (thread_pool.put (new ACE_Message_Block) == -1)
		ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
	    }

	  ACE_DEBUG ((LM_DEBUG, "\n(%t) end loop, dump of task:\n"));
	  thread_pool.dump ();
          break;
	}
    }
}

#endif /* ACE_HAS_THREADS */

int 
main (int argc, char *argv[])
{
  ACE_START_TEST ("Thread_Pool_Test.cpp");
#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  
  ACE_DEBUG ((LM_DEBUG, "(%t) argc = %d, threads = %d\n", 
	      argc, n_threads));

  // Create the worker tasks.
  Thread_Pool thread_pool (ACE_Service_Config::thr_mgr (), 
			   n_threads);

  // Create work for the worker tasks to process in their own threads.
  produce (thread_pool);

  // Wait for all the threads to reach their exit point.

  ACE_DEBUG ((LM_DEBUG, "(%t) waiting with thread manager...\n"));
  ACE_Service_Config::thr_mgr ()->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) destroying worker tasks and exiting...\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
