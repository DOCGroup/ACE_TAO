// $Id$

//=============================================================================
/**
 *  @file    Refcounted_Auto_Ptr_Test.cpp
 *
 *  $Id$
 *
 *  This example tests the <ACE_Refcounted_Auto_Ptr> and illustrates
 *  how they may be dispersed between multiple threads using an
 *  implementation of the Active Object pattern, which is available
 *  at <http://www.cs.wustl.edu/~schmidt/PDF/Act-Obj.pdf>.
 *
 *  @author Johnny Tucker <johnny_tucker@yahoo.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Null_Mutex.h"
#include "ace/Activation_Queue.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "Refcounted_Auto_Ptr_Test.h"

ACE_Atomic_Op<ACE_SYNCH_MUTEX, unsigned int> Printer::current_instance_ (0);
ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> Printer::instance_count_ (0);

Printer::Printer (const char *message)
  : message_ (message)
{
  this->which_ = ++Printer::current_instance_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Creating Printer object %d (%C)\n"),
              this->which_,
              this->message_));
  ++Printer::instance_count_;
}

Printer::~Printer (void)
{
  --Printer::instance_count_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Deleting Printer object %d (%C)\n"),
              this->which_,
              this->message_));
}

void
Printer::print (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) %C\n"),
              this->message_));
}

#if defined (ACE_HAS_THREADS)

typedef ACE_Refcounted_Auto_Ptr<Printer, ACE_Thread_Mutex> Printer_var;

/**
 * @class Scheduler
 *
 * @brief The scheduler for the Active Object.
 *
 * This class also plays the role of the Proxy and the Servant
 * in the Active Object pattern.  Naturally, these roles could
 * be split apart from the Scheduler.
 */
class Scheduler : public ACE_Task<ACE_SYNCH>
{

  friend class Method_Request_print;
  friend class Method_Request_end;
public:
  // = Initialization and termination methods.
  /// Constructor.
  Scheduler (Scheduler * = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  /// Initializer.
  virtual int open (void *args = 0);

  /// Terminator.
  virtual int close (u_long flags = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  /// Destructor.
  virtual ~Scheduler (void);

  // = These methods are part of the Active Object Proxy interface.
  void print (Printer_var &printer);
  void end (void);

protected:
  /// Runs the Scheduler's event loop, which dequeues <Method_Requests>
  /// and dispatches them.
  virtual int svc (void);

private:
  // = These are the <Scheduler> implementation details.
  ACE_Activation_Queue activation_queue_;
  Scheduler *scheduler_;
};

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
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer reference count: %d\n"),
              printer_.count ()));
}

Method_Request_print::~Method_Request_print (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Method_Request_print will be deleted.\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer reference count: %d\n"),
              printer_.count ()));
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
      // Dequeue the next method request (we use an auto pointer in
      // case an exception is thrown in the <call>).
      ACE_Method_Request *mo_p = this->activation_queue_.dequeue ();
      if (0 == mo_p)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) activation queue shut down\n")));
          break;
        }
      auto_ptr<ACE_Method_Request> mo (mo_p);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) calling method request\n")));
      // Call it.
      if(mo->call () == -1)
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


// This will be used in a single thread to test the reset and release
// methods. See Bugzilla #1925 for history.

typedef ACE_Refcounted_Auto_Ptr <Printer, ACE_Null_Mutex> Printer_Ptr;

static bool expect (const ACE_TCHAR *name,
                    const Printer_Ptr &ptr,
                    bool expect_null,
                    unsigned int expect_which,
                    int expect_count)
{
  if (ptr.null () != expect_null)
    {
      if (expect_null)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Expecting: %s null:: ")
                    ACE_TEXT ("Actual: Printer: %u; Count %d\n"),
                    name,
                    ptr->which_,
                    ptr.count ()));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Expecting: %s Printer: %u; Count %d:: ")
                    ACE_TEXT ("Actual: Null.\n"),
                    name,
                    expect_which,
                    expect_count));
      return false;
    }
  if (ptr.null ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Expecting: %s null:: Actual: Null.\n"),
                  name));
      return true;
    }

  // Note that count is zero based (0 means one reference, etc.)
  bool fail = (expect_which != ptr->which_) || (expect_count != ptr.count ());
  ACE_DEBUG ((fail ? LM_ERROR : LM_DEBUG,
              ACE_TEXT ("Expecting: %s Printer: %u; Count %d:: ")
              ACE_TEXT ("Actual: Printer: %u; Count %d\n"),
              name,
              expect_which,
              expect_count,
              ptr->which_,
              ptr.count ()));
  return !fail;
}

