//
// $Id$
//

#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/ORB.h"


/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

enum ClientSyncModeEnum {
   MODE_NONE,
   MODE_TRANSPORT,
   MODE_SERVER,
   MODE_TARGET
};

  /// Constructor
  Client_Task (const char *input,
               CORBA::ORB_ptr corb,
               ClientSyncModeEnum syncMode,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:
  const char *input_;

  CORBA::ORB_var corb_;

  ClientSyncModeEnum syncMode_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
