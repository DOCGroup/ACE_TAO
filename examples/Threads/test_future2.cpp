// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Test_Future.cpp
//
// = DESCRIPTION
//    This example tests the ACE Future.
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu> 
//
// Modification History
// Aug. 96; A.Kruse; dev.
// Aug. 96; D.Schmidt; complete workover
// 08/27/96; A.Kruse; - the friends of Scheduler are "Method_Object_name"
//                      and "Method_Object_work".
//                    - make the methods "work_i" and "name_i" private
// 09/2/96; D.Schmidt; Integrate with new ACE_Future API and rearrange
//                     the tests so they are more modular.
// ============================================================================

#include <math.h>
#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Method_Object.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, u_long> ATOMIC_INT;

// a counter for the tasks..
static ATOMIC_INT scheduler_open_count (0);

// forward declarations
class Method_Object_work;
class Method_Object_name;

class Scheduler : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //     Active Object Scheduler.
{
  // Every method object has to be able to access the private methods.

  friend class Method_Object_work; 
  friend class Method_Object_name;
  friend class Method_Object_end;
public:

  Scheduler (const char *, Scheduler * = 0);
  ~Scheduler (void);

  virtual int open (void *args = 0);
  // The method that is used to start the active object.

  // = Here are the methods exported by the class. They return an
  // <ACE_Future>.
  ACE_Future<float> work (float param, int count);
  ACE_Future<char*> name (void);
  void end (void);

private:
  virtual int close (u_long flags = 0);
  // Should not be accessible from outside...  (use end () instead).

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0)
    { return 0; };
  // Doesn't have any use for this example.

  virtual int svc (void);
  // Here the actual servicing of all requests is happening..

  // = Implementation methods.
  float work_i (float, int);
  char *name_i (void);

  const char *name_;
  ACE_Activation_Queue activation_queue_;
  Scheduler *scheduler_;
};

class Method_Object_work : public ACE_Method_Object
  // = TITLE
  //     Reification of the <work> method.
{
public:
  Method_Object_work (Scheduler *, float, int, ACE_Future<float> &);
  ~Method_Object_work (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
  float param_;
  int count_;
  ACE_Future<float> future_result_;
};

Method_Object_work::Method_Object_work (Scheduler* new_Scheduler,
				        float new_param, 
				        int new_count, 
					ACE_Future<float> &new_result)
  : scheduler_ (new_Scheduler),
    param_ (new_param),
    count_ (new_count),
    future_result_ (new_result)
{ 
}

Method_Object_work::~Method_Object_work (void)
{
}

int
Method_Object_work::call (void)
{
  return this->future_result_.set (this->scheduler_->work_i (this->param_, this->count_));
}

class Method_Object_name : public ACE_Method_Object
  // = TITLE
  //     Reification of the <name> method.
{
public:
  Method_Object_name (Scheduler *, ACE_Future<char*> &);
  ~Method_Object_name (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
  ACE_Future<char*> future_result_;
};


Method_Object_name::Method_Object_name (Scheduler *new_scheduler,
					ACE_Future<char*> &new_result)
  : scheduler_ (new_scheduler),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
	      " (%t) Method_Object_name created\n"));
};

Method_Object_name::~Method_Object_name (void)
{
  ACE_DEBUG ((LM_DEBUG, 
	      " (%t) Method_Object_name will be deleted.\n"));
}

int
Method_Object_name::call (void)
{
  return future_result_.set (scheduler_->name_i ());
}

class Method_Object_end : public ACE_Method_Object
  // = TITLE
  //     Reification of the <end> method.
{
public:
  Method_Object_end (Scheduler *new_Scheduler): scheduler_ (new_Scheduler) {}
  ~Method_Object_end (void) {}
  virtual int call (void) { this->scheduler_->close (); return -1; }

private:
  Scheduler *scheduler_;
};

