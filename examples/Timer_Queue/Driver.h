/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Driver.h
 *
 *  $Id$
 *
 *  This code builds an abstraction to factor out common code for
 *  the different implementations of the Timer_Queue.
 *
 *
 *  @author Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"

/**
 * @class Command
 *
 * @brief Defines an abstract class that allows us to invoke commands
 * without knowing anything about the implementation.  This class
 * is used in the <Timer_Queue_Test_Driver> to invoke operations
 * of the driver.
 *
 * This class declares an interface to execute operations,
 * binding a RECEIVER object with an ACTION.  The RECEIVER knows
 * how to implement the operation.  A class can invoke operations
 * without knowing anything about it, or how it was implemented.
 */
template <class RECEIVER, class ACTION>
class Command
{
public:
  /// Sets the <receiver_> of the Command to recvr, and the
  /// <action_> of the Command to <action>.
  Command (RECEIVER &recvr, ACTION action);

  virtual ~Command (void);

  /// Invokes the method <action_> from the object <receiver_>.
  virtual int execute (void *arg);

private:
  /// object where the method resides.
  RECEIVER &receiver_;

  /// method that is going to be invoked.
  ACTION action_;
};

/**
 * @class Timer_Queue_Test_Driver
 *
 * @brief Defines a class that provides a simmple implementation for
 * a test driver for timer queues.
 *
 * This is the place where the common code to test the different
 * implementations of the timer queue resides.  This class has
 * the logic for the parse_commands() method, the run_test(),
 * read_input() and the get_next_request().  Subclasses can
 * override these methods if there is some logic that is specific
 * to that implementation.
 */
template <class TQ, class RECEIVER, class ACTION>
class Timer_Queue_Test_Driver
{
public:
  /// Default destructor
  virtual ~Timer_Queue_Test_Driver (void);

  /// Breaks up the input string buffer into pieces and executes
  /// the appropriate method to handle that operation.
  virtual int parse_commands (const char *buf);

  /**
   * This is the main entry point to the test driver.  The user
   * of the class should normally invoke this method.
   * Returns 0 when successful, or 0 otherwise.
   */
  virtual int run_test (void);

  /// This internal method gets the next request from the user.
  /// Returns -1 when user wants to exit.  Returns 0 otherwise.
  virtual int get_next_request (void);

  /**
   * Reads input from the user into the buffer <buf> with a maximum
   * of <bufsiz> bytes.  Returns the amount of bytes actually read
   * Otherwise, a -1 is returned and errno is set to indicate the error.
   */
  virtual ssize_t read_input (char *buf, size_t bufsiz);

  // = Template Methods.

  /// Prints the user interface for the driver to STDOUT.
  virtual int display_menu (void)=0;

  /// Initializes values and operations for the driver.
  virtual int init (void)=0;

protected:
  /// timer queue
  TQ timer_queue_;

  // = Set of <Command>s to be executed.

  /// schedule timer command
  Command<RECEIVER, ACTION> *schedule_cmd_;

  /// cancel timer command.
  Command<RECEIVER, ACTION> *cancel_cmd_;

  /// list timers command.
  Command<RECEIVER, ACTION> *list_cmd_;

  /// shutdown the driver.
  Command<RECEIVER, ACTION> *shutdown_cmd_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Driver.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Driver.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _DRIVER_H_ */
