
//=============================================================================
/**
 *  @file    Bound_Ptr_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================


#ifndef ACE_TESTS_BOUND_PTR_TEST_H
#define ACE_TESTS_BOUND_PTR_TEST_H

#include "ace/Activation_Queue.h"
#include "ace/Bound_Ptr.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"

struct Printer
{
  Printer (const char *message);
  ~Printer (void) ;

  void print (void);

  const char *message_;
  static size_t instance_count_;
};

#if defined (ACE_HAS_THREADS)

typedef ACE_Strong_Bound_Ptr<Printer, ACE_Thread_Mutex> Printer_var;

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

#endif /* ACE_HAS_THREADS */
#endif /* ACE_TESTS_BOUND_PTR_TEST_H */
