//
// $Id$
//

#ifndef LONGUPCALLS_MANAGER_H
#define LONGUPCALLS_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

class Manager
  :  public virtual POA_Test::Manager
{
public:
  Manager (CORBA::ORB_ptr orb);
  // Constructor

  // = The skeleton methods
  virtual void start_workers (CORBA::Short worker_count,
                              CORBA::Long milliseconds,
                              Test::Controller_ptr controller);
  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB, to shutdown
};

// ****************************************************************

class Worker : public ACE_Task_Base
{
public:
  Worker (ACE_Thread_Manager *thr_mgr,
          Test::Controller_ptr controller,
          CORBA::Long milliseconds);

  virtual int svc (void);
  // The service method

private:
  Test::Controller_var controller_;
  // The controller pointer

  CORBA::Long milliseconds_;
  // The number of milliseconds.
};

#endif /* LONGUPCALLS_MANAGER_H */
