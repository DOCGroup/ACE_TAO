//
// $Id$
//

#ifndef SERVER_TASK_H
#define SERVER_TASK_H
#include "ace/pre.h"

#include "tao/corba.h"
#include "ace/Task.h"

/// Implement a simple ACE_Task to run the ORB event loop
/**
 * This class is used to implement a simple thread pool.
 */
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (CORBA::ORB_ptr orb);

  /// The thread entry point
  virtual int svc (void);

private:
  /// Keep a reference to the ORB
  CORBA::ORB_var orb_;
};

#include "ace/post.h"
#endif /* SERVER_TASK_H */
