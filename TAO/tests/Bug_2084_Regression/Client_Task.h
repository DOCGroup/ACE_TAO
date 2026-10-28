
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
  /// Constructor
  Client_Task (ACE_TCHAR const *input,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);
  Client_Task (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  ACE_TCHAR const *input_;

  CORBA::ORB_var corb_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
