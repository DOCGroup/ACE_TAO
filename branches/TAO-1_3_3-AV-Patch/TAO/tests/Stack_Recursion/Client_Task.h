//
// $Id$
//

#ifndef STACK_RECURSION_CLIENT_TASK_H
#define STACK_RECURSION_CLIENT_TASK_H
#include /**/ "ace/pre.h"
#include "TestC.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (Test::Sender_ptr receiver,
               CORBA::Long event_count,
               CORBA::ULong event_size,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:

  /// Simple pings to prune up the connections
  void validate_connections (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// Reference to the test interface
  Test::Sender_var sender_;

  /// Total number of events
  CORBA::Long event_count_;

  /// Size of each message
  CORBA::ULong event_size_;
};

#include /**/ "ace/post.h"
#endif /* STACK_RECURSION_CLIENT_TASK_H */
