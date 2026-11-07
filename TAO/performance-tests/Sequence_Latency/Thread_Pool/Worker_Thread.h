#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "ace/Task.h"

/// Implement the Test::Worker_Thread interface
class Worker_Thread : public ACE_Task_Base
{
public:
  /// Constructor
  explicit Worker_Thread (CORBA::ORB_ptr orb);
  Worker_Thread (Worker_Thread const &) = delete;
  Worker_Thread (Worker_Thread &&) = delete;
  Worker_Thread &operator= (Worker_Thread const &) = delete;
  Worker_Thread &operator= (Worker_Thread &&) = delete;

  // = The service method
  int svc () override;

private:
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* WORKER_THREAD_H */
