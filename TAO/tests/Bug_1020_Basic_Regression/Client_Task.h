
#ifndef BUG_1020_BASIC_REGRESSION_CLIENT_TASK_H
#define BUG_1020_BASIC_REGRESSION_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "TestC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (CORBA::ORB_ptr orb);
  Client_Task (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BUG_1020_BASIC_REGRESSION_CLIENT_TASK_H*/
