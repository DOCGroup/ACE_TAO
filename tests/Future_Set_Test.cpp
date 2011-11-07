
//=============================================================================
/**
 *  @file    Future_Set_Test.cpp
 *
 *  $Id$
 *
 *  This example tests the ACE Future Set and illustrates an
 *  implementation of the Active Object pattern, which is available
 *  at <http://www.cs.wustl.edu/~schmidt/PDF/Act-Obj.pdf>.  The
 *  Active Object itself is very simple -- it determines if numbers
 *  are prime.
 *
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Per Andersson <pera@ipso.se> and Johnny Tucker <jtucker@infoglide.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Future_Set.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"
#include "ace/Atomic_Op.h"
#include "ace/Null_Mutex.h"



#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> ATOMIC_INT;

// A counter for the tasks..
static ATOMIC_INT task_count (0);

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

  /// Terminator.
  //FUZZ: enable check_for_lack_ACE_OS
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

typedef ACE_Future_Rep<u_long> *u_long_key;
typedef ACE_Future_Holder<u_long> *u_long_value;

typedef ACE_Future_Rep<const ACE_TCHAR *> *char_star_key;
typedef ACE_Future_Holder<const ACE_TCHAR *> *char_star_value;

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Future_Set_Test"));

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

  ACE_Future<u_long> fresulta;
  ACE_Future<u_long> fresultb;
  ACE_Future<u_long> fresultc;
  ACE_Future<u_long> fresultd;
  ACE_Future<const ACE_TCHAR *> fname;

  ACE_Future_Set<u_long> fseta;
  ACE_Future_Set<u_long> fsetb;
  ACE_Future_Set<u_long> fsetc;
  ACE_Future_Set<u_long> fsetd;
  ACE_Future_Set<const ACE_TCHAR *> fsetname;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) initializing future sets with non-blocking call\n")));

  for (int i = 0; i < n_loops; i++)
    {
      // Spawn off the methods, which run in a separate thread as
      // active object invocations.
      fresulta = andres->work (9013);
      fresultb = peter->work (9013);
      fresultc = helmut->work (9013);
      fresultd = matias->work (9013);
      fname = andres->name ();

      fsetname.insert (fname);
      fname = peter->name ();
      fsetname.insert (fname);
      fname = helmut->name ();

      fseta.insert (fresulta);
      fsetb.insert (fresultb);
      fsetc.insert (fresultc);
      fsetd.insert (fresultd);
      fsetname.insert (fname);
    }


  // See if the result is available...

  if (!fseta.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set a is not empty.....\n")));

  if (!fsetb.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set b is not empty.....\n")));

  if (!fsetc.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set c is not empty.....\n")));

  if (!fsetd.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set d is not empty.....\n")));

  if (!fsetname.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set name is not empty.....\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) non-blocking calls done... now blocking...\n")));

  // Save the result of fresulta.

  u_long resulta = 0;
  u_long resultb = 0;
  u_long resultc = 0;
  u_long resultd = 0;

  u_int count = 0;
  while (fseta.next_readable (fresulta))
    {
      fresulta.get (resulta);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) result(%u) a %u\n"),
                  count,
                  (u_int) resulta));
    }

  count = 0;
  while (fsetb.next_readable (fresultb))
    {
      fresultb.get (resultb);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) result(%u) b %u\n"),
                  count,
                  (u_int) resultb));
    }

  count = 0;
  while (fsetc.next_readable (fresultc))
    {
      fresultc.get (resultc);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) result(%u) c %u\n"),
                  count,
                  (u_int) resultc));
    }

  count = 0;
  while (fsetd.next_readable (fresultd))
    {
      fresultd.get (resultd);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) result(%u) d %u\n"),
                  count,
                  (u_int) resultd));
    }

  const ACE_TCHAR *name = 0;
  count = 0;
  while (fsetname.next_readable (fname))
    {
      fname.get (name);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) result(%u) name %s\n"),
                  count,
                  name));
    }

  if (fseta.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set a is empty.....\n")));

  if (fsetb.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set b is empty.....\n")));

  if (fsetc.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set c is empty.....\n")));

  if (fsetd.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set d is empty.....\n")));

  if (fsetname.is_empty ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) wow.. set name is empty.....\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) task_count %d\n"),
              task_count.value () ));

  // Close things down.
  andres->end ();
  peter->end ();
  helmut->end ();
  matias->end ();

  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) task_count %d\n"),
              task_count.value () ));

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
