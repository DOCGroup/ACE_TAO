// $Id$

// ============================================================================
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
// 08/27/96; A.Kruse; - the friends of Scheduler are "Method_Request_name"
//                      and "Method_Request_work".
//                    - make the methods "work_i" and "name_i" private
// 09/2/96; D.Schmidt; Integrate with new ACE_Future API and rearrange
//                     the tests so they are more modular.
// ============================================================================

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"
#include "ace/Atomic_Op.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> ATOMIC_INT;

// a counter for the tasks..
static ATOMIC_INT scheduler_open_count (0);

// forward declarations
class Method_Request_work;
class Method_Request_name;

class Scheduler : public ACE_Task_Base
  // = TITLE
  //     Active Object Scheduler.
{
  // Every method object has to be able to access the private methods.

  friend class Method_Request_work;
  friend class Method_Request_name;
  friend class Method_Request_end;
public:

  Scheduler (const char *, Scheduler * = 0);
  virtual ~Scheduler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *args = 0);
  // The method that is used to start the active object.
  //FUZZ: enable check_for_lack_ACE_OS

  // = Here are the methods exported by the class. They return an
  // <ACE_Future>.
  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<char*> name (void);
  void end (void);

private:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int close (u_long flags = 0);
  // Should not be accessible from outside...  (use end () instead).
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int svc (void);
  // Here the actual servicing of all requests is happening..

  // = Implementation methods.
  u_long work_i (u_long, int);
  char *name_i (void);

  char *name_;
  ACE_Activation_Queue activation_queue_;
  Scheduler *scheduler_;
};

class Method_Request_work : public ACE_Method_Request
  // = TITLE
  //     Reification of the <work> method.
{
public:
  Method_Request_work (Scheduler *, u_long, int, ACE_Future<u_long> &);
  virtual ~Method_Request_work (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
  u_long param_;
  int count_;
  ACE_Future<u_long> future_result_;
};

Method_Request_work::Method_Request_work (Scheduler* new_Scheduler,
                                        u_long new_param,
                                        int new_count,
                                        ACE_Future<u_long> &new_result)
  : scheduler_ (new_Scheduler),
    param_ (new_param),
    count_ (new_count),
    future_result_ (new_result)
{
}

Method_Request_work::~Method_Request_work (void)
{
}

int
Method_Request_work::call (void)
{
  return this->future_result_.set (this->scheduler_->work_i (this->param_, this->count_));
}

class Method_Request_name : public ACE_Method_Request
  // = TITLE
  //     Reification of the <name> method.
{
public:
  Method_Request_name (Scheduler *, ACE_Future<char*> &);
  virtual ~Method_Request_name (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
  ACE_Future<char*> future_result_;
};


Method_Request_name::Method_Request_name (Scheduler *new_scheduler,
                                        ACE_Future<char*> &new_result)
  : scheduler_ (new_scheduler),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              " (%t) Method_Request_name created\n"));
}

Method_Request_name::~Method_Request_name (void)
{
  ACE_DEBUG ((LM_DEBUG,
              " (%t) Method_Request_name will be deleted.\n"));
}

int
Method_Request_name::call (void)
{
  return future_result_.set (scheduler_->name_i ());
}

class Method_Request_end : public ACE_Method_Request
  // = TITLE
  //     Reification of the <end> method.
{
public:
  Method_Request_end (Scheduler *new_Scheduler): scheduler_ (new_Scheduler) {}
  virtual ~Method_Request_end (void) {}
  virtual int call (void) { return -1; }

private:
  Scheduler *scheduler_;
  // Keep track of our scheduler.
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
      auto_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

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
  this->activation_queue_.enqueue (new Method_Request_end (this));
}

// Here's where the Work takes place.
u_long
Scheduler::work_i (u_long param,
                   int count)
{
  ACE_UNUSED_ARG (count);

  return ACE::is_prime (param, 2, param / 2);
}

char *
Scheduler::name_i (void)
{
  char *the_name;

  ACE_NEW_RETURN (the_name, char[ACE_OS::strlen (this->name_) + 1], 0);
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

          auto_ptr<ACE_Method_Request> mo (new Method_Request_name (this, new_future));

          mo->call ();
          // Smart pointer destructor automatically deletes mo.
        }
      else
        // @@ What happens if new fails here?
        this->activation_queue_.enqueue
          (new Method_Request_name (this, new_future));

      return new_future;
    }
}

ACE_Future<u_long>
Scheduler::work (u_long newparam, int newcount)
{
  if (this->scheduler_)
    return this->scheduler_->work (newparam, newcount);
  else
    {
      ACE_Future<u_long> new_future;

      if (this->thr_count () == 0)
        {
          auto_ptr<ACE_Method_Request> mo
            (new Method_Request_work (this, newparam, newcount, new_future));
          mo->call ();
          // Smart pointer destructor automatically deletes it.
        }
      else
        this->activation_queue_.enqueue
          (new Method_Request_work (this, newparam, newcount, new_future));

      return new_future;
    }
}

