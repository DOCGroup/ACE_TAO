
//=============================================================================
/**
 *  @file    Reactor_Performance_Test.h
 *
 *  This file includes template class definitions for the
 *  Reactor_Performance_Test.cpp program.
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
  int open (void *) override;
  int handle_input (ACE_HANDLE h) override;
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask) override;

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
  int open (void *) override;
  virtual int send_data ();
};

#endif /* ACE_TESTS_REACTOR_PERFORMANCE_TEST_H */
