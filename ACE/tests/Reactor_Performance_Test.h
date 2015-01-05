
//=============================================================================
/**
 *  @file    Reactor_Performance_Test.h
 *
 *  $Id$
 *
 *  This file includes template class definitions for the
 *  Reactor_Performance_Test.cpp program.  This class gets its own
 *  header file to work around AIX C++ compiler "features" related
 *  to template instantiation...
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu> and Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#ifndef ACE_TESTS_REACTOR_PERFORMANCE_TEST_H
#define ACE_TESTS_REACTOR_PERFORMANCE_TEST_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"

/**
 * @class Read_Handler
 *
 * @brief Simple class for reading in the data
 */
class Read_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  static void set_countdown (int nconnections);

  /// The Svc_Handler callbacks.
  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:
  /// How many connections are we waiting for.
  static int waiting_;
};

/**
 * @class Write_Handler
 *
 * @brief This Svc_Handler simply connects to a server and sends some
 * output to it.  Its purpose is to feed the test.
 */
class Write_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  virtual int open (void *);
  virtual int send_data (void);
};

#endif /* ACE_TESTS_REACTOR_PERFORMANCE_TEST_H */
