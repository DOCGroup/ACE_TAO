//
// $Id$
//

#ifndef MUXING_CLIENT_TASK_H
#define MUXING_CLIENT_TASK_H
#include "ace/pre.h"

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
  Client_Task (Test::Receiver_ptr receiver,
               CORBA::Long event_count,
               CORBA::ULong event_size,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the test interface
  Test::Receiver_var receiver_;

  /// Total number of events
  CORBA::Long event_count_;

  /// Size of each message
  CORBA::ULong event_size_;
};

#include "ace/post.h"
#endif /* MUXING_CLIENT_TASK_H */
