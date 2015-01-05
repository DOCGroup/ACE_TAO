
//=============================================================================
/**
 *  @file    future1.cpp
 *
 *  $Id$
 *
 *  This example tests the ACE Future.
 *
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"
#include "ace/Atomic_Op.h"



#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> ATOMIC_INT;

// a counter for the tasks..
static ATOMIC_INT task_count (0);

// a counter for the futures..
static ATOMIC_INT future_count (0);
static ATOMIC_INT future_no (0);

// a counter for the capsules..
static ATOMIC_INT capsule_count (0);
static ATOMIC_INT capsule_no (0);

// a counter for the method objects...
static ATOMIC_INT methodobject_count (0);
static ATOMIC_INT methodobject_no (0);

/**
 * @class Scheduler
 *
 * @brief Active Object Scheduler.
 */
class Scheduler : public ACE_Task_Base
{
  friend class Method_RequestWork;
public:
  Scheduler (const char *, Scheduler * = 0);
  virtual ~Scheduler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);

  virtual int svc (void);

  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<const char*> name (void);
  void end (void);

  u_long work_i (u_long, int);
  const char *name_i (void);

private:
  char *name_;
  ACE_Activation_Queue activation_queue_;
  Scheduler *scheduler_;
};

/**
 * @class Method_Request_work
 *
 * @brief Reification of the <work> method.
 */
class Method_Request_work : public ACE_Method_Request
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
  :   scheduler_ (new_Scheduler),
      param_ (new_param),
      count_ (new_count),
      future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Method_Request_work created\n"));
}

Method_Request_work::~Method_Request_work (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Method_Request_work will be deleted.\n"));
}


int
Method_Request_work::call (void)
{
  return this->future_result_.set (this->scheduler_->work_i (this->param_, this->count_));
}

/**
 * @class Method_Request_name
 *
 * @brief Reification of the <name> method.
 */
class Method_Request_name : public ACE_Method_Request
{
public:
  Method_Request_name (Scheduler *, ACE_Future<const char*> &);
  virtual ~Method_Request_name (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
  ACE_Future<const char *> future_result_;
};

Method_Request_name::Method_Request_name (Scheduler *new_scheduler,
                                        ACE_Future<const char *> &new_result)
  : scheduler_ (new_scheduler),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Method_Request_name created\n"));
}

Method_Request_name::~Method_Request_name (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Method_Request_name will be deleted.\n"));
}

int
Method_Request_name::call (void)
{
  return future_result_.set (scheduler_->name_i ());
}

/**
 * @class Method_Request_end
 *
 * @brief Reification of the <end> method.
 */
class Method_Request_end : public ACE_Method_Request
{
public:
  Method_Request_end (Scheduler *new_scheduler): scheduler_ (new_scheduler) {}
  virtual ~Method_Request_end (void) {}
  virtual int call (void) { return -1; }

private:
  /// Keep track of our scheduler.
  Scheduler *scheduler_;
};

// Constructor.
Scheduler::Scheduler (const char *newname,
                      Scheduler *new_scheduler)
{
  ACE_NEW (this->name_, char[ACE_OS::strlen (newname) + 1]);
  ACE_OS::strcpy (this->name_, newname);
  this->scheduler_ = new_scheduler;
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s created\n", this->name_));
}

// Destructor
Scheduler::~Scheduler (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s will be destroyed\n", this->name_));
  delete [] this->name_;
}

// open
int
Scheduler::open (void *)
{
  task_count++;
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s open\n", this->name_));
  return this->activate (THR_BOUND);
}

// close
int
Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s close\n", this->name_));
  task_count--;
  return 0;
}

