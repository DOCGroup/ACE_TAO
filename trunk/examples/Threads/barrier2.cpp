// $Id$

// generic_worker_task.cpp
//
// This test program illustrates how the ACE task workers/barrier
// synchronization mechanisms work in conjunction with the ACE_Task
// and the ACE_Thread_Manager.  The manual flag not set simulates
// user input, if set input comes from stdin until RETURN only is
// entered which stops all workers via a message block of length
// 0. This is an alernative shutdown of workers compared to queue
// deactivate.  The delay_put flag simulates a delay between the
// shutdown puts. All should work with this flag disabled!  The
// BARRIER_TYPE is supposed to enable/disable barrier sync on each svc
// a worker has done.

#include <iostream.h>
#include "ace/Task.h"
#include "ace/Service_Config.h"

#if defined (ACE_HAS_THREADS)

#define BARRIER_TYPE ACE_Null_Barrier
//#define BARRIER_TYPE ACE_Barrier
//#ifdef delay_put
//#define manual

template <class BARRIER>
class Worker_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:

  Worker_Task (ACE_Thread_Manager *thr_mgr, 
	       int n_threads, 
	       int inp_serialize = 1);
  
  virtual int Producer (void);
  // produce input for workers

  virtual int input (ACE_Message_Block *mb);
  // Fill one message block via a certain input strategy.

  virtual int output (ACE_Message_Block *mb);
  // Forward one message block via a certain output strategy to the
  // next task if any.

  virtual int service (ACE_Message_Block *mb, int iter);
  // Perform one message block dependant service.

private:
  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv=0);

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  // = Not needed for this test.
  virtual int open (void *) { return 0; }
  virtual int close (u_long) {ACE_DEBUG ((LM_DEBUG,"(%t) in close of worker\n")); return 0; }
  
  int nt_;
  // Number of worker threads to run.
  int inp_serialize_;

  BARRIER barrier_;
};

template <class BARRIER>
Worker_Task<BARRIER>::Worker_Task (ACE_Thread_Manager *thr_mgr, 
				   int n_threads, 
				   int inp_serialize) 
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr), 
    barrier_ (n_threads)
{
  nt_ = n_threads;
  // Create worker threads.
  inp_serialize_ = inp_serialize;

  // Use the task's message queue for serialization (default) or run
  // service in the context of the caller thread.

  if (nt_ > 0 && inp_serialize == 1)
    if (this->activate (THR_NEW_LWP, n_threads) == -1)
      ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}

// Simply enqueue the Message_Block into the end of the queue.

template <class BARRIER> int
Worker_Task<BARRIER>::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{ 
  int result;
  if (this->inp_serialize_)
    result = this->putq (mb, tv); 
  else
    {
      static int iter = 0;
      result = this->service (mb, iter++);

      if (this->output (mb) < 0)
	ACE_DEBUG ((LM_DEBUG, "(%t) output not connected!\n"));

      mb->release ();
    }
  return result;
}

template <class BARRIER> int 
Worker_Task<BARRIER>::service (ACE_Message_Block *mb, int iter) 
{
  int length = mb->length ();

  if (length > 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%t) in iteration %d len=%d text got:\n",iter,length));
      ACE_OS::write (ACE_STDOUT, mb->rd_ptr (), length);
      ACE_DEBUG ((LM_DEBUG,"\n"));
    }
  return 0;
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

template <class BARRIER> int 
Worker_Task<BARRIER>::svc (void) 
{  
  // Note that the ACE_Task::svc_run () method automatically adds us
  // to the Thread_Manager when the thread begins.

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (int iter = 1; ;iter++)
    {       
      ACE_Message_Block *mb = 0;

      int result = this->getq (mb);

      if (result == -1)
	{
	  ACE_ERROR ((LM_ERROR, 
		      "(%t) in iteration %d\n", "error waiting for message in iteration", iter));
	  break;
	}

      int length = mb->length ();
      this->service (mb,iter);

      if (length == 0)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) in iteration %d got quit, exit!\n", iter));
	  mb->release ();
	  break;
	}

      this->barrier_.wait ();
      this->output (mb);

      mb->release ();
    }

  // Note that the ACE_Task::svc_run () method automatically removes
  // us from the Thread_Manager when the thread exits.

  return 0;
}

template <class BARRIER> int
Worker_Task<BARRIER>::Producer (void)
{
  // Keep reading stdin, until we reach EOF.

  for (;;)
    {
      // Allocate a new message.
      ACE_Message_Block *mb = new ACE_Message_Block (BUFSIZ);

      if (this->input (mb) == -1)
	return -1;
    }

  return 0; 
}

template <class BARRIER>int
Worker_Task<BARRIER>::output (ACE_Message_Block *mb)
{
  return this->put_next (mb);
}

template <class BARRIER>int
Worker_Task<BARRIER>::input (ACE_Message_Block *mb)
{
  ACE_Message_Block *mb1;

#ifndef manual
  static int l= 0;
  char str[]="kalle";
  strcpy (mb->rd_ptr (),str);
  int n=strlen (str);
  if (l==1000)
    n=1;
  l++;
  if (l==0 || (l%100 == 0)) ACE_OS::sleep (5);
  if (n <= 1)
#else
    ACE_DEBUG ((LM_DEBUG,"(%t) press chars and enter to put a new message into task queue ...\n"));
  if ((n = read (0, mb->rd_ptr (), mb->size ())) <= 1) 
#endif // manual
    {
      // Send a shutdown message to the waiting threads and exit.
      // cout << "\nvor loop, dump of task msg queue:\n" << endl;
      // this->msg_queue ()->dump ();
      for (int i=0;i<nt_;i++)
	{
	  ACE_DEBUG ((LM_DEBUG,"(%t) eof, sending block for thread=%d\n",i+1));
	  mb1 = new ACE_Message_Block (2);
	  mb1->length (0);
	  if (this->put (mb1) == -1)
	    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put"));
#ifdef delay_put
	  ACE_OS::sleep (1); // this sleep helps to shutdown correctly -> was an error!
#endif /* delay_put */
	}
      //          cout << "\nnach loop, dump of task msg queue:\n" << endl;
      //          this->msg_queue ()->dump ();
      return (-1);
    }
  else
    {
      // Send a normal message to the waiting threads and continue producing.
      mb->wr_ptr (n);
      if (this->put (mb) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put"));
    }
  return 0; 
}

int 
main (int argc, char *argv[])
{
  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_THREADS;
  
  ACE_DEBUG ((LM_DEBUG,"(%t) worker threads running=%d\n",n_threads));

  
  Worker_Task<BARRIER_TYPE> *worker_task = 
	  new Worker_Task<BARRIER_TYPE> (ACE_Thread_Manager::instance (), 
				   /*n_threads*/ 0,0); 
  
  worker_task->Producer ();

  // Wait for all the threads to reach their exit point.
  ACE_DEBUG ((LM_DEBUG,"(%t) waiting with thread manager ...\n"));
  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG,"(%t) delete worker task ...\n"));

  delete worker_task;
  ACE_DEBUG ((LM_DEBUG,"(%t) done correct!\n"));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class Worker_Task<ACE_Null_Barrier>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
