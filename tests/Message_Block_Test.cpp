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
//    Doug Schmidt
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Malloc.h"
#include "ace/Profile_Timer.h"
#include "ace/Free_List.h"
#include "test_config.h"

#define  ACE_ALLOC_STRATEGY_NO   2
// Number of memory allocation strategies used in this test

#if defined (ACE_HAS_THREADS)

// Number of iterations to run the test.
static size_t n_iterations = ACE_MAX_ITERATIONS;

static ACE_Lock_Adapter<ACE_Thread_Mutex> lock_adapter_;
// Serialize access to <ACE_Message_Block> reference count, which will
// be decremented from multiple threads.

template <class TYPE>
class Mem_Pool_Node
// Mem_Pool_Node keeps unused memory within free list
// Free list structure is kept within the memory being kept.
// The length of a piece of unused memory must be greater than
// sizeof (void*).  This makes sense because we'll waste even 
// more memory if we keep them in a separate data structure.
{
public:
  TYPE *addr () { return &this->obj_; }
  // return the address of free memory

  Mem_Pool_Node<TYPE> *get_next () { return this->next_; }
  // get the next Mem_Pool_Node

  void  set_next (Mem_Pool_Node<TYPE> * ptr) { this->next_ = ptr; }
  // set the next Mem_Pool_Node 

private:
  union
  {
    TYPE obj_;
    Mem_Pool_Node<TYPE> *next_;
  } ;
};

template <class TYPE, class LOCK>
class Cached_Memory_Pool_Allocator : public ACE_New_Allocator
{
public:
  Cached_Memory_Pool_Allocator (size_t n_chunks);
  // Create a cached memory poll with <n_chunks> chunks 
  // each with sizeof (TYPE) size.  

  ~Cached_Memory_Pool_Allocator ();
  // clear things up

  void* malloc (size_t);
  // get a chunk of memory

  void free (void *);
  // return a chunk of memory

private:
  TYPE *pool_;

  ACE_Locked_Simple_Free_List<Mem_Pool_Node<TYPE>, LOCK> free_list_;
};

template <class TYPE, class LOCK>
Cached_Memory_Pool_Allocator<TYPE, LOCK>::Cached_Memory_Pool_Allocator (size_t n_chunks)
  : pool_ (0)
{
  ACE_NEW (this->pool_, TYPE [n_chunks]);
  // ERRNO could be lost because this is within ctor
  
  for (size_t c = 0 ; c < n_chunks ; c ++)
    this->free_list_.add (new (&this->pool_ [c]) Mem_Pool_Node<TYPE> ());
  // put into free list using placement contructor, no real memory
  // allocation in the above new
}

template <class TYPE, class LOCK>
Cached_Memory_Pool_Allocator<TYPE, LOCK>::~Cached_Memory_Pool_Allocator ()
{
  delete [] this->pool_;
}

template <class TYPE, class LOCK> void *
Cached_Memory_Pool_Allocator<TYPE, LOCK>::malloc (size_t nbytes)
{
  // check if size requested fits within pre-determined size
  if (sizeof (TYPE) < nbytes)
    return NULL;

  void * ptr;
  return this->free_list_.remove ()->addr ();
  // addr() call is really not absolutely necessary because of 
  // the way Mem_Pool_Node's internal structure is arranged.
}

template <class TYPE, class LOCK> void
Cached_Memory_Pool_Allocator<TYPE, LOCK>::free (void * ptr)
{
  this->free_list_.add ((Mem_Pool_Node<TYPE> *) ptr) ;
}

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

      // If there's a next() Task then duplicate the message and send
      // it on down the pipeline.
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

Worker_Task::Worker_Task ()
{
  // Make us an Active Object.
  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}  

static int
produce (Worker_Task &worker_task, ACE_Allocator *alloc_strategy)
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


#endif /* ACE_HAS_THREADS */

typedef char memory_chunk[ACE_MALLOC_ALIGN * 5];

int 
main (int, char *[])
{
  ACE_START_TEST ("Message_Block_Test");
#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  
  ACE_DEBUG ((LM_DEBUG, "(%t) threads = %d\n", n_threads));

  Cached_Memory_Pool_Allocator<memory_chunk, ACE_Thread_Mutex> mem_allocator (48);
  ACE_Profile_Timer ptime;
  ACE_Profile_Timer::ACE_Elapsed_Time et[ACE_ALLOC_STRATEGY_NO];
  ACE_Allocator *alloc_strategy [] = { NULL, &mem_allocator };
  char *alloc_name[] = { "Default", "Cached Memory" };

  for (int i = 0; i < ACE_ALLOC_STRATEGY_NO; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) Start Message_Block_Test using %s allocation strategy\n", alloc_name[i]));
      // Create the worker tasks.
      Worker_Task worker_task[ACE_MAX_THREADS] ;
    
      // Link all the tasks together into a simple pipeline.
      for (int j = 1; j < ACE_MAX_THREADS; j++)
	worker_task[j - 1].next (&worker_task[j]);

      ptime.start ();
      // Generate messages and pass them through the pipeline.
      produce (worker_task[0], alloc_strategy [i]);

      // Wait for all the threads to reach their exit point.
    
      ACE_DEBUG ((LM_DEBUG, "(%t) waiting for worker tasks to finish...\n"));
    
      ACE_Service_Config::thr_mgr ()->wait ();
      ptime.stop ();
      ptime.elapsed_time (et[i]);

      ACE_DEBUG ((LM_DEBUG, "(%t) destroying worker tasks\n"));
    }

  for (i = 0; i < ACE_ALLOC_STRATEGY_NO; i++)
    ACE_DEBUG ((LM_DEBUG, "Elapsed time using %s allocation strategy: %f sec\n", alloc_name [i], et[i].real_time));

  ACE_DEBUG ((LM_DEBUG, "(%t) Exiting...\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if defined(ACE_HAS_THREADS)
 template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
#endif /* ACE_HAS_THREADS */
template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