static int test_reset_release (void)
{
  int errors = 0;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test copy constructor\n")));
  Printer_Ptr bar(new Printer ("1"));
  Printer_Ptr fum = bar;
  if (!expect (ACE_TEXT ("bar"), bar, false, 1, 1))
    ++errors;
  if (!expect (ACE_TEXT ("fum"), fum, false, 1, 1))
    ++errors;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test reset to a new value\n")));
  bar.reset (new Printer ("2"));
  if (!expect (ACE_TEXT ("bar"), bar, false, 2, 0))
    ++errors;
  if (!expect (ACE_TEXT ("fum"), fum, false, 1, 0))
    ++errors;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test release\n")));
  Printer_Ptr fie(new Printer ("3"));
  Printer_Ptr foe = fie;
  foe.release();
  if (!expect (ACE_TEXT ("fie"), fie, false, 3, 0))
    ++errors;
  if (!expect (ACE_TEXT ("foe"), foe, true, 0, 0))
    ++errors;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test assignment to null\n")));
  Printer_Ptr fee(new Printer ("4"));
  Printer_Ptr eraser;
  fee = eraser;
  if (!expect (ACE_TEXT ("fee"), fee, true, 0, 0))
    ++errors;
  if (!expect (ACE_TEXT ("eraser"), eraser, true, 0, 0))
    ++errors;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test assignment to value\n")));
  Printer_Ptr fix(new Printer ("5"));
  Printer_Ptr fax(new Printer ("6"));
  fix = fax;
  if (!expect (ACE_TEXT ("fix"), fix, false, 6, 1))
    ++errors;
  if (!expect (ACE_TEXT ("fax"), fax, false, 6, 1))
    ++errors;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test reset to null\n")));
  Printer_Ptr fey(new Printer ("7"));
  Printer_Ptr flo = fey;
  flo.reset ();
  if (!expect (ACE_TEXT ("fey"), fey, false, 7, 0))
    ++errors;
  if (!expect (ACE_TEXT ("flo"), flo, true, 0, 0))
    ++errors;

  return errors;
}

static int test_operator(void)
{
  int errors = 0;

  // test null
  Printer_Ptr printer_null;
  if (!printer_null)
    {
    }
  else
    {
      ++errors;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("!printer_null should be false\n")));
    }
  if (printer_null)
    {
      ++errors;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("printer_null should be false\n")));
    }
  else
    {
    }

  // test not null
  Printer_Ptr printer_not_null(new Printer("check not null"));
  if (!printer_not_null)
    {
      ++errors;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("!printer_not_null should be false\n")));
    }
  else
    {
    }
  if (printer_not_null)
    {
    }
  else
    {
      ++errors;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("printer_not_null should be false\n")));
    }

  return errors;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Refcounted_Auto_Ptr_Test"));

  int test_errors = 0;

  // =========================================================================
  // The following test uses the ACE_Refcounted_Auto_Ptr in a single
  // thread of control, hence we use the ACE_Null_Mutex

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing synchronous tests...\n")));

  test_errors += test_reset_release ();

  Printer *printer1;
  ACE_NEW_RETURN (printer1,
                  Printer ("I am printer 1"),
                  -1);
  {
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r(printer1);
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r1(r);
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r2(r);
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r3(r);
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r4(r);
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r5 = r2;
    ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex> r6 = r1;
  }
  if (Printer::instance_count_ == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Printer instance count is 0; correct\n")));
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Printer instance count %d; expecting 0\n"),
                  Printer::instance_count_.value ()));
      ++test_errors;
    }

#if defined (ACE_HAS_THREADS)

  // =========================================================================
  // The following test uses the ACE_Refcounted_Auto_Ptr in multiple
  // threads of control.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing asynchronous test...\n")));

  Scheduler *scheduler_ptr;

  // Create active objects..
  ACE_NEW_RETURN (scheduler_ptr,
                  Scheduler (),
                  -1);

  auto_ptr<Scheduler> scheduler(scheduler_ptr);

  if (scheduler->open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Error opening scheduler")),
                      1);

  {
    ACE_NEW_RETURN (printer1,
                    Printer ("I am printer 2"),
                    -1);

    Printer_var r (printer1);

    for (int i = 0; i < n_loops; i++)
      // Spawn off the methods, which run in a separate thread as
      // active object invocations.
      scheduler->print (r);
  }

  // Close things down.
  scheduler->end ();

  scheduler->wait ();

  if (Printer::instance_count_ == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Printer instance count is 0; correct\n")));
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Printer instance count %d; expecting 0\n"),
                  Printer::instance_count_.value ()));
      ++test_errors;
    }

#endif /* ACE_HAS_THREADS */

  test_errors += test_operator();

  ACE_END_TEST;

  return test_errors;
}
