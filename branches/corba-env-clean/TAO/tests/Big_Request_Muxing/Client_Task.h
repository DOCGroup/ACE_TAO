//
// $Id$
//

#ifndef BIG_REQUEST_MUXING_CLIENT_TASK_H
#define BIG_REQUEST_MUXING_CLIENT_TASK_H
#include "ace/pre.h"

#include "TestC.h"
#include "tao/Messaging.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (ACE_Thread_Manager *thr_mgr,
               Test::Payload_Receiver_ptr receiver,
               CORBA::Long event_count,
               CORBA::ULong event_size,
               CORBA::ORB_ptr orb,
               Messaging::SyncScope sync_scope);

  /// Thread entry point
  int svc (void);

private:
  /// Make sure that all threads have connections available to
  /// workaround bug 189
  void validate_connection (TAO_ENV_SINGLE_ARG_DECL);

private:
  /// Reference to the test interface
  Test::Payload_Receiver_var payload_receiver_;

  /// Total number of events
  CORBA::Long event_count_;

  /// Size of each message
  CORBA::ULong event_size_;

  /// Keep a reference to the ORB, used to initialize the SyncScope
  /// policy
  CORBA::ORB_var orb_;

  /// The SyncScope used in this Task
  Messaging::SyncScope sync_scope_;
};

#include "ace/post.h"
#endif /* BIG_REQUEST_MUXING_CLIENT_TASK_H */
