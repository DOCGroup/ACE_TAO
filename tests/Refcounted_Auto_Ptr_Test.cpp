// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Refcounted_Auto_Ptr_Test.cpp
//
// = DESCRIPTION
//    This example tests the <ACE_Refcounted_Auto_Ptr> and illustrates
//    how they may be dispersed between multiple threads using an
//    implementation of the Active Object pattern, which is available
//    at <http://www.cs.wustl.edu/~schmidt/PDF/Act-Obj.pdf>.
//
// = AUTHOR
//    Johnny Tucker <johnny_tucker@yahoo.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Refcounted_Auto_Ptr.h"

ACE_RCSID(tests, Refcounted_Auto_Ptr_Test, "Refcounted_Auto_Ptr_Test.cpp,v 4.8 2000/04/23 04:43:58 brunsch Exp")

#if defined (ACE_HAS_THREADS)

struct Printer
{
  Printer (const char *message);
  ~Printer (void) ;

  void print (void);

  const char *message_;
};

typedef ACE_Refcounted_Auto_Ptr<Printer, ACE_Thread_Mutex> Printer_var;

Printer::Printer (const char *message) 
  : message_ (message)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Creating Printer object\n")));
}

Printer::~Printer (void) 
{
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

class Scheduler : public ACE_Task_Base
{
  // = TITLE
  //     The scheduler for the Active Object.
  //
  // = DESCRIPTION
  //     This class also plays the role of the Proxy and the Servant
  //     in the Active Object pattern.  Naturally, these roles could
  //     be split apart from the Scheduler.

  friend class Method_Request_print;
  friend class Method_Request_end;
public:
  // = Initialization and termination methods.
  Scheduler (Scheduler * = 0);
  // Constructor.

  virtual int open (void *args = 0);
  // Initializer.

  virtual int close (u_long flags = 0);
  // Terminator.

  virtual ~Scheduler (void);
  // Destructor.

  // = These methods are part of the Active Object Proxy interface.
  void print (Printer_var &printer);
  void end (void);

protected:
  virtual int svc (void);
  // Runs the Scheduler's event loop, which dequeues <Method_Requests>
  // and dispatches them.

private:
  // = These are the <Scheduler> implementation details.
  ACE_Activation_Queue activation_queue_;
  Scheduler *scheduler_;
};

class Method_Request_print : public ACE_Method_Request
{
  // = TITLE
  //     Reification of the <print> method.
public:
  Method_Request_print (Scheduler *,
                        Printer_var &printer);
  virtual ~Method_Request_print (void);

  virtual int call (void);
  // This is the entry point into the Active Object method.

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

class Method_Request_end : public ACE_Method_Request
{
  // = TITLE
  //     Reification of the <end> method.
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
  // Shut down the scheduler.
  this->scheduler_->close ();
  return -1;
}

// Constructor
Scheduler::Scheduler (Scheduler *new_scheduler)
  : scheduler_ (new_scheduler)
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
  return this->activate (THR_BOUND | THR_DETACHED, num_threads);
}

// close

int
Scheduler::close (u_long)
{
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
      auto_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

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
template class ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex>;
template class ACE_Auto_Basic_Ptr<Printer>;
template class ACE_Auto_Basic_Ptr<Scheduler>;
template class auto_ptr<Scheduler>;
template class auto_ptr<ACE_Method_Request>;
template class ACE_Auto_Basic_Ptr<ACE_Method_Request>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Refcounted_Auto_Ptr<Printer, ACE_Thread_Mutex>
#pragma instantiate ACE_Refcounted_Auto_Ptr<Printer, ACE_Null_Mutex>
#pragma instantiate ACE_Auto_Basic_Ptr<Printer>
#pragma instantiate ACE_Auto_Basic_Ptr<Scheduler>
#pragma instantiate auto_ptr<Scheduler>
#pragma instantiate auto_ptr<ACE_Method_Request>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Method_Request>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Refcounted_Auto_Ptr_Test"));

#if defined (ACE_HAS_THREADS)

  // ============================================================================
  // The following test uses the ACE_Refcounted_Auto_Ptr in a single
  // thread of control, hence we use the ACE_Null_Mutex

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing synchronous test on Printer object\n")));

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
              ACE_TEXT ("(%t) Printer object should have been deleted\n")));

  // ============================================================================
  // The following test uses the ACE_Refcounted_Auto_Ptr in a single
  // thread of control, hence we use the ACE_Null_Mutex

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) performing asynchronous test on Printer object\n")));

  // @@ Should make these be <auto_ptr>s...
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

    for(int i = 0; i < n_loops; i++)
      // Spawn off the methods, which run in a separate thread as
      // active object invocations.
      scheduler->print (r);
  }

  // Close things down.
  scheduler->end (); 

  ACE_OS::sleep (5);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Printer object should have been deleted\n")));

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
