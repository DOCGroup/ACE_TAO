//
// $Id$
//

#ifndef SERVER_TASK_H
#define SERVER_TASK_H
#include "ace/pre.h"

#include "tao/corba.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (ACE_Thread_Manager *thr_mgr,
               CORBA::ORB_ptr orb,
               const ACE_Time_Value &running_time);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;

  /// Total running time for each thread
  ACE_Time_Value running_time_;
};

#include "ace/post.h"
#endif /* SERVER_TASK_H */
