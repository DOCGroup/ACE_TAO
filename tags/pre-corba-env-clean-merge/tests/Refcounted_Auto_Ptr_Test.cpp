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
 *
 *  @author Johnny Tucker <johnny_tucker@yahoo.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Refcounted_Auto_Ptr.h"

ACE_RCSID (tests,
           Refcounted_Auto_Ptr_Test,
           "Refcounted_Auto_Ptr_Test.cpp,v 4.8 2000/04/23 04:43:58 brunsch Exp")

struct Printer
{
  Printer (const char *message);
  ~Printer (void) ;

  void print (void);

  const char *message_;
  static size_t instance_count_;
};

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
              ACE_TEXT ("(%t) %s\n"),
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

  /// Initializer.
  virtual int open (void *args = 0);

  /// Terminator.
  virtual int close (u_long flags = 0);

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Refcounted_Auto_Ptr<Printer, ACE_Thread_Mutex>;
template class ACE_Auto_Basic_Ptr<Printer>;
template class ACE_Auto_Basic_Ptr<Scheduler>;
template class auto_ptr<Scheduler>;
template class auto_ptr<ACE_Method_Request>;
template class ACE_Auto_Basic_Ptr<ACE_Method_Request>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Refcounted_Auto_Ptr<Printer, ACE_Thread_Mutex>
#pragma instantiate ACE_Auto_Basic_Ptr<Printer>
#pragma instantiate ACE_Auto_Basic_Ptr<Scheduler>
#pragma instantiate auto_ptr<Scheduler>
#pragma instantiate auto_ptr<ACE_Method_Request>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Method_Request>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Refcounted_Auto_Ptr_Test"));


  // =========================================================================
  // The following test uses the ACE_Refcounted_Auto_Ptr in a single
  // thread of control, hence we use the ACE_Null_Mutex

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing synchronous test...\n")));

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
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer instance count is %d, expecting 0\n"),
              Printer::instance_count_));
  ACE_ASSERT (Printer::instance_count_ == 0);

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

  ACE_ASSERT (scheduler->open () != -1);

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

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer instance count is %d, expecting 0\n"),
              Printer::instance_count_));
  ACE_ASSERT (Printer::instance_count_ == 0);

#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;

  return 0;
}
