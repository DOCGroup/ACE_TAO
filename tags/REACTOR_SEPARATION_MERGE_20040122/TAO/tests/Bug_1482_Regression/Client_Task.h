//
// $Id$
//

#ifndef LONGUPCALLS_CLIENT_TASK_H
#define LONGUPCALLS_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "TestC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Reply_Handler.h"
#include "ace/Task.h"

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (Test::Hello_ptr receiver,
               CORBA::ORB_ptr o,
               ACE_Thread_Manager *thr_mgr);

  ~Client_Task (void);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the test interface
  Test::Hello_var receiver_;

  Reply_Handler handler_;

  Test::AMI_HelloHandler_var handler_var_;

  CORBA::ORB_var orb_;
};
#include /**/ "ace/post.h"
#endif /* LONGUPCALLS_CLIENT_TASK_H */
