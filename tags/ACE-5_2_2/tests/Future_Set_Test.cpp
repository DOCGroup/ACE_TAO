// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Future_Set_Test.cpp
//
// = DESCRIPTION
//    This example tests the ACE Future Set and illustrates an
//    implementation of the Active Object pattern, which is available
//    at <http://www.cs.wustl.edu/~schmidt/Act-Obj.ps.gz>.  The
//    Active Object itself is very simple -- it determines if numbers
//    are prime.
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch>,
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>,
//    Per Andersson <pera@ipso.se> and
//    Johnny Tucker <jtucker@infoglide.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Future_Set.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"
#include "ace/Atomic_Op.h"

ACE_RCSID(tests, Future_Set_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> ATOMIC_INT;

// A counter for the tasks..
static ATOMIC_INT task_count (0);

class Prime_Scheduler : public ACE_Task_Base
{
  // = TITLE
  //     Prime number scheduler for the Active Object.
  //
  // = DESCRIPTION
  //     This class also plays the role of the Proxy and the Servant
  //     in the Active Object pattern.  Naturally, these roles could
  //     be split apart from the Prime_Scheduler.

  friend class Method_Request_work;
  friend class Method_Request_name;
  friend class Method_Request_end;
public:
  // = Initialization and termination methods.
  Prime_Scheduler (const ACE_TCHAR *,
                   Prime_Scheduler * = 0);
  // Constructor.

  virtual int open (void *args = 0);
  // Initializer.

  virtual int close (u_long flags = 0);
  // Terminator.

  virtual ~Prime_Scheduler (void);
  // Destructor.

  // = These methods are part of the Active Object Proxy interface.
  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<const ACE_TCHAR*> name (void);
  void end (void);

protected:
  virtual int svc (void);
  // Runs the Prime_Scheduler's event loop, which dequeues
  // <Method_Requests> and dispatches them.

  // = These are the Servant methods that do the actual work.
  u_long work_i (u_long, int);
  const ACE_TCHAR *name_i (void);

private:
  // = These are the <Prime_Scheduler> implementation details.
  ACE_TCHAR *name_;
  ACE_Activation_Queue activation_queue_;
  Prime_Scheduler *scheduler_;
};

class Method_Request_work : public ACE_Method_Request
{
  // = TITLE
  //     Reification of the <work> method.
public:
  Method_Request_work (Prime_Scheduler *,
                       u_long,
                       int,
                       ACE_Future<u_long> &);
  virtual ~Method_Request_work (void);

  virtual int call (void);
  // This is the entry point into the Active Object method.

private:
  Prime_Scheduler *scheduler_;

  u_long param_;
  // Parameter to the method that's used to determine if a number if
  // prime.

  int count_;
  // Unused.

  ACE_Future<u_long> future_result_;
  // Store the result of the Future.
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

class Method_Request_name : public ACE_Method_Request
{
  // = TITLE
  //     Reification of the <name> method.
public:
  Method_Request_name (Prime_Scheduler *,
                       ACE_Future<const ACE_TCHAR*> &);
  virtual ~Method_Request_name (void);

  virtual int call (void);
  // This is the entry point into the Active Object method.

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

class Method_Request_end : public ACE_Method_Request
{
  // = TITLE
  //     Reification of the <end> method.
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
  this->scheduler_->close ();
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
Prime_Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Prime_Scheduler %s close\n"),
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
template class ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>;
template class ACE_Future_Holder<const ACE_TCHAR *>;
template class ACE_Future_Holder<u_long>;
template class ACE_Future_Observer<const ACE_TCHAR *>;
template class ACE_Future_Observer<u_long>;
template class ACE_Future<const ACE_TCHAR *>;
template class ACE_Future<u_long>;
template class ACE_Future_Rep<const ACE_TCHAR *>;
template class ACE_Future_Rep<u_long>;
template class ACE_Future_Set<const ACE_TCHAR *>;
template class ACE_Future_Set<u_long>;
template class auto_ptr<ACE_Method_Request>;
template class ACE_Auto_Basic_Ptr<ACE_Method_Request>;
template class ACE_Node<ACE_Future_Observer<const ACE_TCHAR *> *>;
template class ACE_Node<ACE_Future_Observer<u_long> *>;
template class ACE_Unbounded_Set<ACE_Future_Observer<const ACE_TCHAR *> *>;
template class ACE_Unbounded_Set<ACE_Future_Observer<u_long> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Future_Observer<const ACE_TCHAR *> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Future_Observer<u_long> *>;
template class ACE_Pointer_Hash<u_long_key>;
template class ACE_Equal_To<u_long_key>;
template class ACE_Hash_Map_Entry<u_long_key, u_long_value>;
template class ACE_Hash_Map_Manager_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>;
template class ACE_Pointer_Hash<char_star_key>;
template class ACE_Equal_To<char_star_key>;
template class ACE_Hash_Map_Entry<char_star_key, char_star_value>;
template class ACE_Hash_Map_Manager_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Future_Holder<const ACE_TCHAR *>
#pragma instantiate ACE_Future_Holder<u_long>
#pragma instantiate ACE_Future_Observer<const ACE_TCHAR *>
#pragma instantiate ACE_Future_Observer<u_long>
#pragma instantiate ACE_Future<const ACE_TCHAR *>
#pragma instantiate ACE_Future<u_long>
#pragma instantiate ACE_Future_Rep<const ACE_TCHAR *>
#pragma instantiate ACE_Future_Rep<u_long>
#pragma instantiate ACE_Future_Set<const ACE_TCHAR *>
#pragma instantiate ACE_Future_Set<u_long>
#pragma instantiate auto_ptr<ACE_Method_Request>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Method_Request>
#pragma instantiate ACE_Node<ACE_Future_Observer<const ACE_TCHAR *> *>
#pragma instantiate ACE_Node<ACE_Future_Observer<u_long> *>
#pragma instantiate ACE_Unbounded_Set<ACE_Future_Observer<const ACE_TCHAR *> *>
#pragma instantiate ACE_Unbounded_Set<ACE_Future_Observer<u_long> *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Future_Observer<const ACE_TCHAR *> *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Future_Observer<u_long> *>
#pragma instantiate ACE_Pointer_Hash<u_long_key>
#pragma instantiate ACE_Equal_To<u_long_key>
#pragma instantiate ACE_Hash_Map_Entry<u_long_key, u_long_value>
#pragma instantiate ACE_Hash_Map_Manager_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<u_long_key, u_long_value, ACE_Pointer_Hash<u_long_key>, ACE_Equal_To<u_long_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Pointer_Hash<char_star_key>
#pragma instantiate ACE_Equal_To<char_star_key>
#pragma instantiate ACE_Hash_Map_Entry<char_star_key, char_star_value>
#pragma instantiate ACE_Hash_Map_Manager_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<char_star_key, char_star_value, ACE_Pointer_Hash<char_star_key>, ACE_Equal_To<char_star_key>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Future_Set_Test"));

#if defined (ACE_HAS_THREADS)
  // @@ Should make these be <auto_ptr>s...
  Prime_Scheduler *andres, *peter, *helmut, *matias;

  // Create active objects..
  ACE_NEW_RETURN (andres,
                  Prime_Scheduler (ACE_TEXT ("andres")),
                  -1);
  ACE_ASSERT (andres->open () != -1);
  ACE_NEW_RETURN (peter,
                  Prime_Scheduler (ACE_TEXT ("peter")),
                  -1);
  ACE_ASSERT (peter->open () != -1);
  ACE_NEW_RETURN (helmut,
                  Prime_Scheduler (ACE_TEXT ("helmut")),
                  -1);
  ACE_ASSERT (helmut->open () != -1);

  // Matias passes all asynchronous method calls on to Andres...
  ACE_NEW_RETURN (matias,
                  Prime_Scheduler (ACE_TEXT ("matias"),
                                   andres),
                  -1);
  ACE_ASSERT (matias->open () != -1);

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

  const ACE_TCHAR *name;
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

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) task_count %d\n"),
              task_count.value () ));

  ACE_OS::sleep (5);

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
