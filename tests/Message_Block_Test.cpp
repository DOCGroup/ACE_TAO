// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Message_Block_Test.cpp
//
// = DESCRIPTION
//      This test program illustrates how ACE_Message_Block reference
//      counting works.
//
// = AUTHOR
//    Doug Schmidt
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Number of iterations to run the test.
static size_t n_iterations = ACE_MAX_ITERATIONS;

static ACE_Lock_Adapter<ACE_Thread_Mutex> lock_adapter_;
// Serialize access to <ACE_Message_Block> reference count, which will
// be decremented from multiple threads.

class Worker_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Worker_Task (void);
  // Activate the task.

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
  // Allows the producer to pass messages to the <Message_Block>.

private:
  virtual int close (u_long);
  // Close hook.
};

int 
Worker_Task::close (u_long) 
{ 
  ACE_DEBUG ((LM_DEBUG, "(%t) close of worker\n"));
  return 0;
}

// Simply enqueue the Worker_Task into the end of the queue.

int
Worker_Task::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{ 
  return this->putq (mb, tv); 
}

// Iterate <n_iterations> printing off a message and "waiting" for all
// other threads to complete this iteration.

int 
Worker_Task::svc (void) 
{  
  ACE_NEW_THREAD;
  // The <ACE_Task::svc_run()> method automatically adds us to the
  // <ACE_Service_Config>'s <ACE_Thread_Manager> when the thread
  // begins.

  ACE_DEBUG ((LM_DEBUG, "(%t) starting svc() method\n"));
  
  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (int count = 1; ; count++)
    {       
      ACE_Message_Block *mb;

      ACE_ASSERT (this->getq (mb) != -1);

      // Duplicate the message and send it on down the pipeline.
      if (this->put_next (mb->duplicate ()) != 0)
	ACE_DEBUG ((LM_DEBUG, "(%t) put_next failed\n"));

      int length = mb->length ();

      if (length > 0)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) in iteration %d, length = %d, text = \"%*s\"\n",
		    count, length, length - 1, mb->rd_ptr ()));

      // We're responsible for deallocating this.
      mb->release ();

      if (length == 0)
	{
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%t) in iteration %d, queue len = %d, got NULL message, exiting\n",
		      count, this->msg_queue ()->message_count ()));
	  break;
	}
    }

  // Note that the ACE_Task::svc_run () method automatically removes
  // us from the Thread_Manager when the thread exits.
  return 0;
}

Worker_Task::Worker_Task (void)
{
  // Create worker threads.
  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}  

static int
produce (Worker_Task &worker_task)
{
  ACE_Message_Block *mb;

  for (size_t count = 0;;)
    {
      // Allocate a new message.
      ACE_NEW_RETURN (mb, 
		      ACE_Message_Block (BUFSIZ, ACE_Message_Block::MB_DATA, 
					 0, 0, 0, &lock_adapter_),
		      -1);

      ACE_OS::sprintf (mb->rd_ptr (), "%d\n", count);
      int n = ACE_OS::strlen (mb->rd_ptr ());

      if (count == n_iterations)
	break;
      else
	count++;

      if (count == 0 || (count % 20 == 0))
	ACE_OS::sleep (1);

      mb->wr_ptr (n);

      // Pass the message to the Worker_Task.
      if (worker_task.put (mb) == -1)
	ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
    }

  // Send a shutdown message to the waiting threads and exit.
  ACE_DEBUG ((LM_DEBUG, 
	      "\n(%t) sending shutdown message\n"));

  ACE_NEW_RETURN (mb, 
		  ACE_Message_Block (0, ACE_Message_Block::MB_DATA, 
				     0, 0, 0, &lock_adapter_),
		  -1);

  if (worker_task.put (mb->duplicate ()) == -1)
    ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));

  mb->release ();

  ACE_DEBUG ((LM_DEBUG, "\n(%t) end producer\n"));
}


#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Message_Block_Test");
#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  
  ACE_DEBUG ((LM_DEBUG, "(%t) threads = %d\n", n_threads));

  // Create the worker tasks.
  Worker_Task worker_task[ACE_MAX_THREADS];

  // Link all the tasks together into a simple pipeline.
  for (int i = 1; i < ACE_MAX_THREADS; i++)
    worker_task[i - 1].next (&worker_task[i]);

  produce (worker_task[0]);

  // Wait for all the threads to reach their exit point.

  ACE_DEBUG ((LM_DEBUG, "(%t) waiting for worker tasks to finish...\n"));

  ACE_Service_Config::thr_mgr ()->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) destroying worker tasks and exiting...\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
