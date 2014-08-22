//
// $Id$
//

#ifndef STACK_RECURSION_SERVER_TASK_H
#define STACK_RECURSION_SERVER_TASK_H
#include /**/ "ace/pre.h"
#include "tao/ORB.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Task.h"

class Sender;

/// Implement a Task to run the experiments using multiple threads.
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (CORBA::ORB_ptr orb,
               const Sender *ptr,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;

  /// The sender pointer
  const Sender *sender_;
};

#include /**/ "ace/post.h"
#endif /* STACK_RECURSION_SERVER_TASK_H */
