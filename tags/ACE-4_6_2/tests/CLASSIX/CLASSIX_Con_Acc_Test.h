/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Priority_Reactor_Test.h
//
// = DESCRIPTION
//    This class gets its own header file to work around AIX C++
//    compiler "features" related to template instantiation...  It is
//    only used by Priority_Reactor_Test.cpp.
//
// = AUTHOR
//    Carlos O'Ryan
//
// ============================================================================

#ifndef ACE_TESTS_PRIORITY_REACTOR_TEST_H
#define ACE_TESTS_PRIORITY_REACTOR_TEST_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch_T.h"

#define ACE_CLASSIX_STREAM ACE_CLASSIX_Stream, ACE_CLASSIX_Addr
#define ACE_CLASSIX_CO_CONNECTOR ACE_CLASSIX_CO_Connector, ACE_CLASSIX_Port
#define ACE_CLASSIX_CO_ACCEPTOR ACE_CLASSIX_CO_Acceptor, ACE_CLASSIX_Port

class Read_Handler : public ACE_Svc_Handler<ACE_CLASSIX_STREAM, ACE_SYNCH>
  // = TITLE
  //   A Svc_Handler with a priority twist.
  //
  // = DESCRIPTION
  //   This Svc_Handler receives the data sent by the childs or writer
  //   threads; each one sets it own priority to a new level, in a
  //   cyclic manner.  The main point is test and exercise the
  //   priority dispatching features of ACE_Priority_Reactor.
{
public:
  static void set_countdown (int nchildren);
  // Set the number of children or writer threads we will be running,
  // when they are all gone we terminate the reactor loop.

  static int get_countdown (void);
  // Get the number of children we are still waiting for.

  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE h);
  // The Svc_Handler callbacks.

private:
  static ACE_Atomic_Op<ACE_Thread_Mutex, int>  waiting_;
  // How many writers are we waiting for.

  static ACE_Atomic_Op<ACE_Thread_Mutex, int>  started_;
  // How many readers have started.
};

class Write_Handler : public ACE_Svc_Handler<ACE_CLASSIX_STREAM, ACE_SYNCH>
  // = TITLE
  //   A simple writer.
  //
  // = DESCRIPTION
  //   This Svc_Handler simply connects to a server and sends some
  //   output to it.  Its purpose is to feed the test.
{
public:
  virtual int open (void *);
  virtual int svc (void);
};

#endif /* ACE_TESTS_PRIORITY_REACTOR_TEST_H */
