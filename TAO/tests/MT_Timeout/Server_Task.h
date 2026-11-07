
#ifndef SERVER_TASK_H
#define SERVER_TASK_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
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
               ACE_Time_Value const& running_time);

  Server_Task (Server_Task const&) = delete;
  Server_Task (Server_Task&&) = delete;
  Server_Task& operator= (Server_Task const&) = delete;
  Server_Task& operator= (Server_Task&&) = delete;

  /// Thread entry point
  int svc () override;

private:
  /// Reference to the ORB
  CORBA::ORB_var const orb_;

  /// Total running time for each thread
  ACE_Time_Value running_time_;
};

#include /**/ "ace/post.h"
#endif /* SERVER_TASK_H */
