// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Performance_Test.h
//
// = DESCRIPTION
//
//    This file includes template class definitions for the
//    Reactor_Performance_Test.cpp program.
//
// = AUTHOR
//    Irfan Pyarali, Steve Huston
//
// ============================================================================

#ifndef __REACTOR_PERFORMANCE_TEST_H
#define __REACTOR_PERFORMANCE_TEST_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

// Simple class for reading in the data
class Read_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  static void set_countdown (int nconnections);

  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  // The Svc_Handler callbacks.

private:
  static int waiting_;
  // How many connections are we waiting for.
};



// This Svc_Handler simply connects to a server and sends some output
// to it.  Its purpose is to feed the test.
class Write_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  virtual int open (void *);
  virtual int send_data (void);
};


#endif /* __REACTOR_PERFORMANCE_TEST_H */