// constructor
Scheduler::Scheduler (const char *newname, Scheduler *new_Scheduler)
{
  ACE_NEW (this->name_, char[ACE_OS::strlen (newname) + 1]);
  ACE_OS::strcpy ((char *) this->name_, newname);
  this->scheduler_ = new_Scheduler;
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s created\n", this->name_));
}

// Destructor
Scheduler::~Scheduler (void)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s will be destroyed\n", this->name_));
}

int 
Scheduler::open (void *)
{
  scheduler_open_count++;
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s open\n", this->name_));
  return this->activate (THR_BOUND);
}

int 
Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s close\n", this->name_));
  scheduler_open_count--;
  return 0;
}

int 
Scheduler::svc (void)
{
  // Main event loop for this active object.
  for (;;) 
    {
      // Dequeue the next method object (we use an auto pointer in
      // case an exception is thrown in the <call>).
      auto_ptr<ACE_Method_Object> mo (this->activation_queue_.dequeue ());

      ACE_DEBUG ((LM_DEBUG, " (%t) calling method object\n"));
      // Call it.
      if (mo->call () == -1)
	break;
      // Smart pointer destructor automatically deletes mo.
    }

  /* NOTREACHED */
  return 0;
}

void 
Scheduler::end (void)
{
  this->activation_queue_.enqueue (new Method_Object_end (this));
}

// Here's where the Work takes place.
float 
Scheduler::work_i (float param, 
		   int count)
{
  float x = 0, y = 0;
  
  for (int j = 0; j < count; j++) 
    {
      x = x + param;
      y = y + ::sin (x);
    }

  return y;
}

char *
Scheduler::name_i (void)
{
  char *the_name;

  the_name = new char[ACE_OS::strlen (this->name_) + 1];
  ACE_OS::strcpy (the_name, this->name_);

  return the_name;
}

ACE_Future<char *> 
Scheduler::name (void)
{
  if (this->scheduler_) 
    // Delegate to the other scheduler
    return this->scheduler_->name ();
  else 
    {
      ACE_Future<char*> new_future;

      if (this->thr_count () == 0) 
	{
	  // This scheduler is inactive... so we execute the user
	  // request right away...
	
	  auto_ptr<ACE_Method_Object> mo (new Method_Object_name (this, new_future));

	  mo->call ();
	  // Smart pointer destructor automatically deletes mo.
	}
      else
	// @@ What happens if new fails here?
	this->activation_queue_.enqueue 
	  (new Method_Object_name (this, new_future));
	
      return new_future;
    }
}

ACE_Future<float> 
Scheduler::work (float newparam, int newcount)
{
  if (this->scheduler_) 
    return this->scheduler_->work (newparam, newcount);
  else 
    {
      ACE_Future<float> new_future;

      if (this->thr_count () == 0) 
	{
	  auto_ptr<ACE_Method_Object> mo 
	    (new Method_Object_work (this, newparam, newcount, new_future));
	  mo->call ();
	  // Smart pointer destructor automatically deletes it.
	}
      else
	this->activation_queue_.enqueue
	  (new Method_Object_work (this, newparam, newcount, new_future));

      return new_future;
    }
}

static int 
determine_iterations (void)
{
  int n_iterations;

  ACE_DEBUG ((LM_DEBUG," (%t) determining the number of iterations...\n"));
  Scheduler *worker_a = new Scheduler ("worker A");

  ACE_Time_Value tstart (ACE_OS::gettimeofday ());
  ACE_Time_Value tend (ACE_OS::gettimeofday ());

  // Determine the number of iterations... we want so many that the
  // work () takes about 1 second...

  for (n_iterations = 1;
       (tend.sec () - tstart.sec ()) < 1;
       n_iterations *= 2)
    {
      tstart = ACE_OS::gettimeofday ();

      worker_a->work (0.1, n_iterations);

      tend = ACE_OS::gettimeofday ();
    }

  ACE_DEBUG ((LM_DEBUG," (%t) n_iterations %d\n", 
	      (u_long) n_iterations));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
  return n_iterations;
}

