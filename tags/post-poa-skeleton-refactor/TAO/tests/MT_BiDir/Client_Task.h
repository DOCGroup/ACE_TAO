//
// $Id$
//

#ifndef MT_BIDIR__CLIENT_TASK_H
#define MT_BIDIR_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"


/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* MT_BIDIR_CLIENT_TASK_H */