// service..
int
Scheduler::svc (void)
{
  for (;;)
    {
      // Dequeue the next method object (we use an auto pointer in
      // case an exception is thrown in the <call>).
      auto_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

      ACE_DEBUG ((LM_DEBUG, "(%t) calling method object\n"));
      // Call it.
      if (mo->call () == -1)
        break;
      // Destructor automatically deletes it.
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

const char *
Scheduler::name_i (void)
{
  char *the_name;

  ACE_NEW_RETURN (the_name, char[ACE_OS::strlen (this->name_) + 1], 0);
  ACE_OS::strcpy (the_name, this->name_);

  return the_name;
}

ACE_Future<const char *>
Scheduler::name (void)
{
  if (this->scheduler_)
    // Delegate to the Scheduler.
    return this->scheduler_->name ();
  else
    {
      ACE_Future<const char*> new_future;

      // @@ What happens if new fails here?
      this->activation_queue_.enqueue
        (new Method_Request_name (this, new_future));

      return new_future;
    }
}

ACE_Future<u_long>
Scheduler::work (u_long newparam,
                 int newcount)
{
  if (this->scheduler_) {
    return this->scheduler_->work (newparam, newcount);
  }
  else {
    ACE_Future<u_long> new_future;

    this->activation_queue_.enqueue
      (new Method_Request_work (this, newparam, newcount, new_future));
    return new_future;
  }
}

// @@ These values should be set by the command line options!

// Total number of loops.
static size_t n_loops = 100;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Scheduler *andres, *peter, *helmut, *matias;

  // Create active objects..
  // @@ Should "open" be subsumed within the constructor of
  // Scheduler()?
  ACE_NEW_RETURN (andres, Scheduler ("andres"), -1);
  andres->open ();
  ACE_NEW_RETURN (peter, Scheduler ("peter"), -1);
  peter->open ();
  ACE_NEW_RETURN (helmut, Scheduler ("helmut"), -1);
  helmut->open ();

  // Matias passes all asynchronous method calls on to Andres...
  ACE_NEW_RETURN (matias, Scheduler ("matias", andres), -1);
  matias->open ();

  for (size_t i = 0; i < n_loops; i++)
    {
      {
        ACE_Future<u_long> fresulta, fresultb, fresultc, fresultd, fresulte;
        ACE_Future<const char *> fname;

        ACE_DEBUG ((LM_DEBUG, "(%t) going to do a non-blocking call\n"));

        fresulta = andres->work (9013);
        fresultb = peter->work (9013);
        fresultc = helmut->work (9013);
        fresultd = matias->work (9013);
        fname = andres->name ();

        // see if the result is available...
        if (fresulta.ready ())
          ACE_DEBUG ((LM_DEBUG, "(%t) wow.. work is ready.....\n"));

        ACE_DEBUG ((LM_DEBUG, "(%t) non-blocking call done... now blocking...\n"));

        // Save the result of fresulta.

        fresulte = fresulta;

        if (i % 3 == 0)
          {
            // Every 3rd time... disconnect the futures...
            // but "fresulte" should still contain the result...
            fresulta.cancel (10);
            fresultb.cancel (20);
            fresultc.cancel (30);
            fresultd.cancel (40);
          }

        u_long resulta = 0, resultb = 0, resultc = 0, resultd = 0, resulte = 0;

        fresulta.get (resulta);
        fresultb.get (resultb);
        fresultc.get (resultc);
        fresultd.get (resultd);
        fresulte.get (resulte);

        ACE_DEBUG ((LM_DEBUG, "(%t) result a %u\n", (u_int) resulte));
        ACE_DEBUG ((LM_DEBUG, "(%t) result b %u\n", (u_int) resulta));
        ACE_DEBUG ((LM_DEBUG, "(%t) result c %u\n", (u_int) resultb));
        ACE_DEBUG ((LM_DEBUG, "(%t) result d %u\n", (u_int) resultc));
        ACE_DEBUG ((LM_DEBUG, "(%t) result e %u\n", (u_int) resultd));

        const char *name = 0;

        fname.get (name);

        ACE_DEBUG ((LM_DEBUG, "(%t) name %s\n", name));
        delete [] (char *) name;
      }

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) task_count %d future_count %d capsule_count %d methodobject_count %d\n",
                  task_count.value (),
                  future_count.value (),
                  capsule_count.value (),
                  methodobject_count.value ()));
    }

  // Close things down.
  andres->end ();
  peter->end ();
  helmut->end ();
  matias->end ();

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) task_count %d future_count %d capsule_count %d methodobject_count %d\n",
              task_count.value (),
              future_count.value (),
              capsule_count.value (),
              methodobject_count.value ()));

  ACE_DEBUG ((LM_DEBUG,"(%t) th' that's all folks!\n"));

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