static void
test_active_object (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing active object pattern...\n"));
  // A simple example for the use of the active object pattern and
  // futures to return values from an active object.

  Scheduler *worker_a = new Scheduler ("worker A");
  Scheduler *worker_b = new Scheduler ("worker B");

  // Have worker_c delegate his work to worker_a.
  Scheduler *worker_c = new Scheduler ("worker C", worker_a);

  // loop 0:
  // test the Schedulers when they are not active.
  // now the method objects will be created but since
  // there is no active thread they will also be
  // immediately executed, in the "main" thread.
  // loop 1:
  // do the same test but with the schedulers
  // activated
  for (int i = 0; i < 2; i++)
    {
      if (i == 1) 
	{
	  worker_a->open ();
	  worker_b->open ();
	  worker_c->open ();
	}

      ACE_Future<float> fresulta = worker_a->work (0.01, n_iterations);
      ACE_Future<float> fresultb = worker_b->work (0.02, n_iterations);
      ACE_Future<float> fresultc = worker_c->work (0.03, n_iterations);

      if (i == 0) 
	{
	  if (!fresulta.ready ())
	    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker A is should be ready!!!\n"));
	  if (!fresultb.ready ())
	    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker B is should be ready!!!\n"));
	  if (!fresultc.ready ())
	    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker C is should be ready!!!\n"));
	}

      // When the workers are active we will block here until the
      // results are available.

      float resulta = fresulta;
      float resultb = fresultb;
      float resultc = fresultc;

      ACE_Future<char *> fnamea = worker_a->name ();
      ACE_Future<char *> fnameb = worker_b->name ();
      ACE_Future<char *> fnamec = worker_c->name ();

      char *namea = fnamea;
      char *nameb = fnameb;
      char *namec = fnamec;

      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %f\n",
		  namea, resulta));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %f\n",
		  nameb, resultb));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %f\n",
		  namec, resultc));
    }

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d before end ()\n",
	      (u_long) scheduler_open_count));

  worker_a->end ();
  worker_b->end ();
  worker_c->end ();

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d immediately after end ()\n",
	      (u_long) scheduler_open_count));

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d after waiting\n",
	      (u_long) scheduler_open_count));
  // @@ Can we safely delete worker_a, worker_b, and worker_c?
}

static void
test_cancellation (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing cancellation of a future...\n"));

  // Now test the cancelling a future.

  Scheduler *worker_a = new Scheduler ("worker A");
  worker_a->open ();

  ACE_Future<float> fresulta = worker_a->work (0.01, n_iterations);

  // save the result by copying the future
  ACE_Future<float> fresultb = fresulta;

  // now we cancel the first future.. but the
  // calculation will still go on...
  fresulta.cancel (10.0);

  if (!fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should be ready!!!\n"));

  float resulta = fresulta;

  ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result %f\n", resulta));

  if (resulta != 10.0)
    ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result should be 10.0!!\n", resulta));

  resulta = fresultb;

  ACE_DEBUG ((LM_DEBUG, " (%t) true result %f\n", resulta));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
}

static void
test_timeout (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing timeout on waiting for the result...\n"));
  Scheduler *worker_a = new Scheduler ("worker A");
  worker_a->open ();

  ACE_Future<float> fresulta = worker_a->work (0.01, 2 * n_iterations);

  // Should immediately return... and we should see an error...
  ACE_Time_Value *delay = new ACE_Time_Value (1);

  float resulta;
  fresulta.get (resulta, delay);

  if (fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should not be ready!!!\n"));
  else
    ACE_DEBUG ((LM_DEBUG," (%t) timed out on future A\n"));

  // now we wait until we are done...
  fresulta.get (resulta);
  ACE_DEBUG ((LM_DEBUG, " (%t) result %f\n", resulta));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
}

int
main (int, char *[]) 
{
  int n_iterations = determine_iterations ();

  test_active_object (n_iterations);
  test_cancellation (n_iterations);
  test_timeout (n_iterations);

  ACE_DEBUG ((LM_DEBUG," (%t) that's all folks!\n"));

  ACE_OS::sleep (5);
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, u_long>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