static int
determine_iterations (void)
{
  int n_iterations;

  ACE_DEBUG ((LM_DEBUG," (%t) determining the number of iterations...\n"));
  Scheduler *worker_a;

  ACE_NEW_RETURN (worker_a, Scheduler ("worker A"), -1);

  ACE_Time_Value tstart (ACE_OS::gettimeofday ());
  ACE_Time_Value tend (ACE_OS::gettimeofday ());

  // Determine the number of iterations... we want so many that the
  // work () takes about 1 second...

  for (n_iterations = 1;
       (tend.sec () - tstart.sec ()) < 1;
       n_iterations *= 2)
    {
      tstart = ACE_OS::gettimeofday ();

      worker_a->work (9013, n_iterations);

      tend = ACE_OS::gettimeofday ();
    }

  ACE_DEBUG ((LM_DEBUG," (%t) n_iterations %d\n",
              (int) n_iterations));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
  return n_iterations;
}

static void
test_active_object (int n_iterations)
{
  ACE_UNUSED_ARG (n_iterations);

  ACE_DEBUG ((LM_DEBUG," (%t) testing active object pattern...\n"));
  // A simple example for the use of the active object pattern and
  // futures to return values from an active object.

  Scheduler *worker_a;
  Scheduler *worker_b;
  Scheduler *worker_c;

  ACE_NEW (worker_a, Scheduler ("worker A"));
  ACE_NEW (worker_b, Scheduler ("worker B"));
  // Have worker_c delegate his work to worker_a.
  ACE_NEW (worker_c, Scheduler ("worker C", worker_a));

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

      ACE_Future<u_long> fresulta = worker_a->work (9013);
      ACE_Future<u_long> fresultb = worker_b->work (9013);
      ACE_Future<u_long> fresultc = worker_c->work (9013);

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

      u_long resulta = fresulta;
      u_long resultb = fresultb;
      u_long resultc = fresultc;

      ACE_Future<char *> fnamea = worker_a->name ();
      ACE_Future<char *> fnameb = worker_b->name ();
      ACE_Future<char *> fnamec = worker_c->name ();

      char *namea = fnamea;
      char *nameb = fnameb;
      char *namec = fnamec;

      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  namea, (u_int) resulta));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  nameb, (u_int) resultb));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  namec, (u_int) resultc));
    }

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d before end ()\n",
              scheduler_open_count.value ()));

  worker_a->end ();
  worker_b->end ();
  worker_c->end ();

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d immediately after end ()\n",
              scheduler_open_count.value ()));

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d after waiting\n",
              scheduler_open_count.value ()));
  // @@ Can we safely delete worker_a, worker_b, and worker_c?
}

static void
test_cancellation (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing cancellation of a future...\n"));

  // Now test the cancelling a future.

  Scheduler *worker_a;
  ACE_NEW (worker_a, Scheduler ("worker A"));
  worker_a->open ();

  ACE_Future<u_long> fresulta = worker_a->work (9013, n_iterations);

  // save the result by copying the future
  ACE_Future<u_long> fresultb = fresulta;

  // now we cancel the first future.. but the
  // calculation will still go on...
  fresulta.cancel (10);

  if (!fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should be ready!!!\n"));

  u_long resulta = fresulta;

  ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result %u\n", (u_int) resulta));

  if (resulta != 10)
    ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result should be 10!!\n", resulta));

  resulta = fresultb;

  ACE_DEBUG ((LM_DEBUG, " (%t) true result %u\n", (u_int) resulta));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
}

static void
test_timeout (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing timeout on waiting for the result...\n"));
  Scheduler *worker_a;
  ACE_NEW (worker_a, Scheduler ("worker A"));
  worker_a->open ();

  ACE_Future<u_long> fresulta = worker_a->work (9013, 2 * n_iterations);

  // Should immediately return... and we should see an error...
  ACE_Time_Value *delay;
  ACE_NEW (delay, ACE_Time_Value (1));

  u_long resulta = 0;
  fresulta.get (resulta, delay);

  if (fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should not be ready!!!\n"));
  else
    ACE_DEBUG ((LM_DEBUG," (%t) timed out on future A\n"));

  // now we wait until we are done...
  fresulta.get (resulta);
  ACE_DEBUG ((LM_DEBUG, " (%t) result %u\n", (u_int) resulta));

  worker_a->end ();
  // @@ Can we safely delete worker_a here?
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int n_iterations = determine_iterations ();

  test_active_object (n_iterations);
  test_cancellation (n_iterations);
  test_timeout (n_iterations);

  ACE_DEBUG ((LM_DEBUG," (%t) that's all folks!\n"));

  ACE_OS::sleep (5);
  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
