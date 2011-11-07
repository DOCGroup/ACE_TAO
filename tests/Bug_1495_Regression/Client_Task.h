// -*- C++ -*-

/**
 * @file Client_Task.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>'
 *
 * $Id$
 *
 * Defines the Client_Task class which acts as the process colocated client
 * thread for the Bug1492_Regression test.
 *
 */


#ifndef BUG1492_REGRESSION_CLIENT_TASK_H
#define BUG1492_REGRESSION_CLIENT_TASK_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#include "tao/ORB.h"

/// Implements a Task to run the process colocated client ORB.

class Client_Task : public ACE_Task_Base
{
 public:
  /// Constructor
  Client_Task (const ACE_TCHAR *input,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point.
  int svc (void);

private:
  /// File to read in the remote IOR.
  const ACE_TCHAR *input_;

  /// Client ORB.
  CORBA::ORB_var corb_;
};


#include /**/ "ace/post.h"
#endif /* BUG1492_REGRESSION_CLIENT_TASK_H */
