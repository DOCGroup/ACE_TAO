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
//      mechanisms and ACE_Message_Block reference counting works in
//      conjunction with the ACE_Task and the ACE_Thread_Manager.  If
//      the manual flag is not set input comes from stdin until the
//      user enters a return.  This stops all workers via a message
//      block of length 0.  This shows an alternative way to shutdown
//      worker tasks compared to queue deactivate.
//
// = AUTHOR
//    Karlheinz Dorn, Doug Schmidt, and Prashant Jain
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Number of iterations to run the test.
static size_t n_iterations = 100;

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Thread_Pool (int n_threads);
  // Create the thread pool containing <n_threads>.
  
  ~Thread_Pool (void);

  virtual int open (void * = 0);
  // Produce the messages that are consumed by the threads in the
  // thread pool.

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
  // Allows the producer to pass messages to the <Thread_Pool>.

private:
  virtual int close (u_long);
  // Close hook.

  ACE_Lock_Adapter<ACE_Thread_Mutex> lock_adapter_;
  // Serialize access to <ACE_Message_Block> reference count, which
  // will be decremented from multiple threads.
};

Thread_Pool::~Thread_Pool (void)
{
}

int 
Thread_Pool::close (u_long) 
{ 
  ACE_DEBUG ((LM_DEBUG, "(%t) close of worker\n"));
  return 0;
}

Thread_Pool::Thread_Pool (int n_threads) 
{
  // Create a pool of worker threads.
  if (this->activate (THR_NEW_LWP, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}
  
// Simply enqueue the Message_Block into the end of the queue.

int
Thread_Pool::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{ 
  return this->putq (mb, tv); 
}

// Iterate <n_iterations> printing off a message and "waiting" for all
// other threads to complete this iteration.

int 
Thread_Pool::svc (void) 
{  
  ACE_NEW_THREAD;
  // The <ACE_Task::svc_run()> method automatically adds us to the
  // <ACE_Service_Config>'s <ACE_Thread_Manager> when the thread
  // begins.

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (int count = 1; ; count++)
    {       
      ACE_Message_Block *mb;

      ACE_ASSERT (this->getq (mb) != -1);

      int length = mb->length ();

      if (length > 0)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) in iteration %d, queue len = %d, length = %d, text = \"%*s\"\n",
		    count, this->msg_queue ()->message_count (),
		    length, length - 1, mb->rd_ptr ()));

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

int
Thread_Pool::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) producer start, dumping the Thread_Pool\n"));
  this->dump ();

  for (size_t count = 0; count < n_iterations; count++)
    {
      ACE_Message_Block *mb = 0;

      // Allocate a new message.
      ACE_NEW_RETURN (mb, 
		      ACE_Message_Block (BUFSIZ, ACE_Message_Block::MB_DATA, 
					 0, 0, 0, &this->lock_adapter_),
		      -1);

      ACE_OS::sprintf (mb->rd_ptr (), "%d\n", count);
      int n = ACE_OS::strlen (mb->rd_ptr ());

      if (count == 0 || (count % 20 == 0))
	ACE_OS::sleep (1);

      // Send a normal message to the waiting threads and continue
      // producing.
      mb->wr_ptr (n);

      // Pass the message to the Thread_Pool.
      if (this->put (mb) == -1)
	ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
    }

  // Send a shutdown message to the waiting threads and exit.
  ACE_DEBUG ((LM_DEBUG, 
	      "\n(%t) sending shutdown message to %d threads, dump of task:\n",
	      this->thr_count ()));
  this->dump ();

  ACE_Message_Block *mb = 0;

  ACE_NEW_RETURN (mb, 
		  ACE_Message_Block (0, ACE_Message_Block::MB_DATA, 
				     0, 0, 0, &this->lock_adapter_),
		  -1);

  for (int i = this->thr_count (); i > 0; i--)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) EOF, enqueueing NULL block for thread = %d\n", 
		  i));

      // Enqueue an empty message to flag each consumer to shutdown.
      // Note that we use reference counting to avoid having to copy
      // the message.
      ACE_Message_Block *dup = mb->duplicate ();

      if (this->put (dup) == -1)
	ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
    }

  mb->release ();

  ACE_DEBUG ((LM_DEBUG, "\n(%t) end loop, dump of task:\n"));
  this->dump ();

  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)   
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Thread_Pool_Test");
#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  
  ACE_DEBUG ((LM_DEBUG, "(%t) threads = %d\n", n_threads));

  // Create the worker tasks.
  Thread_Pool thread_pool (n_threads);

  // Create work for the worker tasks to process in their own threads.
  thread_pool.open ();

  // Wait for all the threads to reach their exit point.

  ACE_DEBUG ((LM_DEBUG, "(%t) waiting for worker tasks to finish...\n"));

  ACE_Service_Config::thr_mgr ()->wait ();

  ACE_ASSERT (thread_pool.msg_queue ()->is_empty ());
  ACE_DEBUG ((LM_DEBUG, "(%t) destroying worker tasks and exiting...\n"));
  
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
