//
// $Id$
//

#ifndef COLLOCATED_SERVER_TASK_H
#define COLLOCATED_SERVER_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

/// Implement a Task to run the server in a single thread
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (char* ior_file,
               CORBA::ORB_ptr sorb,
               ACE_Null_Condition &cond,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:

  const char* ior_file;
  /// Manual event to wake up the main thread to create a client
  /// thread.
  ACE_Null_Condition &cond_;

  CORBA::ORB_var sorb_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_SERVER_TASK_H */
