//
// $Id$
//

#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/SString.h"

/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

  /// Constructor
  Client_Task (const char *input,
               const int named_orbs,
               int argc,
               char *argv [],
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:

  const char *input_;

  ACE_CString name_;

  int argc_;
  char **argv_;
};

#include "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
