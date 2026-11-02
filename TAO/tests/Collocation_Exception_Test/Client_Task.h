
#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/ORB.h"

#include "TestC.h"

/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (ACE_TCHAR const *input,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc () override;

  Client_Task (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

private:
  void test_system_exception (Test::Hello_ptr hello_ptr);
  void test_user_exception_expected (Test::Hello_ptr hello_ptr);
  void test_user_exception_not_expected (Test::Hello_ptr hello_ptr);

  ACE_TCHAR const *input_;

  CORBA::ORB_var corb_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
