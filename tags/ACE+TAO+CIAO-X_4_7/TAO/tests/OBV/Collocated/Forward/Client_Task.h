//
// $Id$
//

#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"
#include "TreeBaseC.h"
#include "TreeControllerC.h"
#include "TreeNodeC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/ORB.h"


/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

  /// Constructor
  Client_Task (const char *input,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

  void dump_node (BaseNode *bn, int indent);

  void dump_tree (TreeController *tc);

  int is_equal_tree (TreeController * tree1, TreeController * tree2);

  int is_equal_node (BaseNode * node1, BaseNode * node2);

private:
  const char *input_;

  CORBA::ORB_var corb_;

};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
