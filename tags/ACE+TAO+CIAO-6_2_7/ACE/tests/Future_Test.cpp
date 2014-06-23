
//=============================================================================
/**
 *  @file    Future_Test.cpp
 *
 *  $Id$
 *
 *  This example tests the ACE Future and illustrates an
 *  implementation of the Active Object pattern, which is described in
 *  the POSA2 book <www.dre.vanderbilt.edu/~schmidt/POSA/> and in a
 *  paper at <www.dre.vanderbilt.edu/~schmidt/PDF/Act-Obj.pdf>.  The
 *  Active Object example here is very simple -- it determines if
 *  numbers are prime.
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author and Per Andersson <pera@ipso.se>
 */
//=============================================================================

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
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

// A counter for the tasks..
static ATOMIC_INT task_count (0);

// A counter for the futures..
static ATOMIC_INT future_count (0);

// A counter for the capsules..
static ATOMIC_INT capsule_count (0);

// A counter for the method requests...
static ATOMIC_INT method_request_count (0);

/**
 * @class Prime_Scheduler
 *
 * @brief Prime number scheduler for the Active Object.
 *
 * This class also plays the role of the Proxy and the Servant
 * in the Active Object pattern.  Naturally, these roles could
 * be split apart from the Prime_Scheduler.
 */
class Prime_Scheduler : public ACE_Task_Base
{

  friend class Method_Request_work;
  friend class Method_Request_name;
  friend class Method_Request_end;
public:
  // = Initialization and termination methods.
  /// Constructor.
  Prime_Scheduler (const ACE_TCHAR *,
                   Prime_Scheduler * = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  /// Initializer.
  virtual int open (void *args = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  /// Terminator.
  virtual int shutdown (void);

  /// Destructor.
  virtual ~Prime_Scheduler (void);

  // = These methods are part of the Active Object Proxy interface.
  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<const ACE_TCHAR*> name (void);
  void end (void);

protected:
  /// Runs the Prime_Scheduler's event loop, which dequeues
  /// <Method_Requests> and dispatches them.
  virtual int svc (void);

  // = These are the Servant methods that do the actual work.
  u_long work_i (u_long, int);
  const ACE_TCHAR *name_i (void);

private:
  // = These are the <Prime_Scheduler> implementation details.
  ACE_TCHAR *name_;
  ACE_Activation_Queue activation_queue_;
  Prime_Scheduler *scheduler_;
};

/**
 * @class Method_Request_work
 *
 * @brief Reification of the <work> method.
 */
class Method_Request_work : public ACE_Method_Request
{
public:
  Method_Request_work (Prime_Scheduler *,
                       u_long,
                       int,
                       ACE_Future<u_long> &);
  virtual ~Method_Request_work (void);

  /// This is the entry point into the Active Object method.
  virtual int call (void);

private:
  Prime_Scheduler *scheduler_;

  /// Parameter to the method that's used to determine if a number if
  /// prime.
  u_long param_;

  /// Unused.
  int count_;

  /// Store the result of the Future.
  ACE_Future<u_long> future_result_;
};

Method_Request_work::Method_Request_work (Prime_Scheduler *new_Prime_Scheduler,
                                          u_long new_param,
                                          int new_count,
                                          ACE_Future<u_long> &new_result)
  : scheduler_ (new_Prime_Scheduler),
    param_ (new_param),
    count_ (new_count),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_work created\n")));
}

Method_Request_work::~Method_Request_work (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_work will be deleted.\n")));
}

int
Method_Request_work::call (void)
{
  // Dispatch the Servant's operation and store the result into the
  // Future.
  return this->future_result_.set (this->scheduler_->work_i
                                   (this->param_,
                                    this->count_));
}

/**
 * @class Method_Request_name
 *
 * @brief Reification of the <name> method.
 */
class Method_Request_name : public ACE_Method_Request
{
public:
  Method_Request_name (Prime_Scheduler *,
                       ACE_Future<const ACE_TCHAR*> &);
  virtual ~Method_Request_name (void);

  /// This is the entry point into the Active Object method.
  virtual int call (void);

private:
  Prime_Scheduler *scheduler_;
  ACE_Future<const ACE_TCHAR*> future_result_;
};

Method_Request_name::Method_Request_name (Prime_Scheduler *new_scheduler,
                                          ACE_Future<const ACE_TCHAR*> &new_result)
  : scheduler_ (new_scheduler),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_name created\n")));
}

Method_Request_name::~Method_Request_name (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_name will be deleted.\n")));
}

int
Method_Request_name::call (void)
{
  // Dispatch the Servant's operation and store the result into the
  // Future.
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
  Method_Request_end (Prime_Scheduler *new_Prime_Scheduler);
  virtual ~Method_Request_end (void);
  virtual int call (void);

private:
  Prime_Scheduler *scheduler_;
};

