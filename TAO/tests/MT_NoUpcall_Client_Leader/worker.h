#if !defined( WORKER_H)
#define WORKER_H

#include /**/ "ace/pre.h"
#include "tao/corba.h"
#include "ace/Task.h"

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;

  int svc () override;
  // The thread entry point.

public:
  // The orb
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"

#endif /* WORKER_H */
