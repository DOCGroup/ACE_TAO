
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
               CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr);
  Client_Task (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  ~Client_Task () override = default;

  /// Thread entry point
  int svc () override;

private:
  /// Reference to the test interface
  Test::Hello_var const receiver_;

  PortableServer::Servant_var<Reply_Handler> const handler_;

  Test::AMI_HelloHandler_var const handler_var_;

  CORBA::ORB_var const orb_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_TASK_H */