Method_Request_end::Method_Request_end (Prime_Scheduler *scheduler)
  : scheduler_ (scheduler)
{
}

Method_Request_end::~Method_Request_end (void)
{
}

int
Method_Request_end::call (void)
{
  // Shut down the scheduler.
  this->scheduler_->shutdown ();
  return -1;
}

// Constructor
Prime_Scheduler::Prime_Scheduler (const ACE_TCHAR *newname,
                                  Prime_Scheduler *new_scheduler)
  : scheduler_ (new_scheduler)
{
  ACE_NEW (this->name_,
           ACE_TCHAR[ACE_OS::strlen (newname) + 1]);
  ACE_OS::strcpy ((ACE_TCHAR *) this->name_,
                  newname);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Prime_Scheduler %s created\n"),
              this->name_));
}

// Destructor

Prime_Scheduler::~Prime_Scheduler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Prime_Scheduler %s will be destroyed\n"),
              this->name_));
  delete [] this->name_;
}

// open

int
Prime_Scheduler::open (void *)
{
  task_count++;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Prime_Scheduler %s open\n"),
              this->name_));
  // Become an Active Object.
  return this->activate (THR_BOUND | THR_DETACHED);
}

// close

int
Prime_Scheduler::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Prime_Scheduler %s shutdown\n"),
              this->name_));
  task_count--;
  return 0;
}

// Service..

int
Prime_Scheduler::svc (void)
{
  for (;;)
    {
      // Dequeue the next method request (we use an auto pointer in
      // case an exception is thrown in the <call>).
      auto_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) calling method request\n")));
      // Call it.
      if (mo->call () == -1)
        break;
      // Destructor automatically deletes it.
    }

  /* NOTREACHED */
  return 0;
}

void
Prime_Scheduler::end (void)
{
  this->activation_queue_.enqueue (new Method_Request_end (this));
}

// Here's where the Work takes place.  We compute if the parameter is
// a prime number.

u_long
Prime_Scheduler::work_i (u_long param,
                         int count)
{
  ACE_UNUSED_ARG (count);

  return ACE::is_prime (param, 2, param / 2);
}

const ACE_TCHAR *
Prime_Scheduler::name_i (void)
{
  return this->name_;
}

ACE_Future<const ACE_TCHAR *>
Prime_Scheduler::name (void)
{
  if (this->scheduler_)
    // Delegate to the Prime_Scheduler.
    return this->scheduler_->name ();
  else
    {
      ACE_Future<const ACE_TCHAR*> new_future;

      // @@ What happens if new fails here?
      this->activation_queue_.enqueue
        (new Method_Request_name (this,
                                  new_future));
      return new_future;
    }
}

ACE_Future<u_long>
Prime_Scheduler::work (u_long newparam,
                       int newcount)
{
  if (this->scheduler_) {
    return this->scheduler_->work (newparam, newcount);
  }
  else {
    ACE_Future<u_long> new_future;

    this->activation_queue_.enqueue
      (new Method_Request_work (this,
                                newparam,
                                newcount,
                                new_future));
    return new_future;
  }
}

// @@ These values should be set by the command line options!

// Total number of loops.
static int n_loops = 100;

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Future_Test"));

