// $Id$

//=============================================================================
/**
 *  @file    Bound_Ptr_Test.cpp
 *
 *  $Id$
 *
 *  This example tests the <ACE_Strong_Bound_Ptr> and
 *  <ACE_Weak_Bound_Ptr> and illustrates how they may be dispersed
 *  between multiple threads using an implementation of the Active
 *  Object pattern, which is available in the POSA2 book
 *  <http://www.cs.wustl.edu/~schmidt/POSA>.
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Null_Mutex.h"
#include "ace/Method_Request.h"
#include "Bound_Ptr_Test.h"



// The following Parent and Child classes illustrate how you might use the
// ACE_Strong_Bound_Ptr and ACE_Weak_Bound_Ptr together in cyclic
// relationships.

struct Child_Base
{
  virtual ~Child_Base (void);

  // Perform some operation.
  virtual void do_something (void) = 0;
};


// This class should only be created on the heap. Normally it would be an
// abstract class, and the implementation would be elsewhere.
struct Parent
{
  Parent (void);
  ~Parent (void);

  // Weak pointer to this object used to hand out new references. Must be
  // weak since it can't own itself!
  ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> weak_self_;

  // The parent owns the child. When the parent is destroyed the child will
  // be automatically deleted.
  ACE_Strong_Bound_Ptr<Child_Base, ACE_Null_Mutex> child_;

  // Called by the child to perform some operation.
  void do_something (void);

  static size_t instance_count_;
};

// This class should only be created on the heap. Normally it would be an
// abstract class, and the implementation would be elsewhere.
struct Child : public Child_Base
{
  Child (ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> parent);
  virtual ~Child (void);

  // Back pointer to the parent. The child does not own the parent so has no
  // effect on its lifetime.
  ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> parent_;

  // Perform some operation. Delegates the work to the parent.
  virtual void do_something (void);

  static size_t instance_count_;
};

Child_Base::~Child_Base (void)
{
}


size_t Parent::instance_count_ = 0;

Parent::Parent (void)
  : weak_self_(this),
    child_(new Child(weak_self_))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Creating Parent object\n")));
  ++Parent::instance_count_;
}

Parent::~Parent (void)
{
  --Parent::instance_count_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Deleting Parent object\n")));
}

void Parent::do_something (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Parent doing something\n")));
}

size_t Child::instance_count_ = 0;

Child::Child (ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> parent)
  : parent_(parent)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Creating Child object\n")));
  ++Child::instance_count_;
}

Child::~Child (void)
{
  --Child::instance_count_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Deleting Child object\n")));
}

void Child::do_something (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Child doing something\n")));

  // Using operator-> on a weak pointer will automatically create a strong
  // pointer as a temporary. This ensures that the object exists for the
  // lifetime of the call (although it does not check for null).
  parent_->do_something ();

  // In cases where we may need to call operations on the weak pointer
  // many times, we can reduce the overhead by explicitly converting to a
  // strong pointer first.
  ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> strong_parent (parent_);

  // You can check for null to see if the parent object still exists (in this
  // case it is not strictly necessary since the child will only exist if the
  // parent still exists).
  if (strong_parent == 0)
    return;

  for (int i = 0; i < 5; ++i)
    strong_parent->do_something ();
}

size_t Printer::instance_count_ = 0;

Printer::Printer (const char *message)
  : message_ (message)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Creating Printer object\n")));
  ++Printer::instance_count_;
}

Printer::~Printer (void)
{
  --Printer::instance_count_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Deleting Printer object\n")));
}

void
Printer::print (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) %C\n"),
              this->message_));
}

#if defined (ACE_HAS_THREADS)

/**
 * @class Method_Request_print
 *
 * @brief Reification of the <print> method.
 */
class Method_Request_print : public ACE_Method_Request
{
public:
  Method_Request_print (Scheduler *,
                        Printer_var &printer);
  virtual ~Method_Request_print (void);

  /// This is the entry point into the Active Object method.
  virtual int call (void);

private:
  Scheduler *scheduler_;
  Printer_var printer_;
};

Method_Request_print::Method_Request_print (Scheduler *new_scheduler,
                                            Printer_var &printer)
  : scheduler_ (new_scheduler),
    printer_ (printer)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_print created\n")));
}

Method_Request_print::~Method_Request_print (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_print will be deleted.\n")));
}

int
Method_Request_print::call (void)
{
  // Dispatch the Servant's operation and store the result into the
  // Future.
  Printer_var temp = printer_;

  temp->print ();

  return 0;
}

/**
 * @class Method_Request_end
 *
 * @brief Reification of the <end> method.
 */
class Method_Request_end : public ACE_Method_Request
{
public:
  Method_Request_end (Scheduler *new_Prime_Scheduler);
  virtual ~Method_Request_end (void);
  virtual int call (void);

private:
  Scheduler *scheduler_;
};

Method_Request_end::Method_Request_end (Scheduler *scheduler)
  : scheduler_ (scheduler)
{
}

Method_Request_end::~Method_Request_end (void)
{
}

int
Method_Request_end::call (void)
{
  // Shut down the scheduler by deactivating the activation queue's
  // underlying message queue - should pop all worker threads off their
  // wait and they'll exit.
  this->scheduler_->msg_queue ()->deactivate ();
  return -1;
}

