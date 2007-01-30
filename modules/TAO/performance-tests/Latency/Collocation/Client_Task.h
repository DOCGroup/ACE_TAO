//
// $Id$
//

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
  Client_Task (Test::Roundtrip_ptr reference,
               int iterations,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:

  const int niterations;
  Test::Roundtrip_var remote_ref_;

};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