#if defined (ACE_HAS_THREADS)
  // @@ Should make these be <auto_ptr>s...
  Prime_Scheduler *andres, *peter, *helmut, *matias;

  // Create active objects..
  ACE_NEW_RETURN (andres,
                  Prime_Scheduler (ACE_TEXT ("andres")),
                  -1);
  int result = andres->open ();
  ACE_TEST_ASSERT (result != -1);
  ACE_NEW_RETURN (peter,
                  Prime_Scheduler (ACE_TEXT ("peter")),
                  -1);
  result = peter->open ();
  ACE_TEST_ASSERT (result != -1);
  ACE_NEW_RETURN (helmut,
                  Prime_Scheduler (ACE_TEXT ("helmut")),
                  -1);
  result = helmut->open ();
  ACE_TEST_ASSERT (result != -1);

  // Matias passes all asynchronous method calls on to Andres...
  ACE_NEW_RETURN (matias,
                  Prime_Scheduler (ACE_TEXT ("matias"),
                                   andres),
                  -1);
  result = matias->open ();
  ACE_TEST_ASSERT (result != -1);

  for (int i = 0; i < n_loops; i++)
    {
      {
        ACE_Future<u_long> fresulta;
        ACE_Future<u_long> fresultb;
        ACE_Future<u_long> fresultc;
        ACE_Future<u_long> fresultd;
        ACE_Future<u_long> fresulte;
        ACE_Future<const ACE_TCHAR *> fname;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) going to do a non-blocking call\n")));

        // Spawn off the methods, which run in a separate thread as
        // active object invocations.
        fresulta = andres->work (9013);
        fresultb = peter->work (9013);
        fresultc = helmut->work (9013);
        fresultd = matias->work (9013);
        fname = andres->name ();

        // See if the result is available...
        if (fresulta.ready ())
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) wow.. work is ready.....\n")));

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) non-blocking call done... now blocking...\n")));

        // Save the result of fresulta.

        fresulte = fresulta;

        if (i % 3 == 0)
          {
            // Every 3rd time... disconnect the futures...  but
            // "fresulte" should still contain the result...
            fresulta.cancel (10ul);
            fresultb.cancel (20ul);
            fresultc.cancel (30ul);
            fresultd.cancel (40ul);
          }

        u_long resulta = 0, resultb = 0, resultc = 0, resultd = 0, resulte = 0;

        fresulta.get (resulta);
        fresultb.get (resultb);
        fresultc.get (resultc);
        fresultd.get (resultd);
        fresulte.get (resulte);

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) result a %u\n")
                    ACE_TEXT ("(%t) result b %u\n")
                    ACE_TEXT ("(%t) result c %u\n")
                    ACE_TEXT ("(%t) result d %u\n")
                    ACE_TEXT ("(%t) result e %u\n"),
                    (u_int) resulta,
                    (u_int) resultb,
                    (u_int) resultc,
                    (u_int) resultd,
                    (u_int) resulte));

        const ACE_TCHAR *name = 0;
        fname.get (name);

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) name %s\n"),
                    name));
      }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) task_count %d future_count %d ")
                  ACE_TEXT ("capsule_count %d method_request_count %d\n"),
                  task_count.value (),
                  future_count.value (),
                  capsule_count.value (),
                  method_request_count.value ()));
    }

  // Close things down.
  andres->end ();
  peter->end ();
  helmut->end ();
  matias->end ();

  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) task_count %d future_count %d ")
              ACE_TEXT ("capsule_count %d method_request_count %d\n"),
              task_count.value (),
              future_count.value (),
              capsule_count.value (),
              method_request_count.value ()));
  {
    // Check if set then get works, older versions of <ACE_Future>
    // will lock forever (or until the timer expires), will use a
    // small timer value to avoid blocking the process.

    ACE_Future<int> f1;
    f1.set (100);

    // Note you need to use absolute time, not relative time.
    ACE_Time_Value timeout (ACE_OS::gettimeofday () + ACE_Time_Value (10));
    int value = 0;

    if (f1.get (value, &timeout) == 0
        && value == 100)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Ace_Future<T>::Set followed by Ace_Future<T>::Get works.\n")));
    else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ACE_Future<T>::Set followed by Ace_Future<T>::Get does ")
                  ACE_TEXT ("not work, broken Ace_Future<> implementation.\n")));
  }

  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Checking if Ace_Future<T>::operator= is implemented ")
                ACE_TEXT ("incorrectly this might crash the program.\n")));
    ACE_Future<int> f1;
    {
      // To ensure that a rep object is created.
      ACE_Future<int> f2 (f1);
    }
    // Now it is one ACE_Future<int> referencing the rep instance

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("0.\n")));

    //Check that self assignment works.
    f1 = f1;

    // Is there any repesentation left, and if so what is the ref
    // count older ACE_Future<> implementations have deleted the rep
    // instance at this moment

    // The stuff below might crash the process if the <operator=>
    // implementation was bad.
    int value = 0;

    ACE_Time_Value timeout (ACE_OS::gettimeofday () + ACE_Time_Value (10));

    f1.set (100);
    f1.get (value, &timeout);

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("1.\n")));
    {
      // Might delete the same data a couple of times.
      ACE_Future<int> f2 (f1);
      f1.set (100);
      f1.get (value, &timeout);
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("2.\n")));
    {
      ACE_Future<int> f2 (f1);
      f1.set (100);
      f1.get (value, &timeout);
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("3.\n")));
    {
      ACE_Future<int> f2 (f1);
      f1.set (100);
      f1.get (value, &timeout);
    }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("4.\n")));
    {
      ACE_Future<int> f2 (f1);
      f1.set (100);
      f1.get (value, &timeout);
    }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("5.\n")));
    {
      ACE_Future<int> f2 (90);
      f2.get (value, &timeout);
      f1.get (value, &timeout);
    }
  }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("No it did not crash the program.\n")));

  delete andres;
  delete peter;
  delete helmut;
  delete matias;

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
