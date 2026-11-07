
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
               Sender const *sender,
               ACE_Thread_Manager *thr_mgr);
  Server_Task (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;

  /// The sender pointer
  Sender const *const sender_;
};

#include /**/ "ace/post.h"
#endif /* STACK_RECURSION_SERVER_TASK_H */
