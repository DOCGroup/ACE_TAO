//
// $Id$
//

#ifndef BIG_REPLY_CLIENT_TASK_H
#define BIG_REPLY_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "TestC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (Test::Big_Reply_ptr receiver,
               CORBA::Long event_count,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:

  /// Validate the connection
  void validate_connection (void);

private:

  /// Reference to the test interface
  Test::Big_Reply_var reply_gen_;

  /// Number of remote calls
  int event_count_;
};

#include /**/ "ace/post.h"
#endif /* BIG_REPLY_CLIENT_TASK_H */
