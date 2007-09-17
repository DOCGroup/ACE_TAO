#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "ace/Task.h"

/// Implement a simple ACE_Task to run the ORB event loop
/**
 * This class is used to implement a simple thread pool.
 */
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (CORBA::ORB_ptr orb);

  /// The thread entry point
  virtual int svc (void);

private:
  /// Keep a reference to the ORB
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_TASK_H */
