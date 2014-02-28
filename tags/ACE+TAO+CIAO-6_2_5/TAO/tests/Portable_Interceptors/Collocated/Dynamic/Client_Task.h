//
// $Id$
//

#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "ace/Task.h"
#include "testS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

  /// Constructor
  Client_Task (const ACE_TCHAR *input,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

  void run_test (Test_Interceptors::Visual_ptr server);

private:
  const ACE_TCHAR *input_;

  CORBA::ORB_var corb_;

};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
