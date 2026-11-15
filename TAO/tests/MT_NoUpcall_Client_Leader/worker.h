#if !defined( WORKER_H)
#define WORKER_H

#include /**/ "ace/pre.h"
#include "tao/corba.h"
#include "ace/Task.h"

/// Runs a server thread using `ACE_Task_Base`.
class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;

  /// Entry point for the server thread.
  int svc () override;

public:
  /// ORB used by this worker.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"

#endif /* WORKER_H */
