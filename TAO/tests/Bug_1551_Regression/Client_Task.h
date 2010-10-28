//
// $Id$
//

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include /**/ "ace/pre.h"

#include "TestC.h"
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

  PortableServer::Servant_var <Reply_Handler> handler_;

  Test::AMI_HelloHandler_var handler_var_;

  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_TASK_H */
