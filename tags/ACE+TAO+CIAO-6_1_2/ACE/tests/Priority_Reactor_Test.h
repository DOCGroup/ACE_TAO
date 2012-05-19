
//=============================================================================
/**
 *  @file    Priority_Reactor_Test.h
 *
 *  $Id$
 *
 *  This class gets its own header file to work around AIX C++
 *  compiler "features" related to template instantiation...  It is
 *  only used by Priority_Reactor_Test.cpp.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_PRIORITY_REACTOR_TEST_H
#define ACE_TESTS_PRIORITY_REACTOR_TEST_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

/**
 * @class Read_Handler
 *
 * @brief A Svc_Handler with a priority twist.
 *
 * This Svc_Handler receives the data sent by the childs or writer
 * threads; each one sets it own priority to a new level, in a
 * cyclic manner.  The main point is test and exercise the
 * priority dispatching features of ACE_Priority_Reactor.
 */
class Read_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  /// Set the number of children or writer threads we will be running,
  /// when they are all gone we terminate the reactor loop.
  static void set_countdown (int nchildren);

  /// Get the number of children we are still waiting for.
  static int get_countdown (void);

  /// The Svc_Handler callbacks.
  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE h);

private:
  /// How many writers are we waiting for.
  static int waiting_;

  /// How many readers have started.
  static int started_;
};

/**
 * @class Write_Handler
 *
 * @brief A simple writer.
 *
 * This Svc_Handler simply connects to a server and sends some
 * output to it.  Its purpose is to feed the test.
 */
class Write_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  virtual int open (void *);
  virtual int svc (void);
};

#endif /* ACE_TESTS_PRIORITY_REACTOR_TEST_H */
