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
//      This test program is a torture test that illustrates how
//      ACE_Message_Block reference counting works in multi-threaded
//      code.
//
// = AUTHOR
//    Doug Schmidt and Nanbor Wang
//
// ============================================================================

#include "test_config.h"
#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Malloc.h"
#include "ace/Profile_Timer.h"
#include "ace/Free_List.h"

// Number of memory allocation strategies used in this test.
static const int ACE_ALLOC_STRATEGY_NO = 2;

// Size of a memory block (multiple of ACE_MALLOC_ALIGN)
static const int ACE_ALLOC_SIZE = 5;

// Amount of memory block preallocated
static const size_t ACE_ALLOC_AMOUNT = 48;

#if defined (ACE_HAS_THREADS)

// Number of iterations to run the test.
static size_t n_iterations = ACE_MAX_ITERATIONS;

static ACE_Lock_Adapter<ACE_SYNCH_MUTEX> lock_adapter_;
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
  return this->msg_queue ()->enqueue_prio (mb, tv);
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

  for (int count = 0; ; count++)
    {
      ACE_Message_Block *mb;

      ACE_ASSERT (this->msg_queue ()->dequeue_head (mb) != -1);

      int length = mb->length ();

      // If there's a next() Task then "logically" copy the message by
      // calling <duplicate> and send it on down the pipeline.  Note
      // that this doesn't actually make a copy of the message
      // contents (i.e., the Data_Block portion), it just makes a copy
      // of the header and reference counts the data.
      if (this->next () != 0)
	ACE_ASSERT (this->put_next (mb->duplicate ()) != -1);

      // If there's no next() Task to send to, then we'll consume the
      // message here.
      else if (length > 0)
	{
	  int current_count = ACE_OS::atoi (mb->rd_ptr ());
	  int i;

	  ACE_ASSERT (count == current_count);

	  ACE_DEBUG ((LM_DEBUG, "(%t) enqueueing %d duplicates\n",
		      current_count));

	  ACE_Message_Block *dup;

	  // Enqueue <current_count> duplicates with msg_priority == 1.
	  for (i = current_count; i > 0; i--)
	    {
	      ACE_ALLOCATOR_RETURN (dup, mb->duplicate (), -1);
	      // Set the priority to be greater than "normal"
	      // messages.  Therefore, all of these messages should go
	      // to the "front" of the queue, i.e., ahead of all the
	      // other messages that are being enqueued by other
	      // threads.
	      dup->msg_priority (1);

	      ACE_ASSERT (this->msg_queue ()->enqueue_prio
			  (dup,
			   // Don't block indefinitely if we flow control...
			   (ACE_Time_Value *) &ACE_Time_Value::zero) != -1);
	    }

	  ACE_DEBUG ((LM_DEBUG, "(%t) dequeueing %d duplicates\n",
		      current_count));

	  // Dequeue the same <current_count> duplicates.
	  for (i = current_count; i > 0; i--)
	    {
	      ACE_ASSERT (this->msg_queue ()->dequeue_head (dup) != -1);
	      ACE_ASSERT (count == ACE_OS::atoi (dup->rd_ptr ()));
	      ACE_ASSERT (ACE_OS::strcmp (mb->rd_ptr (), dup->rd_ptr ()) == 0);
	      ACE_ASSERT (dup->msg_priority () == 1);
	      dup->release ();
	    }

	  ACE_DEBUG ((LM_DEBUG,
		      "(%t) in iteration %d, length = %d, prio = %d, text = \"%*s\"\n",
		      count,
		      length,
		      mb->msg_priority (),
		      length - 2, // remove the trailing "\n\0"
		      mb->rd_ptr ()));
	}

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
  // Make us an Active Object.
  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}

static int
produce (Worker_Task &worker_task,
	 ACE_Allocator *alloc_strategy)
{
  ACE_Message_Block *mb;

  // Send <n_iteration> messages through the pipeline.
  for (size_t count = 0; count < n_iterations; count++)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "%d\n", count);

      int n = ACE_OS::strlen (buf) + 1;

      // Allocate a new message.
      ACE_NEW_RETURN (mb,
		      ACE_Message_Block (n, // size
					 ACE_Message_Block::MB_DATA, // type
					 0, // cont
					 0, // data
					 alloc_strategy, // allocator
					 &lock_adapter_, // locking strategy
					 0), // priority
		      -1);

      // Copy buf into the Message_Block and update the wr_ptr ().
      mb->copy (buf, n);

      // Pass the message to the Worker_Task.
      if (worker_task.put (mb,
			   // Don't block indefinitely if we flow control...
			   (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
	ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));
    }

  // Send a shutdown message to the waiting threads and exit.
  ACE_DEBUG ((LM_DEBUG,
	      "\n(%t) sending shutdown message\n"));

  ACE_NEW_RETURN (mb,
		  ACE_Message_Block (0, ACE_Message_Block::MB_DATA,
				     0, 0, alloc_strategy, &lock_adapter_),
		  -1);

  if (worker_task.put (mb) == -1)
    ACE_ERROR ((LM_ERROR, " (%t) %p\n", "put"));

  ACE_DEBUG ((LM_DEBUG, "\n(%t) end producer\n"));

  return 0;
}

typedef char MEMORY_CHUNK[ACE_MALLOC_ALIGN * ACE_ALLOC_SIZE];

ACE_Cached_Allocator<MEMORY_CHUNK,
                     ACE_SYNCH_MUTEX>
                     mem_allocator (ACE_ALLOC_AMOUNT);

struct
{
  ACE_Allocator *strategy_;
  char *name_;
  ACE_Profile_Timer::ACE_Elapsed_Time et_;
} alloc_struct[ACE_ALLOC_STRATEGY_NO] =
{
  { NULL, "Default" },
  { &mem_allocator, "Cached Memory" }
};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cached_Allocator<MEMORY_CHUNK, ACE_SYNCH_MUTEX>;
template class ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK>, ACE_SYNCH_MUTEX>;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK> >;
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cached_Allocator<MEMORY_CHUNK, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<MEMORY_CHUNK> >
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>
#pragma instantiate ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("Message_Block_Test");
#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;

  ACE_DEBUG ((LM_DEBUG, "(%t) threads = %d\n", n_threads));

  ACE_Profile_Timer ptime;

  int i;

  for (i = 0; i < ACE_ALLOC_STRATEGY_NO; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "(%t) Start Message_Block_Test using %s allocation strategy\n",
		  alloc_struct[i].name_));

      // Create the worker tasks.
      Worker_Task worker_task[ACE_MAX_THREADS] ;

      // Link all the tasks together into a simple pipeline.
      for (int j = 1; j < ACE_MAX_THREADS; j++)
	worker_task[j - 1].next (&worker_task[j]);

      ptime.start ();
      // Generate messages and pass them through the pipeline.
      produce (worker_task[0], alloc_struct[i].strategy_);

      // Wait for all the threads to reach their exit point.

      ACE_DEBUG ((LM_DEBUG,
		  "(%t) waiting for worker tasks to finish...\n"));

      ACE_Thread_Manager::instance ()->wait ();
      ptime.stop ();
      ptime.elapsed_time (alloc_struct[i].et_);

      ACE_DEBUG ((LM_DEBUG,
		  "(%t) destroying worker tasks\n"));
    }

  for (i = 0; i < ACE_ALLOC_STRATEGY_NO; i++)
    ACE_DEBUG ((LM_DEBUG,
		"Elapsed time using %s allocation strategy: %f sec\n",
		alloc_struct[i].name_, alloc_struct[i].et_.real_time));

  ACE_DEBUG ((LM_DEBUG, "(%t) Exiting...\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

