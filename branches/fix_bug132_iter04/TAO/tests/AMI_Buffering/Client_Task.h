//
// $Id$
//

#ifndef AMI_BUFFERING_CLIENT_TASK_H
#define AMI_BUFFERING_CLIENT_TASK_H
#include "ace/pre.h"

#include "tao/corba.h"
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

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;
};

#include "ace/post.h"
#endif /* AMI_BUFFERING_CLIENT_TASK_H */