// Constructor
// Associates the activation queue with this task's message queue,
// allowing easy access to the message queue for shutting it down
// when it's time to stop this object's service threads.
Scheduler::Scheduler (Scheduler *new_scheduler)
  : activation_queue_ (msg_queue ()), scheduler_ (new_scheduler)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Scheduler created\n")));
}

// Destructor

Scheduler::~Scheduler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Scheduler will be destroyed\n")));
}

// open

int
Scheduler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Scheduler open\n")));
  // Become an Active Object.
  int num_threads = 3;
  return this->activate (THR_BOUND | THR_JOINABLE, num_threads);
}

// close

int
Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) rundown\n")));
  return 0;
}

// Service..

int
Scheduler::svc (void)
{
  for (;;)
    {
      // Dequeue the next method request (we use an strong pointer in
      // case an exception is thrown in the <call>).
      ACE_Strong_Bound_Ptr<ACE_Method_Request, ACE_Null_Mutex> mo (this->activation_queue_.dequeue ());
      if (mo == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) activation queue shut down\n")));
          break;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) calling method request\n")));
      // Call it.
      if (mo->call () == -1)
        break;

      // Destructor automatically deletes it.
    }

  return 0;
}

void
Scheduler::end (void)
{
  this->activation_queue_.enqueue (new Method_Request_end (this));
}

// Here's where the work takes place.

void
Scheduler::print (Printer_var &printer)
{
  this->activation_queue_.enqueue
    (new Method_Request_print (this,
                               printer));
}

// Total number of loops.
static int n_loops = 10;

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bound_Ptr_Test"));


  // =========================================================================
  // The following test uses the ACE_Strong_Bound_Ptr in a single
  // thread of control, hence we use the ACE_Null_Mutex

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing synchronous test...\n")));

  Parent *parent1 = 0;
  ACE_NEW_RETURN (parent1,
                  Parent,
                  -1);
  ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> p8;
  {
    // Must get the pointer from the parent object's weak_self_ member.
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p(parent1->weak_self_);
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p1(p);
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p2(p);
    ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> p3(p);
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p4(p);
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p5 = p2;
    ACE_Strong_Bound_Ptr<Parent, ACE_Null_Mutex> p6 = p3;
    ACE_Weak_Bound_Ptr<Parent, ACE_Null_Mutex> p7(p1);
    p8 = p2;
    p->child_->do_something ();
  }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Parent instance count is %d, expecting 0\n"),
              Parent::instance_count_));
  if (Parent::instance_count_ != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) parent instance count not 0...\n")),
                         -1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Child instance count is %d, expecting 0\n"),
              Child::instance_count_));
  if (Child::instance_count_ != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) child instance count not 0...\n")),
                         -1);
    }
  // Weak pointer should now be set to null.
  if(!p8.null ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) p8 not nill...\n")),
                         -1);
    }

  Printer *printer1 = 0;
  ACE_NEW_RETURN (printer1,
                  Printer ("I am printer 1"),
                  -1);
  ACE_Weak_Bound_Ptr<Printer, ACE_Null_Mutex> r9;
  {
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r(printer1);
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r1(r);
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r2(r);
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r3(r);
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r4(r);
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r5 = r2;
    ACE_Strong_Bound_Ptr<Printer, ACE_Null_Mutex> r6 = r1;
    ACE_Weak_Bound_Ptr<Printer, ACE_Null_Mutex> r7(r1);
    ACE_Weak_Bound_Ptr<Printer, ACE_Null_Mutex> r8 = r2;
    r9 = r3;
    r9->print ();
  }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer instance count is %d, expecting 0\n"),
              Printer::instance_count_));
  if (Printer::instance_count_ != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Printer instance count not 0...\n")),
                         -1);
    }
  // Weak pointer should now be set to null.
  if (!r9.null ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) r9 not nill...\n")),
                         -1);
    }

#if defined (ACE_HAS_THREADS)

  // =========================================================================
  // The following test uses the ACE_Strong_Bound_Ptr in multiple
  // threads of control.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing asynchronous test...\n")));

  Scheduler *scheduler_ptr = 0;

  // Create active objects..
  ACE_NEW_RETURN (scheduler_ptr,
                  Scheduler (),
                  -1);

  ACE_Strong_Bound_Ptr<Scheduler, ACE_Null_Mutex> scheduler(scheduler_ptr);

  if (scheduler->open () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Scheduler open failed...\n")),
                         -1);
    }

  {
    Printer *printer2 = 0;
    ACE_NEW_RETURN (printer2,
                    Printer ("I am printer 2"),
                    -1);

    // Ownership is transferred from the auto_ptr to the strong pointer.
    auto_ptr<Printer> a (printer2);
    Printer_var r (a);

    for (int i = 0; i < n_loops; i++)
      // Spawn off the methods, which run in a separate thread as
      // active object invocations.
      scheduler->print (r);
  }

  // Close things down.
  scheduler->end ();

  scheduler->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer instance count is %d, expecting 0\n"),
              Printer::instance_count_));
  if (Printer::instance_count_ != 0)
    return -1;

#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;

  return 0;
}
